#include "clit-assert.h"
#include "clit-runner.h"
#include "clit.h"

CLIT_INCLUDE_MODULE(addition);
CLIT_INCLUDE_MODULE(ignore);
CLIT_INCLUDE_MODULE(assertion);

int main(void) {
  CLIT_ENTRY();

  CLIT_RUN_MODULE(addition);
  CLIT_RUN_MODULE(ignore);
  CLIT_RUN_MODULE(assertion);

  return CLIT_END();
}

/*
 * Addition Tests
 */

CLIT_TEST(add) { CLIT_ASSERT_EQUAL(2, 1 + 1); }
CLIT_TEST(fail_add) { CLIT_ASSERT_EQUAL(5, 2 + 2); }

CLIT_MODULE(addition, CLIT_REGISTER(add, "Test adding of two numbers"),
            CLIT_REGISTER(fail_add, "Test incorrect adding of two numbers",
                          CLIT_SHOULD_FAIL));
/*
 * Ignore Tests
 */

CLIT_TEST(ignore_pass) { CLIT_ASSERT(true); }
CLIT_TEST(ignore_fail) { CLIT_ASSERT(false); }
CLIT_TEST(ignore_should_fail) { CLIT_ASSERT(false); }

CLIT_MODULE(ignore,
            CLIT_REGISTER(ignore_pass, "Ignoring test that is expected to pass",
                          CLIT_SHOULD_IGNORE),
            CLIT_REGISTER(ignore_fail,
                          "Ignoring test that is expected to pass but fails",
                          CLIT_SHOULD_IGNORE),
            CLIT_REGISTER(ignore_should_fail,
                          "Ignoring test that is expected to fail",
                          CLIT_SHOULD_IGNORE | CLIT_SHOULD_FAIL));

/*
 * Assertion Tests
 */

CLIT_TEST(assert_pass) {
  CLIT_ASSERT(true);
  CLIT_ASSERT_EQUAL(0, 0);
}

CLIT_MODULE(assertion, CLIT_REGISTER(assert_pass, "Test Assert Methods"));
