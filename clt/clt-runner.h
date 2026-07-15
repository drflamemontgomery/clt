#ifndef CLT_RUNNER_H
#define CLT_RUNNER_H

#include "clt.h"

#define CLT_RUN_MODULE(MODULE)                                                \
  printf("\n====== " #MODULE " ======\n");                                     \
  for (size_t i = 0; i < clt_module_##MODULE##_len; i++) {                  \
    CLT_RUN_TEST(&clt_module_##MODULE[i]);                                 \
  }
#define CLT_RUN_TEST(TEST) clt_run_test(TEST);

#define CLT_ENTRY() clt_setup()
#define CLT_END() clt_end()

void clt_run_test(const clt_test_info_t *info);
void clt_setup();
int clt_end();

#endif
