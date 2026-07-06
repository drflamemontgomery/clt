#include "clit-assert.h"
#include "clit.h"

long long factorial(long long num) {
  if (num <= 0)
    return 1;
  return num * factorial(num - 1);
}

CLIT_TEST(factorial) {
  CLIT_ASSERT_EQUAL(1LL, factorial(-1));
  CLIT_ASSERT_EQUAL(1LL, factorial(0));
  CLIT_ASSERT_EQUAL(1LL, factorial(1));
  CLIT_ASSERT_EQUAL(2LL, factorial(2));
  CLIT_ASSERT_EQUAL(6LL, factorial(3));
  CLIT_ASSERT_EQUAL(720LL, factorial(6));
}

CLIT_MODULE(algorithm,
            CLIT_REGISTER(factorial, "Test Recursive Factorial Algorithm"));
