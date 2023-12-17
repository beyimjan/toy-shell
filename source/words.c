#include "words.h"
#include <stdio.h>
#include <stdlib.h>

struct words_t init_words()
{
  struct words_t words;
  words.first = NULL;
  words.last = NULL;
  return words;
}

void add_word(struct words_t *words, char *word)
{
  struct word_t *tmp;
  tmp = malloc(sizeof(struct word_t));
  tmp->word = word;
  tmp->next = NULL;
  if (!words->first) {
    words->first = tmp;
    words->last = tmp;
  }
  else {
    words->last->next = tmp;
    words->last = tmp;
  }
}

void free_words(struct words_t *words)
{
  struct word_t *item = words->first;
  while (item) {
    struct word_t *tmp = item;
    item = item->next;
    free(tmp->word);
    free(tmp);
  }
  words->first = NULL;
  words->last = NULL;
}

void print_words(struct words_t words)
{
  for (struct word_t *tmp = words.first; tmp; tmp = tmp->next)
    printf("[%s]\n", tmp->word);
}
