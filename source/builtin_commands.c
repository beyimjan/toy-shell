#include "builtin_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "debug.h"

static void invoke_cd(int argc, char *const *argv)
{
  char *path;
  if (argc == 1) {
    path = getenv("HOME");
    if (path == NULL) {
      fputs("cd: HOME not set\n", stderr);
      return;
    }
  }
  else if (argc == 2) {
    path = argv[1];
  }
  else {
    fputs("cd: too many arguments\n", stderr);
    return;
  }

  int res = chdir(path);
  if (res == -1) {
    perror("cd");
  }
}

int is_builtin_command(const char *cmd)
{
  return strcmp(cmd, "cd") == 0;
}

void invoke_builtin_command(int argc, char *const *argv)
{
  const char *cmd = argv[0];
  if (strcmp(cmd, "cd") == 0)
    invoke_cd(argc, argv);
#if DEBUG_LEVEL >= 1
  else
    DEBUG_TRACE("Error: unknown builtin command.");
#endif
}
