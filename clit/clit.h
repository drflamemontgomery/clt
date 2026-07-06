#ifndef CLIT_H
#define CLIT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define UNUSED __attribute__((unused))

typedef void (*clit_test_cb)(void);
typedef struct {
  clit_test_cb cb;
  const char *name;
  const char *description;
  const char *file;

  int flags;
} clit_test_info_t;

#define CLIT_SHOULD_FAIL 1
#define CLIT_SHOULD_IGNORE 2

#if CLIT_TEST_ENABLE

#define CLIT_TEST(NAME) void __clit_test_##NAME()

#define CLIT_INCLUDE_MODULE(MODULE)                                            \
  extern clit_test_info_t __clit_module_##MODULE[];                            \
  extern size_t __clit_module_##MODULE##_len

#define CLIT_MODULE(MODULE, ...)                                               \
  clit_test_info_t __clit_module_##MODULE[] = {__VA_ARGS__};                   \
  size_t __clit_module_##MODULE##_len =                                        \
      sizeof(__clit_module_##MODULE) / sizeof(*__clit_module_##MODULE)

#define CLIT_REGISTER(NAME, DESCRIPTION, ...)                                  \
  (clit_test_info_t) {                                                         \
    .cb = __clit_test_##NAME, .name = #NAME, .description = DESCRIPTION,       \
    .file = __FILE__, .flags = 0 __VA_OPT__(|) __VA_ARGS__,                    \
  }

#else

#define CLIT_TEST(NAME) void __clit_test_##NAME() UNUSED
#define CLIT_INCLUDE_MODULE(MODULE)
#define CLIT_MODULE(MODULE, ...)
#define CLIT_REGISTER(NAME, DESCRIPTION, SHOULD_FAIL, IGNORE)

#endif

#endif
