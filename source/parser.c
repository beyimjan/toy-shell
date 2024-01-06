#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "words.h"

static void init_parser_buffer(struct parser_t *parser)
{
  parser->buflen = 32;
  parser->buf = malloc(parser->buflen);
  parser->buf[0] = '\0';
  parser->word_length = 0;
  parser->ignore_word = 1;
}

static void add_char_to_parser_buffer(struct parser_t *parser, char c)
{
  parser->ignore_word = 0;

  if (parser->word_length + 1 == parser->buflen) {
    parser->buflen *= 2;
    parser->buf = realloc(parser->buf, parser->buflen);
  }

  parser->buf[parser->word_length] = c;
  parser->word_length++;
  parser->buf[parser->word_length] = '\0';
}

static void add_word_from_parser_buffer(struct parser_t *parser,
                                        struct words_t *words)
{
  add_word(words, parser->buf);
  init_parser_buffer(parser);
}

struct parser_t create_parser()
{
  struct parser_t parser;
  init_parser_buffer(&parser);
  parser.double_quote_expected = 0;
  parser.escape_next_char = 0;
  parser.err = pe_none;
  parser.res = pr_ignore;
  return parser;
}

void free_parser(struct parser_t *parser)
{
  free(parser->buf);
}

static void log_parsing_error(struct parser_t *parser,
                              enum parsing_error_t err)
{
  parser->err = err;
  parser->res = pr_error_occurred;
}

void stop_parser(struct parser_t *parser, struct words_t *words)
{
  if (parser->escape_next_char) {
    log_parsing_error(parser, pe_unfinished_escape_sequence);
    return;
  }
  else if (parser->double_quote_expected) {
    log_parsing_error(parser, pe_missing_closing_quote);
    return;
  }
  else if (parser->res == pr_ignore) {
    parser->res = pr_ignore_parsed;
    return;
  }

  parser->res = pr_parsed;

  if (!parser->ignore_word)
    add_word_from_parser_buffer(parser, words);
}

static void escape_char(struct parser_t *parser, char c)
{
  switch (c) {
    case '\\':
    case '"':
    case ' ':
    case '\t': {
      parser->escape_next_char = 0;
      break;
    }
    default: {
      log_parsing_error(parser, pe_invalid_char_escaped);
    }
  }
}

void parse_char(struct parser_t *parser, struct words_t *words, char c)
{
  if (c == EOF || c == '\n') {
    stop_parser(parser, words);
    return;
  }
  else if (parser->escape_next_char) {
    escape_char(parser, c);
    if (parser->err == pe_invalid_char_escaped)
      return;
  }
  else if (c == '\\') {
    parser->escape_next_char = 1;
    parser->res = pr_in_progress;
    return;
  }
  else if (c == '"') {
    parser->ignore_word = 0;
    parser->double_quote_expected = !parser->double_quote_expected;
    parser->res = pr_in_progress;
    return;
  }
  else if ((c == ' ' || c == '\t') && !parser->double_quote_expected) {
    if (!parser->ignore_word)
      add_word_from_parser_buffer(parser, words);
    return;
  }

  parser->res = pr_in_progress;
  add_char_to_parser_buffer(parser, c);
}
