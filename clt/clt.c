#include "clt.h"
#include "clt-assert.h"
#include "clt-runner.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

enum clt_result {
  OK,
  FAIL,
  IGNORE,
};

#define DefListNode(name, T)                                                   \
  struct name {                                                                \
    struct name *next;                                                         \
    T;                                                                         \
  }

DefListNode(clt_msg_node, char *msg);
DefListNode(
    clt_err_node, struct {
      const struct clt_test_info *test_info;
      const struct clt_module_info *module_info;
      struct clt_msg_node *msg_head;
      struct clt_msg_node *msg_tail;
    });

/*
 * Global state for test runner
 */
static struct {
  size_t n_tests;
  size_t n_pass;
  size_t n_fail;
  size_t n_ignore;

  enum clt_result last_result;

  const struct clt_test_info *current_test;
  const struct clt_module_info *current_module;

  struct clt_err_node *err_head;
  struct clt_err_node *err_tail;
} clt_g_data CLT_DATA_SECTION;

CLT_TEXT_SECTION void clt_free_msg_list(struct clt_msg_node *head) {
  struct clt_msg_node *next = NULL;
  while (head != NULL) {
    next = head->next;

    free(head->msg);
    head->next = NULL;
    head->msg = NULL;

    free(head);

    head = next;
  }
}

CLT_TEXT_SECTION void clt_free_err_list(struct clt_err_node *head) {
  struct clt_err_node *next = NULL;
  while (head != NULL) {
    next = head->next;

    head->next = NULL;
    head->test_info = NULL;
    clt_free_msg_list(head->msg_head);
    free(head);

    head = next;
  }
}

/*
 * Initialize Global state to begin running tests
 */
CLT_TEXT_SECTION void clt_begin() {
  clt_g_data.n_pass = 0;
  clt_g_data.n_fail = 0;
  clt_g_data.n_ignore = 0;

  clt_g_data.last_result = OK;
  clt_g_data.current_test = NULL;
  clt_g_data.current_module = NULL;

  clt_free_err_list(clt_g_data.err_head);
  clt_g_data.err_head = NULL;
  clt_g_data.err_tail = NULL;
}

CLT_TEXT_SECTION void clt_print_err_list(struct clt_err_node *head) {
  while (head != NULL) {
    printf("\nfailures:\n\n");
    printf("---- %s::%s ----\n", head->module_info->name,
           head->test_info->name);
    printf(ANSI_BOLD "File:" ANSI_CLEAR_COLOR " %s\n", head->test_info->file);
    printf(ANSI_BOLD "Description:" ANSI_CLEAR_COLOR " %s\n",
           head->test_info->description);
    printf(ANSI_BOLD "Should Fail:" ANSI_CLEAR_COLOR " %s\n",
           (head->test_info->flags & CLT_SHOULD_FAIL) == 0 ? "false" : "true");
    printf(ANSI_BOLD "Should Ignore:" ANSI_CLEAR_COLOR " %s\n",
           (head->test_info->flags & CLT_SHOULD_IGNORE) == 0 ? "false"
                                                             : "true");
    putchar('\n');

    struct clt_msg_node *msg = head->msg_head;
    while (msg != NULL) {
      printf("%s\n", msg->msg);
      msg = msg->next;
    }

    head = head->next;
  }
}

/*
 * Print Test results and run cleanup routines
 *
 * @return 0 if success, 1 if fail
 */
CLT_TEXT_SECTION int clt_end() {
  clt_print_err_list(clt_g_data.err_head);

  printf("\n----------------\n"
         "%ld Tests "
         "%ld Failures "
         "%ld Ignored\n",
         clt_g_data.n_tests, clt_g_data.n_fail, clt_g_data.n_ignore);

  printf("%s\n\n", clt_g_data.n_fail == 0 ? "OK" : "FAIL");

  clt_free_err_list(clt_g_data.err_head);
  clt_g_data.err_head = NULL;
  clt_g_data.err_tail = NULL;

  return clt_g_data.n_fail != 0;
}

