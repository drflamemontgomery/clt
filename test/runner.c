#include "clt-assert.h"
#include "clt-runner.h"
#include "clt.h"

CLT_INCLUDE_MODULE(addition);
CLT_INCLUDE_MODULE(ignore);
CLT_INCLUDE_MODULE(assertion);
CLT_INCLUDE_MODULE(external);

int main(void) {
  CLT_ENTRY();

  CLT_RUN_MODULE(addition);
  CLT_RUN_MODULE(ignore);
  CLT_RUN_MODULE(assertion);
  CLT_RUN_MODULE(external);

  return CLT_END();
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

static CLT_TEST(ignore_pass) { CLT_ASSERT(true); }
static CLT_TEST(ignore_fail) { CLT_ASSERT(false); }
static CLT_TEST(ignore_should_fail) { CLT_ASSERT(false); }

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
  CLT_ASSERT(true);
  clt_assert_equal(0, 0);
}

CLT_MODULE(assertion, CLT_REGISTER(assert_pass, "Test Assert Methods"));
