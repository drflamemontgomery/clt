#include "clt-assert.h"
#include "clt-runner.h"
#include "clt.h"

CLT_INCLUDE_MODULE(addition);
CLT_INCLUDE_MODULE(ignore);
CLT_INCLUDE_MODULE(assertion);
CLT_INCLUDE_MODULE(external);

int main(void) {
  clt_begin();

  clt_run_module(addition);
  clt_run_module(ignore);
  clt_run_module(assertion);
  clt_run_module(external);

  return clt_end();
}

/*
 * Addition Tests
 */

static CLT_TEST(add) { clt_assert_equal(2, 1 + 1); }
static CLT_TEST(fail_add) { clt_assert_equal(5, 2 + 2); }

CLT_MODULE(addition, CLT_REGISTER(add, "Test adding of two numbers"),
           CLT_REGISTER(fail_add, "Test incorrect adding of two numbers",
                        CLT_SHOULD_FAIL));
/*
 * Ignore Tests
 */

static CLT_TEST(ignore_pass) { clt_assert(true); }
static CLT_TEST(ignore_fail) { clt_assert(false); }
static CLT_TEST(ignore_should_fail) { clt_assert(false); }

CLT_MODULE(ignore,
           CLT_REGISTER(ignore_pass, "Ignoring test that is expected to pass",
                        CLT_SHOULD_IGNORE),
           CLT_REGISTER(ignore_fail,
                        "Ignoring test that is expected to pass but fails",
                        CLT_SHOULD_IGNORE),
           CLT_REGISTER(ignore_should_fail,
                        "Ignoring test that is expected to fail",
                        CLT_SHOULD_IGNORE | CLT_SHOULD_FAIL));

/*
 * Assertion Tests
 */

static CLT_TEST(assert_pass) {
  clt_assert(true);
  clt_assert_equal(0, 0);
}

CLT_MODULE(assertion, CLT_REGISTER(assert_pass, "Test Assert Methods"));
