const char* console_print(const char* text)
{
  fprintf(stderr, "%s", text);
  return text;
}
