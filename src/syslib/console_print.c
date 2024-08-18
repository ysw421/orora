#include "syslib/console_print.h"
#include "server/daemon.h"

void console_print(const char* text)
{
  fprintf(stderr, "%s", text);

  if (INTERACTIVE_MODE)
  {
    extern struct lws* wsi_global;
    extern const char* cell_id_global;

    if (wsi_global && cell_id_global)
    {
      char buffer[MAX_PAYLOAD_SIZE + LWS_PRE];
      int n = snprintf(buffer + LWS_PRE, sizeof(buffer) - LWS_PRE,
                "{\"type\":\"execution_status\",\"id\":\"%s\",\"status\":\"running\",\"result\":\"%s\"}", cell_id_global, text);
      
      lws_write(wsi_global, (unsigned char*) buffer + LWS_PRE, n, LWS_WRITE_TEXT);
    }
  }
}
