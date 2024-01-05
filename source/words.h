#ifndef WORDS_H_SENTRY
#define WORDS_H_SENTRY

struct words_t {
  char **buf; /* designed especially to be passed as an argument to execvp */
  int buf_size;
  int word_count;
};

struct words_t init_words();
void add_word(struct words_t *words, char *word);

/* You should not use `words` after calling `free_words` */
void free_words(struct words_t *words);

#endif
