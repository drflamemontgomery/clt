#include "clit-assert.h"
#include "clit-runner.h"
#include "clit.h"

CLIT_INCLUDE_MODULE(main);

int main(void) {
  CLIT_ENTRY();

  CLIT_RUN_MODULE(main);

  return CLIT_END();
}

CLIT_TEST(add) { CLIT_ASSERT_EQUAL(2, 1 + 1); }
CLIT_TEST(fail_add) { CLIT_ASSERT_EQUAL(4, 2 + 2); }

CLIT_MODULE(main, CLIT_REGISTER(add, "Test adding of two numbers"),
            CLIT_REGISTER(fail_add, "Test incorrect adding of two numbers",
                          CLIT_SHOULD_FAIL), );
