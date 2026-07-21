#include "clt.h"
#include "clt-assert.h"
#include "clt-runner.h"

#include <stdio.h>

typedef enum {
  PASS,
  FAIL,
  IGNORE,
} clt_test_status_t;

static struct {
  size_t num_of_tests;
  size_t passed;
  size_t failures;
  size_t ignored;

  clt_test_status_t last_test_status;
  const clt_test_info_t *current_test;
} clt_test_runner_data CLT_DATA_SECTION;

CLT_TEXT_SECTION void clt_setup() {
  clt_test_runner_data.passed = 0;
  clt_test_runner_data.failures = 0;
  clt_test_runner_data.ignored = 0;
  clt_test_runner_data.last_test_status = PASS;
  clt_test_runner_data.current_test = NULL;
}

CLT_TEXT_SECTION int clt_end() {
  printf("\n----------------\n"
         "%ld Tests "
         "%ld Failures "
         "%ld Ignored\n",
         clt_test_runner_data.num_of_tests, clt_test_runner_data.failures,
         clt_test_runner_data.ignored);

  printf("%s\n\n", clt_test_runner_data.failures == 0 ? "PASS" : "FAIL");

  return clt_test_runner_data.failures != 0;
}

CLT_TEXT_SECTION void clt_run_test(const clt_test_info_t *info) {
  clt_test_runner_data.num_of_tests++;
  clt_test_runner_data.current_test = info;

  if (info->flags & CLT_SHOULD_IGNORE) {
    clt_test_runner_data.last_test_status = IGNORE;
  } else {
    clt_test_runner_data.last_test_status =
        (info->flags & CLT_SHOULD_FAIL) ? FAIL : PASS;
    info->cb();
  }

  const char *status = NULL;
  switch (clt_test_runner_data.last_test_status) {
  case FAIL:
    status = "FAIL";
    clt_test_runner_data.failures++;
    break;
  case IGNORE:
    status = "IGNORE";
    clt_test_runner_data.ignored++;
    break;
  case PASS:
    status = "PASS";
    clt_test_runner_data.passed++;
    break;
  }

  if (info->flags & CLT_SHOULD_FAIL &&
      clt_test_runner_data.last_test_status == FAIL) {
    printf("%s:%s: %s (Expected Failure but Instead Passed)\n", info->file,
           info->name, status);
  } else {
    printf("%s:%s: %s\n", info->file, info->name, status);
  }
}

CLT_TEXT_SECTION int clt_fail() {
  if (clt_test_runner_data.current_test == NULL)
    return 1;
  clt_test_runner_data.last_test_status =
      clt_test_runner_data.current_test->flags & CLT_SHOULD_FAIL ? PASS : FAIL;

  return clt_test_runner_data.current_test->flags & CLT_SHOULD_FAIL ? 0 : 1;
}
