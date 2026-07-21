#include "clt.h"
#include "clt-assert.h"
#include "clt-runner.h"

#include <memory.h>
#include <stdio.h>

typedef enum {
  PASS,
  FAIL,
  IGNORE,
} clt_test_status_t;

/*
 * Global state for test runner
 */
static struct {
  size_t num_of_tests;
  size_t passed;
  size_t failures;
  size_t ignored;

  clt_test_status_t last_test_status;
  const clt_test_info_t *current_test;
} clt_test_runner_data CLT_DATA_SECTION;

/*
 * Initialize Global state to begin running tests
 */
CLT_TEXT_SECTION void clt_begin() {
  clt_test_runner_data.passed = 0;
  clt_test_runner_data.failures = 0;
  clt_test_runner_data.ignored = 0;
  clt_test_runner_data.last_test_status = PASS;
  clt_test_runner_data.current_test = NULL;
}

/*
 * Print Test results and run cleanup routines
 *
 * @return 0 if success, 1 if fail
 */
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

/*
 * Run Test from test_info structure
 */
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

CLT_TEXT_SECTION bool clt_is_null_test(const clt_test_info_t *test) {
  if (test == NULL)
    return true;
  const clt_test_info_t null_test = CLT_NULL_TEST_INFO;
  return memcmp(&null_test, test, sizeof(null_test)) == 0;
}

/*
 * Run All tests within a given module
 */
CLT_TEXT_SECTION void clt_internal_run_module(const clt_module_info_t *info) {
  if (info == NULL)
    return; // TODO proper handling and message

  printf("\n====== %s ======\n", info->name);
  for (const clt_test_info_t *test = info->tests;
       test != NULL && !clt_is_null_test(test); test++) {
    clt_run_test(test);
  }
}

/*
 * Set global failure state
 */
CLT_TEXT_SECTION int clt_fail() {
  if (clt_test_runner_data.current_test == NULL)
    return 1;
  clt_test_runner_data.last_test_status =
      clt_test_runner_data.current_test->flags & CLT_SHOULD_FAIL ? PASS : FAIL;

  return clt_test_runner_data.current_test->flags & CLT_SHOULD_FAIL ? 0 : 1;
}
