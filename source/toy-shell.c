#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "core.h"
#include "parser.h"
#include "words.h"

static const char *get_error_message(enum parsing_error_t err)
{
  switch (err) {
    case pe_unfinished_escape_sequence:
      return "Error: unfinished escape sequence.\n";
    case pe_missing_closing_quote:
      return "Error: missing closing quote.\n";
    case pe_invalid_char_escaped:
      return "Error: invalid character escaped.\n";
    case pe_none:
      return "NO ERROR.\n";
    default:
      return "UNKNOWN ERROR.\n";
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
  for (;;) {
    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
      print_prompt();

    struct words_t words = init_words();
    struct parser_t parser = create_parser();
    int c;

    while (parser.res == pr_ignore || parser.res == pr_in_progress) {
      c = getchar();
      parse_char(&parser, &words, c);
    }

    /* skip the rest of the line */
    if (parser.err == pe_invalid_char_escaped) {
      for (;;) {
        c = getchar();
        if (c == EOF || c == '\n')
          break;
      }
    }

    if (parser.res == pr_parsed)
      invoke_command(words.word_count, words.buf);
    else if (parser.res == pr_error_occurred)
      fputs(get_error_message(parser.err), stderr);

    free_words(&words);
    free_parser(&parser);

    if (c == EOF)
      break;
  }

  return 0;
}
