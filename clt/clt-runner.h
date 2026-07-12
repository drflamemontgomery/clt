#ifndef CLT_RUNNER_H
#define CLT_RUNNER_H

#include "clt.h"

#define CLT_RUN_MODULE(MODULE)                                                \
  printf("\n====== " #MODULE " ======\n");                                     \
  for (size_t i = 0; i < __clt_module_##MODULE##_len; i++) {                  \
    CLT_RUN_TEST(&__clt_module_##MODULE[i]);                                 \
  }
#define CLT_RUN_TEST(TEST) __clt_run_test(TEST);

#define CLT_ENTRY() __clt_setup()
#define CLT_END() __clt_end()

void __clt_run_test(clt_test_info_t *info);
void __clt_setup();
int __clt_end();

#endif
