#ifndef CLT_H
#define CLT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "clt-internal.h"

typedef void (*clt_test_cb)(void);
struct clt_test_info {
  clt_test_cb cb;
  const char *name;
  const char *description;
  const char *file;

  int flags;
};

#define CLT_NULL_TEST_INFO ((struct clt_test_info){NULL})

struct clt_module_info {
  const char *name;
  struct clt_test_info tests[];
};

#define CLT_SHOULD_FAIL 1
#define CLT_SHOULD_IGNORE 2

#define CLT_DATA CLT_DATA_SECTION
#define CLT_HELPER CLT_TEXT_SECTION
#define CLT_TEST(NAME) CLT_TEXT_SECTION void clt_test_##NAME()

#define CLT_INCLUDE_MODULE(MODULE)                                             \
  extern const struct clt_module_info clt_module_##MODULE CLT_MODULE_SECTION;

#define CLT_MODULE(MODULE, ...)                                                \
  const struct clt_module_info clt_module_##MODULE CLT_MODULE_SECTION = {      \
      .name = #MODULE,                                                         \
      .tests = {__VA_ARGS__ __VA_OPT__(, ) CLT_NULL_TEST_INFO}};

#define CLT_REGISTER(NAME, DESCRIPTION, ...)                                   \
  (struct clt_test_info) {                                                     \
    .cb = clt_test_##NAME, .name = #NAME, .description = DESCRIPTION,          \
    .file = __FILE__, .flags = 0 __VA_OPT__(|) __VA_ARGS__,                    \
  }

#endif
