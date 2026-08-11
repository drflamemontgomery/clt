#include "clt-assert.h"
#include "clt-runner.h"
#include "clt.h"

CLT_INCLUDE_MODULE(failures);

int main(void) {
  clt_begin();

  clt_run_module(failures);

  return clt_end();
}

/*
 * Addition Tests
 */
static CLT_TEST(fail) { clt_assert_equal(2, 1 + 2); }
static CLT_TEST(dont_fail) { clt_assert_equal(2, 1 + 1); }

CLT_MODULE(failures, CLT_REGISTER(fail, "Test adding of two numbers"),
           CLT_REGISTER(dont_fail, "Test adding of two numbers", CLT_SHOULD_FAIL));
