#ifndef PARSER_H_SENTRY
#define PARSER_H_SENTRY

#include <stddef.h>
#include "words.h"

enum parsing_error_t {
  pe_none,
  pe_unfinished_escape_sequence,
  pe_invalid_char_escaped,
  pe_missing_closing_quote
};

enum parsing_result_t {
  pr_ignore,
  pr_in_progress,
  pr_parsed,
  pr_ignore_parsed,
  pr_error_occurred
};

struct parser_t {
  char *buf;
  size_t buflen, word_length;
  int ignore_word;
  int double_quote_expected;
  int escape_next_char;
  enum parsing_error_t err;
  enum parsing_result_t res;
};

struct parser_t create_parser();
void free_parser(struct parser_t *parser);
void stop_parser(struct parser_t *parser, struct words_t *words);
void parse_char(struct parser_t *parser, struct words_t *words, char c);

#endif
