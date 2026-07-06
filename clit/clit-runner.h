#ifndef CLIT_RUNNER_H
#define CLIT_RUNNER_H

#include "clit.h"

#define CLIT_RUN_MODULE(MODULE)                                                \
  printf("\n====== " #MODULE " ======\n");                                     \
  for (size_t i = 0; i < __clit_module_##MODULE##_len; i++) {                  \
    CLIT_RUN_TEST(&__clit_module_##MODULE[i]);                                 \
  }
#define CLIT_RUN_TEST(TEST) __clit_run_test(TEST);

#define CLIT_ENTRY() __clit_setup()
#define CLIT_END() __clit_end()

void __clit_run_test(clit_test_info_t *info);
void __clit_setup();
int __clit_end();

#endif
