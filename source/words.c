#include "words.h"
#include <stdlib.h>
#include "debug.h"

struct words_t init_words()
{
  struct words_t words;
  words.buf_size = 8;
  words.buf = malloc(words.buf_size * sizeof(*words.buf));
  words.word_count = 0;
  words.buf[words.word_count] = NULL;
  return words;
}

void add_word(struct words_t *words, char *word)
{
#if DEBUG_LEVEL >= 3
  DEBUG_TRACE_SVAR(word);
  DEBUG_TRACE_DVAR(words->word_count);
#endif

  if (words->buf_size == words->word_count + 1) {
    words->buf_size *= 2;
    words->buf = realloc(words->buf, words->buf_size * sizeof(*words->buf));
  }
  words->buf[words->word_count] = word;
  words->buf[words->word_count + 1] = NULL;
  words->word_count++;
}

void free_words(struct words_t *words)
{
  char **word = words->buf;
  while (*word) {
    free(*word);
    word++;
  }
  free(words->buf);
}
