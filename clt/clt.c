#include "clt.h"
#include "clt-assert.h"
#include "clt-runner.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  PASS,
  FAIL,
  IGNORE,
} clt_test_status_t;

/*
 * Linked List for clt test error messages
 */
typedef struct clt_test_error_msg_s {
  struct clt_test_error_msg_s *next;
  char *msg;
} clt_test_error_msg_t;

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
 * Global Linked List for clt test errors
 */
static struct clt_test_error_s {
  struct clt_test_error_s *next;
  const clt_test_info_t *test_info;

  clt_test_error_msg_t *messages;
} *clt_test_runner_errors CLT_DATA_SECTION = NULL;
static struct clt_test_error_s *clt_test_errors_tail CLT_DATA_SECTION = NULL;

CLT_TEXT_SECTION void clt_free_error_msg_list(clt_test_error_msg_t *list) {
  clt_test_error_msg_t *next = NULL;
  while (list != NULL) {
    next = list->next;

    free(list->msg);
    list->next = NULL;
    list->msg = NULL;

    free(list);

    list = next;
  }
}

CLT_TEXT_SECTION void clt_free_error_list(struct clt_test_error_s *list) {
  struct clt_test_error_s *next = NULL;
  while (list != NULL) {
    next = list->next;

    list->next = NULL;
    list->test_info = NULL;
    clt_free_error_msg_list(list->messages);
    free(list);

    list = next;
  }
}

/*
 * Initialize Global state to begin running tests
 */
CLT_TEXT_SECTION void clt_begin() {
  clt_test_runner_data.passed = 0;
  clt_test_runner_data.failures = 0;
  clt_test_runner_data.ignored = 0;
  clt_test_runner_data.last_test_status = PASS;
  clt_test_runner_data.current_test = NULL;

  clt_free_error_list(clt_test_runner_errors);
  clt_test_runner_errors = NULL;
  clt_test_errors_tail = NULL;
}

/*
 * Print Test results and run cleanup routines
 *
 * @return 0 if success, 1 if fail
 */
CLT_TEXT_SECTION int clt_end() {
  struct clt_test_error_s *head = clt_test_runner_errors;
  if (head != NULL) {
    printf("\n====== " ANSI_BOLD "ERROR LOG" ANSI_CLEAR_COLOR " ======\n");
  }

  while (head != NULL) {
    printf("\n====== %s ======\n", head->test_info->name);
    printf(ANSI_BOLD "File:" ANSI_CLEAR_COLOR " %s\n", head->test_info->file);
    printf(ANSI_BOLD "Description:" ANSI_CLEAR_COLOR " %s\n",
           head->test_info->description);
    printf(ANSI_BOLD "Should Fail:" ANSI_CLEAR_COLOR " %s\n",
           (head->test_info->flags & CLT_SHOULD_FAIL) == 0 ? "false" : "true");
    printf(ANSI_BOLD "Should Ignore:" ANSI_CLEAR_COLOR " %s\n",
           (head->test_info->flags & CLT_SHOULD_IGNORE) == 0 ? "false"
                                                             : "true");
    putchar('\n');

    clt_test_error_msg_t *msg = head->messages;
    while (msg != NULL) {
      printf("%s\n", msg->msg);
      msg = msg->next;
    }

    head = head->next;
  }

  printf("\n----------------\n"
         "%ld Tests "
         "%ld Failures "
         "%ld Ignored\n",
         clt_test_runner_data.num_of_tests, clt_test_runner_data.failures,
         clt_test_runner_data.ignored);

  printf("%s\n\n", clt_test_runner_data.failures == 0 ? "PASS" : "FAIL");

  clt_free_error_list(clt_test_runner_errors);
  clt_test_runner_errors = NULL;
  clt_test_errors_tail = NULL;

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
    status = ANSI_RED_FG "FAIL" ANSI_CLEAR_COLOR;
    clt_test_runner_data.failures++;
    break;
  case IGNORE:
    status = ANSI_YELLOW_FG "IGNORE" ANSI_CLEAR_COLOR;
    clt_test_runner_data.ignored++;
    break;
  case PASS:
    status = ANSI_GREEN_FG "PASS" ANSI_CLEAR_COLOR;
    clt_test_runner_data.passed++;
    break;
  }

  if (info->flags & CLT_SHOULD_FAIL &&
      clt_test_runner_data.last_test_status == FAIL) {
    printf(ANSI_BOLD "%s:%s:" ANSI_CLEAR_COLOR
                     " %s " ANSI_BOLD "(Expected Failure but Instead Passed)\n" ANSI_CLEAR_COLOR,
           info->file, info->name, status);
  } else {
    printf(ANSI_BOLD "%s:%s:" ANSI_CLEAR_COLOR " %s\n", info->file, info->name,
           status);
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

CLT_TEXT_SECTION void clt_push_error_msg(char *msg) {
  if (clt_test_errors_tail == NULL ||
      clt_test_errors_tail->test_info != clt_test_runner_data.current_test) {
    struct clt_test_error_s *prev = clt_test_errors_tail;

    clt_test_errors_tail = calloc(1, sizeof(*clt_test_errors_tail));
    clt_test_errors_tail->test_info = clt_test_runner_data.current_test;

    if (prev != NULL)
      prev->next = clt_test_errors_tail;
  }

  if (clt_test_runner_errors == NULL)
    clt_test_runner_errors = clt_test_errors_tail;

  clt_test_error_msg_t *tail = clt_test_errors_tail->messages,
                       *message = calloc(1, sizeof(*message));
  message->msg = msg;
  while (tail != NULL && tail->next != NULL) {
    tail = tail->next;
  }

  if (tail != NULL)
    tail->next = message;
  else
    clt_test_errors_tail->messages = message;
}
