#ifndef CLT_H
#define CLT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__)
#define UNUSED __attribute__((unused))
#define CLT_TEXT_SECTION __attribute((section (".clt_text")))
#define CLT_MODULE_SECTION __attribute((section (".clt_module_rodata")))
#define CLT_DATA_SECTION __attribute((section (".clt_data")))

#elif defined(_MSC_VER) && (_MSC_VER > 1911)
#pragma section(".clt_text", read)
#pragma section(".clt_data", read)
#pragma section(".clt_module_rodata", read)

#define UNUSED [[maybe_unused]]
#define CLT_TEXT_SECTION __declspec((section (".clt_text")))
#define CLT_MODULE_SECTION __declspec((section (".clt_module_rodata")))
#define CLT_DATA_SECTION __declspec((section (".clt_data")))

#else
#pragma section(".clt_text", read)
#pragma section(".clt_data", read)
#pragma section(".clt_module_rodata", read)

#define UNUSED
#define CLT_TEXT_SECTION __declspec((section (".clt_text")))
#define CLT_MODULE_SECTION __declspec((section (".clt_module_rodata")))
#define CLT_DATA_SECTION __declspec((section (".clt_data")))

#endif

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

#define CLT_DATA CLT_DATA_SECTION
#define CLT_HELPER CLT_TEXT_SECTION
#define CLT_TEST(NAME) CLT_TEXT_SECTION void clt_test_##NAME ()

#define CLT_INCLUDE_MODULE(MODULE)                                            \
  extern const clt_test_info_t clt_module_##MODULE[] CLT_MODULE_SECTION;                            \
  extern const size_t clt_module_##MODULE##_len CLT_MODULE_SECTION

#define CLT_MODULE(MODULE, ...)                                               \
  const clt_test_info_t clt_module_##MODULE[] CLT_MODULE_SECTION = {__VA_ARGS__};                   \
  const size_t clt_module_##MODULE##_len CLT_MODULE_SECTION =                                        \
      sizeof(clt_module_##MODULE) / sizeof(*clt_module_##MODULE)

#define CLT_REGISTER(NAME, DESCRIPTION, ...)                                  \
  (clt_test_info_t) {                                                         \
    .cb = clt_test_##NAME, .name = #NAME, .description = DESCRIPTION,       \
    .file = __FILE__, .flags = 0 __VA_OPT__(|) __VA_ARGS__,                    \
  }

#endif
