#ifndef PARSER_H_SENTRY
#define PARSER_H_SENTRY

#include <stddef.h>

enum parsing_result_t {
  pr_ignore,
  pr_in_progress,
  pr_parsed,
  pr_ignore_parsed,
  pr_error_unfinished_escape_sequence,
  pr_error_invalid_char_escaped,
  pr_error_missing_closing_quote
};

struct parser_t {
  char *buf;
  size_t buflen, word_length;
  int double_quote_expected;
  int escape_next_char;
  enum parsing_result_t res;
};

struct parser_t create_parser();
void stop_parser(struct parser_t *parser);
void parse_char(struct parser_t *parser, char c);

#endif
