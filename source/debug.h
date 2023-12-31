#ifndef DEBUG_H_SENTRY
#define DEBUG_H_SENTRY

#include <stdio.h>

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

#define DEBUG_TRACE(...)                                  \
  do {                                                    \
    fprintf(stderr, "DEBUG: %s:%d ", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                         \
    fprintf(stderr, "\n");                                \
  } while (0)

#define DEBUG_TRACE_VAR(x, format_specifier)                                \
  do {                                                                      \
    fprintf(stderr, "DEBUG: %s:%d %s == %" format_specifier "\n", __FILE__, \
            __LINE__, #x, x);                                               \
  } while (0)

#define DEBUG_TRACE_DVAR(x) DEBUG_TRACE_VAR(x, "d")
#define DEBUG_TRACE_CVAR(x) DEBUG_TRACE_VAR(x, "c")
#define DEBUG_TRACE_SVAR(x) DEBUG_TRACE_VAR(x, "s")

#endif
