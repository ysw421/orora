#include "server/daemon.h"
#include "utilities/utils.h"
#include "syslib/console_print.h"
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libwebsockets.h>
#include <pthread.h>
#include <json-c/json.h>
#include <readline/readline.h>
#include <readline/history.h>

jmp_buf interactive_mode_buf;
volatile sig_atomic_t running = 1;
static struct lws *latest_ws_connection = (void*) 0;

struct per_session_data__orora {
  int id;
  struct lws *wsi;
};

int port = 8080;

static struct lws_context *context;
Envs* global_env;
Envs* root_envs;

void execute_orora_code(const char* code, char* result, size_t result_size, Envs* root_envs);
int is_end_interactive_line(Lexer* root);
void* internal_client_thread(void* arg);
int create_and_bind_socket();
void run_client(int port);

char* preprocess_latex_json(const char* input) {
    if (input == NULL) return NULL;

    size_t input_len = strlen(input);
    char* output = malloc(input_len * 2 + 1);  // 최악의 경우를 고려한 메모리 할당
    if (output == NULL) return NULL;

    size_t j = 0;
    int in_latex = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (input[i] == '$') {
            in_latex = !in_latex;  // LaTeX 영역 시작/끝 토글
        }
        
        if (in_latex && input[i] == '\\') {
            output[j++] = '\\';
            output[j++] = '\\';
        } else if (input[i] == '"') {
            output[j++] = '\\';
            output[j++] = '"';
        } else if (input[i] == '\r') {
            output[j++] = '\\';
            output[j++] = 'r';
        } else if (input[i] == '\n') {
            output[j++] = '\\';
            output[j++] = 'n';
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';

    return output;
}

json_object* parse_websocket_message(const char* message) {
    json_object* json = json_tokener_parse(message);
    if (json == NULL) {
        char* preprocessed_message = preprocess_latex_json(message);
        if (preprocessed_message) {
            json = json_tokener_parse(preprocessed_message);
            free(preprocessed_message);
        }
    }
    return json;
}

void execute_orora_code(const char* code, char* result, size_t result_size, Envs* root_envs)
{
  off_t len = strlen(code);
  off_t* len_p = &len;
  Lexer* root = init_lexer((char*)code, len_p);
  Parser* parser = init_parser(root);
  AST* ast_tree = parser_parse(parser);

  if (setjmp(interactive_mode_buf) == 0)
  {
    for (int i = 0; i < ast_tree->value.compound_v->size; i++)
    {
      AST* ast = ast_tree->value.compound_v->items[i];
      AST_value_stack* new_value = visitor_visit(root_envs, ast)->output;
      if (new_value)
      {
        if (new_value->type != AST_VALUE_NULL || !(ast->type == AST_FUNCTION
            || ast->type == AST_CODE
            || ast->type == AST_FOR
            || ast->type == AST_WHILE
            || ast->type == AST_CASES
            || (ast->type == AST_VALUE && (ast->value.value_v->stack->type == AST_VALUE_CODE)
              )
            )
          )
        {
          const char* temp_result = visitor_print_function_value(new_value);
          strncpy(result, temp_result, result_size - 1);
          result[result_size - 1] = '\0';
        }
      }
      else
      {
        strncpy(result, "No output", result_size - 1);
        result[result_size - 1] = '\0';
      }
    }
  }
  else
  {
    strncpy(result, "Execution error", result_size - 1);
    result[result_size - 1] = '\0';
  }

  free(root);
  free(parser);
  free(ast_tree);
}

void handle_signal(int sig)
{
  running = 0;
}

int is_end_interactive_line(Lexer* root)
{
  Lexer* lexer = malloc(sizeof(Lexer));
  memcpy(lexer, root, sizeof(Lexer));
  Token* token = (void*) 0;
  int lpar = 0, rpar = 0, lbrace = 0, rbrace = 0, lsqb = 0, rsqb = 0, begin = 0, end = 0;

  while ((token = lexer_get_token(lexer)) != (void*) 0)
  {
    if (token->type == TOKEN_LPAR) lpar++;
    else if (token->type == TOKEN_RPAR) rpar++;
    else if (token->type == TOKEN_LBRACE) lbrace++;
    else if (token->type == TOKEN_RBRACE) rbrace++;
    else if (token->type == TOKEN_LSQB) lsqb++;
    else if (token->type == TOKEN_RSQB) rsqb++;
    else if (token->type == TOKEN_BEGIN) begin++;
    else if (token->type == TOKEN_END) end++;
  }
  free(lexer);
  if (lpar == rpar && lbrace == rbrace && lsqb == rsqb && begin == end)
    return 0;
  return lpar - rpar + lbrace - rbrace + lsqb - rsqb + begin - end;
}

void send_ws_message(const char* message)
{
  if (latest_ws_connection)
  {
    char buffer[MAX_PAYLOAD_SIZE + LWS_PRE];
    int n = snprintf(buffer + LWS_PRE, sizeof(buffer) - LWS_PRE, "%s", message);
    if (lws_write(latest_ws_connection, (unsigned char*)buffer + LWS_PRE, n, LWS_WRITE_TEXT) < n)
    {
      printf("Error: lws_write failed when sending message from internal client\n");
    }
  }
}

// static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
// {
//   switch (reason)
//   {
//     case LWS_CALLBACK_HTTP:
//     case LWS_CALLBACK_HTTP_BODY:
//     case LWS_CALLBACK_HTTP_BODY_COMPLETION:
//       {
//         unsigned char buffer[1024];
//         unsigned char *p = buffer, *end = p + sizeof(buffer);
// 
//         if (lws_add_http_header_status(wsi, HTTP_STATUS_OK, &p, end))
//           return 1;
//         if (lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_ACCESS_CONTROL_ALLOW_ORIGIN, (unsigned char *)"*", 1, &p, end))
//           return 1;
//         if (lws_add_http_header_by_name(wsi, (unsigned char *)"Access-Control-Allow-Methods:", (unsigned char *)"GET, POST, OPTIONS", 18, &p, end))
//           return 1;
//         if (lws_add_http_header_by_name(wsi, (unsigned char *)"Access-Control-Allow-Headers:", (unsigned char *)"Content-Type", 12, &p, end))
//           return 1;
// 
//         if (lws_finalize_http_header(wsi, &p, end))
//           return 1;
// 
//         if (lws_write(wsi, buffer, p - buffer, LWS_WRITE_HTTP_HEADERS) < 0)
//           return 1;
// 
//         if (lws_hdr_total_length(wsi, WSI_TOKEN_OPTIONS_URI))
//           return lws_http_transaction_completed(wsi);
// 
//         if (!lws_hdr_total_length(wsi, WSI_TOKEN_POST_URI))
//           return lws_http_transaction_completed(wsi);
// 
//         if (reason == LWS_CALLBACK_HTTP_BODY) 
//         {
//           char *body = (char *)in;
// 
//           json_object *jobj = json_tokener_parse(body);
//           if (jobj == (void*) 0) 
//           {
//             printf("Failed to parse JSON\n");
//             return lws_http_transaction_completed(wsi);
//           }
// 
//           json_object *code_obj;
//           if (!json_object_object_get_ex(jobj, "code", &code_obj)) 
//           {
//             printf("Missing 'code' field in JSON\n");
//             json_object_put(jobj);
//             return lws_http_transaction_completed(wsi);
//           }
// 
//           const char *code = json_object_get_string(code_obj);
// 
//           char result[BUFFER_SIZE] = {0};
//           execute_orora_code(code, result, BUFFER_SIZE, root_envs);
// 
//           json_object_put(jobj);
// 
//           char response[BUFFER_SIZE];
//           int written = snprintf(response, BUFFER_SIZE, "{\"status\":\"success\",\"result\":\"%s\"}", result);
//           if (written >= BUFFER_SIZE) 
//           {
//             printf("Warning: Response truncated\n");
//           }
// 
//           if (lws_write_http(wsi, response, strlen(response)) < 0)
//             return 1;
// 
//           return lws_http_transaction_completed(wsi);
//         }
//       }
//       break;
// 
//     default:
//       break;
//   }
// 
//   return lws_callback_http_dummy(wsi, reason, user, in, len);
// }

struct lws* wsi_global = (void*) 0;
const char* cell_id_global = (void*) 0;


static int callback_orora(struct lws* wsi, enum lws_callback_reasons reason,
    void* user, void* in, size_t len)
{
  wsi_global = wsi;

  struct per_session_data__orora *pss = (struct per_session_data__orora *)user;
  char buffer[MAX_PAYLOAD_SIZE + LWS_PRE];
  int n;

  switch (reason)
  {
    case LWS_CALLBACK_ESTABLISHED:
      pss->id = rand();
      printf("WebSocket connection established\n");
      break;

    case LWS_CALLBACK_RECEIVE:
      printf("WebSocket message received: %.*s\n", (int)len, (char *)in);
      if (len > MAX_PAYLOAD_SIZE)
        break;

      json_object *jobj = parse_websocket_message((const char*) in);
      if (jobj == (void*) 0)
      {
        printf("Failed to parse JSON\n");
        break;
      }

      json_object *type_obj, *id_obj, *code_obj;
      if (!json_object_object_get_ex(jobj, "type", &type_obj) ||
          !json_object_object_get_ex(jobj, "id", &id_obj) ||
          !json_object_object_get_ex(jobj, "code", &code_obj))
      {
        printf("Missing required JSON fields\n");
        json_object_put(jobj);
        break;
      }

      const char* type = json_object_get_string(type_obj);
      const char* cell_id = json_object_get_string(id_obj);
      cell_id_global = cell_id;
      const char* code = json_object_get_string(code_obj);

      printf("Received message - Type: %s, Cell ID: %s, Code: %s\n", type, cell_id, code);

      if (strcmp(type, "execute") == 0)
      {
//         n = snprintf(buffer + LWS_PRE, sizeof(buffer) - LWS_PRE,
//             "{\"type\":\"execution_status\",\"id\":\"%s\",\"status\":\"running\",\"result\":\"Execution started...\"}", cell_id);
//         if (lws_write(wsi, (unsigned char*)buffer + LWS_PRE, n, LWS_WRITE_TEXT) < n)
//         {
//           printf("Error: lws_write failed (running status)\n");
//         }
//         else
//         {
//           printf("WebSocket running status sent successfully\n");
//         }
// 
//         char intermediate_result[BUFFER_SIZE] = "Intermediate result...";
//         printf("Intermediate result: %s\n", intermediate_result);
// 
//         n = snprintf(buffer + LWS_PRE, sizeof(buffer) - LWS_PRE,
//             "{\"type\":\"execution_status\",\"id\":\"%s\",\"status\":\"running\",\"result\":\"%s\"}", cell_id, intermediate_result);
// #include "syslib/console_print.h"
//         console_print((const char*) intermediate_result);
//         if (lws_write(wsi, (unsigned char*)buffer + LWS_PRE, n, LWS_WRITE_TEXT) < n)
//         {
//           printf("Error: lws_write failed (intermediate result)\n");
//         }
//         else
//         {
//           printf("WebSocket intermediate result sent successfully\n");
//         }
// 
//         char final_result[BUFFER_SIZE] = {0};
//         execute_orora_code(code, final_result, BUFFER_SIZE, root_envs);
//         printf("Final execution result: %s\n", final_result);
// 
//         n = snprintf(buffer + LWS_PRE, sizeof(buffer) - LWS_PRE,
//             "{\"type\":\"execution_status\",\"id\":\"%s\",\"status\":\"completed\",\"result\":\"%s\"}", cell_id, final_result);
//         if (lws_write(wsi, (unsigned char*)buffer + LWS_PRE, n, LWS_WRITE_TEXT) < n)
//         {
//           printf("Error: lws_write failed (completed status)\n");
//         }
//         else
//         {
//           printf("WebSocket completed status sent successfully\n");
//         }
//       }

        char final_result[BUFFER_SIZE] = {0};
        execute_orora_code(code, final_result, BUFFER_SIZE, root_envs);
//         printf("Final execution result: %s\n", final_result);

        n = snprintf(buffer + LWS_PRE, sizeof(buffer) - LWS_PRE,
            "{\"type\":\"execution_status\",\"id\":\"%s\",\"status\":\"completed\",\"result\":\"%s\"}", cell_id, "");
        if (lws_write(wsi, (unsigned char*)buffer + LWS_PRE, n, LWS_WRITE_TEXT) < n)
        {
          printf("Error: lws_write failed (completed status)\n");
        }
        else
        {
          printf("WebSocket completed status sent successfully\n");
        }
      }

      json_object_put(jobj);
      cell_id_global = (void*) 0;
      break;

    case LWS_CALLBACK_CLOSED:
      printf("WebSocket connection closed\n");
      break;

    default:
      break;
  }

  return 0;
}

// Modify the protocols array
// static struct lws_protocols protocols[] = {
//     {
//         "http-only",
//         callback_http,
//         0,
//         0,
//     },
//     {
//         "orora-protocol",
//         callback_orora,
//         sizeof(struct per_session_data__orora),
//         0,
//     },
//     { (void*) 0, (void*) 0, 0, 0 } // terminator
// };
static struct lws_protocols protocols[] = {
  {
    "orora-protocol",
    callback_orora,
    sizeof(struct per_session_data__orora),
    MAX_PAYLOAD_SIZE,
  },
  { (void*) 0, (void*) 0, 0, 0 }
};

void run_daemon()
{
  int socket_state = create_and_bind_socket();
  if (socket_state == -1)
  {
    return;
  }

  global_env = init_envs((void*) 0, init_env());
  root_envs = init_envs(global_env, init_env());
  rum_init_or(root_envs);

  pthread_t internal_thread;
  if (pthread_create(&internal_thread, (void*) 0, internal_client_thread, (void*) 0) != 0)
  {
    syslog(LOG_ERR, "Failed to create internal client thread");
    return;
  }
  //     pthread_t client_thread;
  //     if (pthread_create(&client_thread, (void*) 0, run_client, (void*) 0) != 0)
  //     {
  //         syslog(LOG_ERR, "Failed to create internal client thread");
  //         return;
  //     }

  while (1)
  {
    lws_service(context, 50);
  }

  pthread_join(internal_thread, (void*) 0);
  lws_context_destroy(context);
  free(global_env);
  free(root_envs);
  syslog(LOG_NOTICE, "Orora daemon exiting");
}

int create_and_bind_socket()
{
  for (int i = 0; i < MAX_RETRY; i++)
  {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    info.port = port;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.options = LWS_SERVER_OPTION_VALIDATE_UTF8 | LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

    context = lws_create_context(&info);
    if (!context)
    {
      port ++;
      continue;
    }
    return 0;
  }
  printf("Failed to open port");
  return -1;
}
