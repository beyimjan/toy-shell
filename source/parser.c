#include "parser.h"
#include <stdlib.h>

struct parser_t create_parser()
{
  struct parser_t parser;
  parser.buflen = 32;
  parser.word_length = 0;
  parser.buf = malloc(parser.buflen);
  parser.double_quote_expected = 0;
  parser.res = pr_ignore;
  return parser;
}

void stop_parser(struct parser_t *parser)
{
  parser->buf[parser->word_length] = '\0';
  if (parser->double_quote_expected)
    parser->res = pr_error_missing_closing_quote;
  else if (parser->res == pr_ignore && parser->word_length == 0)
    parser->res = pr_ignore_parsed;
  else
    parser->res = pr_parsed;
}

void parse_char(struct parser_t *parser, char c)
{
  if (c == '"') {
    parser->double_quote_expected = !parser->double_quote_expected;
    parser->res = pr_in_progress;
    return;
  }
  else if ((c == ' ' || c == '\t') && !parser->double_quote_expected) {
    stop_parser(parser);
    return;
  }

  parser->res = pr_in_progress;

  if (parser->word_length + 1 == parser->buflen) {
    parser->buflen *= 2;
    parser->buf = realloc(parser->buf, parser->buflen);
  }

  parser->buf[parser->word_length] = c;
  parser->word_length++;
}
