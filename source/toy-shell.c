#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "debug.h"
#include "parser.h"
#include "words.h"

enum read_word_res { rwr_ok, rwr_exec, rwr_parsing_error };

char *read_word(enum read_word_res *res, int *quit)
{
  struct parser_t parser = create_parser();

  *res = rwr_ok;

  while (parser.res == pr_ignore || parser.res == pr_in_progress) {
    char c = getchar();
#if DEBUG_LEVEL >= 3
    DEBUG_TRACE_CVAR(c);
#endif

    if (c == EOF) {
#if DEBUG_LEVEL >= 2
      DEBUG_TRACE("Parsing finished because of EOF.");
#endif
      *res = rwr_exec;
      *quit = 1;
      stop_parser(&parser);
      break;
    }
    else if (c == '\n') {
#if DEBUG_LEVEL >= 2
      DEBUG_TRACE("Parsing finished because of newline.");
#endif
      *res = rwr_exec;
      stop_parser(&parser);
      break;
    }
    else {
#if DEBUG_LEVEL >= 3
      DEBUG_TRACE("Parsing in progress.");
#endif
      parse_char(&parser, c);
    }
  }

  if (parser.res == pr_error_unfinished_escape_sequence) {
    *res = rwr_parsing_error;
    fputs("Error: unfinished escape sequence.\n", stderr);
  }
  else if (parser.res == pr_error_missing_closing_quote) {
    *res = rwr_parsing_error;
    fputs("Error: missing closing quote.\n", stderr);
  }
  else if (parser.res == pr_error_invalid_char_escaped) {
    *res = rwr_parsing_error;
    fputs("Error: invalid character escaped.\n", stderr);
  }

#if DEBUG_LEVEL >= 2
  DEBUG_TRACE_DVAR(parser.res);
#endif

  if (parser.res == pr_parsed) {
    return parser.buf;
  }
  else {
    free(parser.buf);
    return NULL;
  }
}

void invoke_command(char *const *argv)
{
  int pid;

  pid = fork();
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
}

int main()
{
  int quit = 0;

  while (!quit) {
    struct words_t words = init_words();
    fputs("toy-shell> ", stdout);

    for (;;) {
      enum read_word_res rw_res;
      char *word;

      word = read_word(&rw_res, &quit);

      if (rw_res == rwr_parsing_error) {
        goto cleanup;
      }
      else if (word != NULL) {
#if DEBUG_LEVEL >= 2
        DEBUG_TRACE_SVAR(word);
#endif
        add_word(&words, word);
      }

      if (rw_res == rwr_exec)
        break;
    }

    invoke_command(words.buf);
  cleanup:
    free_words(&words);
  }

  return 0;
}
