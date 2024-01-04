#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "core.h"
#include "debug.h"
#include "parser.h"
#include "words.h"

enum read_word_res { rwr_ok, rwr_exec, rwr_parsing_error };

static char *read_word(enum read_word_res *res, int *quit)
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

static void print_prompt()
{
  char *cwd = getcwd(NULL, 0);
  printf("%s> ", cwd);
  free(cwd);
}

int main()
{
  int quit = 0;

  while (!quit) {
    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
      print_prompt();

    struct words_t words = init_words();
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

    if (words.word_count > 0)
      invoke_command(words.word_count, words.buf);
#if DEBUG_LEVEL >= 2
    else
      DEBUG_TRACE("No command to invoke.");
#endif
  cleanup:
    free_words(&words);
  }

  return 0;
}
