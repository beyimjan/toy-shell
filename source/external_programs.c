#include "external_programs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void run_external_program(char *const *argv)
{
#ifndef DEBUG_SKIP_RUNNING_EXTERNAL_PROGRAMS
  int pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) /* child process */
  {
    execvp(argv[0], argv);
    perror(argv[0]);
    exit(2);
  }
  /* parent process */
  wait(NULL);
#endif
}