/*
 * Run Test from test_info structure
 */
CLT_TEXT_SECTION void clt_run_test(const struct clt_test_info *info) {
  clt_g_data.n_tests++;
  clt_g_data.current_test = info;

  if (info->flags & CLT_SHOULD_IGNORE) {
    clt_g_data.last_result = IGNORE;
  } else {
    clt_g_data.last_result = (info->flags & CLT_SHOULD_FAIL) ? FAIL : OK;
    info->cb();
  }

  const char *status = NULL;
  switch (clt_g_data.last_result) {
  case FAIL:
    status = ANSI_RED_FG "FAILED" ANSI_CLEAR_COLOR;
    clt_g_data.n_fail++;
    break;
  case IGNORE:
    status = ANSI_YELLOW_FG "ignored" ANSI_CLEAR_COLOR;
    clt_g_data.n_ignore++;
    break;
  case OK:
    status = ANSI_GREEN_FG "ok" ANSI_CLEAR_COLOR;
    clt_g_data.n_pass++;
    break;
  }

  if (info->flags & CLT_SHOULD_FAIL && clt_g_data.last_result == FAIL) {
    CLT_LOG_FAIL("Expected an assertion to fail");
  }

  printf("test %s::%s ... %s\n", clt_g_data.current_module->name, info->name,
         status);
}

CLT_TEXT_SECTION bool clt_is_null_test(const struct clt_test_info *test) {
  if (test == NULL)
    return true;
  const struct clt_test_info null_test = CLT_NULL_TEST_INFO;
  return memcmp(&null_test, test, sizeof(null_test)) == 0;
}

/*
 * Run All tests within a given module
 */
CLT_TEXT_SECTION void
clt_internal_run_module(const struct clt_module_info *info) {
  if (info == NULL)
    return; // TODO proper handling and message

  // Don't need a NULL pointer check as its never null
  if (clt_is_null_test(info->tests))
    return;

  clt_g_data.current_module = info;

  for (const struct clt_test_info *test = info->tests;
       test != NULL && !clt_is_null_test(test); test++) {
    clt_run_test(test);
  }
}

/*
 * Set global failure state
 */
CLT_TEXT_SECTION int clt_fail() {
  if (clt_g_data.current_test == NULL)
    return 1;
  clt_g_data.last_result =
      clt_g_data.current_test->flags & CLT_SHOULD_FAIL ? OK : FAIL;

  return clt_g_data.current_test->flags & CLT_SHOULD_FAIL ? 0 : 1;
}

/*
 * Validate that our error list node is the current modules
 *
 * Returns 0 on success, 1 on error
 */
CLT_TEXT_SECTION int clt_validate_err_list() {
  if (clt_g_data.err_tail == NULL ||
      clt_g_data.err_tail->test_info != clt_g_data.current_test ||
      clt_g_data.err_tail->module_info != clt_g_data.current_module) {

    struct clt_err_node *next = calloc(1, sizeof(*next));
    if (next == NULL)
      return 1;

    next->test_info = clt_g_data.current_test;
    next->module_info = clt_g_data.current_module;

    if (clt_g_data.err_tail != NULL) {
      clt_g_data.err_tail->next = next;
    }
    if (clt_g_data.err_head == NULL) {
      clt_g_data.err_head = next;
    }

    clt_g_data.err_tail = next;
  }

  return 0;
}

CLT_TEXT_SECTION void clt_push_error_msg(char *msg) {
  if (clt_validate_err_list())
    return;

  struct clt_msg_node **tail = &clt_g_data.err_tail->msg_tail,
                      **head = &clt_g_data.err_tail->msg_head;

  struct clt_msg_node *next = calloc(1, sizeof(*next));
  if (next == NULL) {
    free(msg);
    return;
  }

  next->msg = msg;

  if (*tail != NULL)
    (*tail)->next = next;

  *tail = next;

  if (*head == NULL)
    *head = *tail;
}
