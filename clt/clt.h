#ifndef CLT_H
#define CLT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define UNUSED __attribute__((unused))

typedef void (*clt_test_cb)(void);
typedef struct {
  clt_test_cb cb;
  const char *name;
  const char *description;
  const char *file;

  int flags;
} clt_test_info_t;

#define CLT_SHOULD_FAIL 1
#define CLT_SHOULD_IGNORE 2

#if CLT_TEST_ENABLE

#define CLT_HELPER
#define CLT_TEST(NAME) void __clt_test_##NAME()

#define CLT_INCLUDE_MODULE(MODULE)                                            \
  extern clt_test_info_t __clt_module_##MODULE[];                            \
  extern size_t __clt_module_##MODULE##_len

#define CLT_MODULE(MODULE, ...)                                               \
  clt_test_info_t __clt_module_##MODULE[] = {__VA_ARGS__};                   \
  size_t __clt_module_##MODULE##_len =                                        \
      sizeof(__clt_module_##MODULE) / sizeof(*__clt_module_##MODULE)

#define CLT_REGISTER(NAME, DESCRIPTION, ...)                                  \
  (clt_test_info_t) {                                                         \
    .cb = __clt_test_##NAME, .name = #NAME, .description = DESCRIPTION,       \
    .file = __FILE__, .flags = 0 __VA_OPT__(|) __VA_ARGS__,                    \
  }

#else

#define CLT_HELPER UNUSED
#define CLT_TEST(NAME) UNUSED void __clt_test_##NAME()
#define CLT_INCLUDE_MODULE(MODULE)
#define CLT_MODULE(MODULE, ...)
#define CLT_REGISTER(NAME, DESCRIPTION, ...)

#endif

#endif
