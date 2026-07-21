#ifndef CLT_RUNNER_H
#define CLT_RUNNER_H

#include "clt.h"

#define clt_run_module(MODULE) clt_internal_run_module(&(clt_module_##MODULE))

CLT_TEXT_SECTION void clt_internal_run_module(const clt_module_info_t *info);
CLT_TEXT_SECTION void clt_run_test(const clt_test_info_t *info);
CLT_TEXT_SECTION void clt_begin();
CLT_TEXT_SECTION int clt_end();

#endif
