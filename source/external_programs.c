#include "external_programs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void set_foreground_process_group(pid_t pgid)
{
  signal(SIGTTOU, SIG_IGN);
  tcsetpgrp(STDIN_FILENO, pgid);
  signal(SIGTTOU, SIG_DFL);
}

void run_external_program(char *const *argv)
{
#ifndef DEBUG_SKIP_RUNNING_EXTERNAL_PROGRAMS
  int pid = fork();
  if (pid == -1) {
    perror("fork");
    return;
  }

  setpgid(pid, pid);

  if (pid == 0) /* child process */
  {
    set_foreground_process_group(getpgid(0));
    execvp(argv[0], argv);
    if (!argv[0][0])
      fputs("Error: ", stderr);
    perror(argv[0]);
    exit(1);
  }

  /* parent process */
  set_foreground_process_group(pid);
  wait(NULL);
  set_foreground_process_group(getpgid(0));
#endif
}
