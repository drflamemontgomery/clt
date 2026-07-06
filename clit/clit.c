#include "clit.h"
#include "clit-runner.h"

#include <stdio.h>

typedef enum {
  PASS,
  FAIL,
  IGNORE,
} clit_test_status_t;

static struct {
  size_t num_of_tests;
  size_t passed;
  size_t failures;
  size_t ignored;

  clit_test_status_t last_test_status;
  clit_test_info_t *current_test;
} clit_test_runner_data;

void __clit_setup() {
  clit_test_runner_data.passed = 0;
  clit_test_runner_data.failures = 0;
  clit_test_runner_data.ignored = 0;
  clit_test_runner_data.last_test_status = PASS;
  clit_test_runner_data.current_test = NULL;
}

int __clit_end() {
  printf("\n----------------\n"
         "%ld Tests "
         "%ld Failures "
         "%ld Ignored\n",
         clit_test_runner_data.num_of_tests, clit_test_runner_data.failures,
         clit_test_runner_data.ignored);

  printf("%s\n\n", clit_test_runner_data.failures == 0 ? "PASS" : "FAIL");

  return clit_test_runner_data.failures != 0;
}

void __clit_run_test(clit_test_info_t *info) {
  clit_test_runner_data.num_of_tests++;
  clit_test_runner_data.current_test = info;

  if (info->flags & CLIT_SHOULD_IGNORE) {
    clit_test_runner_data.last_test_status = IGNORE;
  } else {
    clit_test_runner_data.last_test_status =
        (info->flags & CLIT_SHOULD_FAIL) ? FAIL : PASS;
    info->cb();
  }

  const char *status = NULL;
  switch (clit_test_runner_data.last_test_status) {
  case FAIL:
    status = "FAIL";
    clit_test_runner_data.failures++;
    break;
  case IGNORE:
    status = "IGNORE";
    clit_test_runner_data.ignored++;
    break;
  case PASS:
    status = "PASS";
    clit_test_runner_data.passed++;
    break;
  }

  if (info->flags & CLIT_SHOULD_FAIL &&
      clit_test_runner_data.last_test_status == FAIL) {
    printf("%s:%s: %s (Expected Failure but Instead Passed)\n", info->file,
           info->name, status);
  } else {
    printf("%s:%s: %s\n", info->file, info->name, status);
  }
}

int __clit_fail() {
  if (clit_test_runner_data.current_test == NULL)
    return 1;
  clit_test_runner_data.last_test_status =
      clit_test_runner_data.current_test->flags & CLIT_SHOULD_FAIL ? PASS
                                                                   : FAIL;

  return clit_test_runner_data.current_test->flags & CLIT_SHOULD_FAIL ? 0 : 1;
}
