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
    return;
  }

  /* ^C should kill the child process, not the shell */
  setpgid(pid, pid);
  tcsetpgrp(STDIN_FILENO, pid);

  if (pid == 0) /* child process */
  {
    execvp(argv[0], argv);
    perror(argv[0]);
    exit(2);
  }

  /* parent process */
  wait(NULL);

  /* Ignore SIGTTOU to prevent the process from being stopped */
  signal(SIGTTOU, SIG_IGN);
  /* Give the terminal back to the shell */
  tcsetpgrp(STDIN_FILENO, getpgid(getpid()));
  /* Restore the default signal handler */
  signal(SIGTTOU, SIG_DFL);
#endif
}
