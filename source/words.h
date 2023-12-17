#ifndef WORDS_H_SENTRY
#define WORDS_H_SENTRY

struct word_t {
  char *word;
  struct word_t *next;
};

struct words_t {
  struct word_t *first, *last;
};

struct words_t init_words();
void add_word(struct words_t *words, char *word);
void free_words(struct words_t *words);
void print_words(struct words_t words);

#endif
