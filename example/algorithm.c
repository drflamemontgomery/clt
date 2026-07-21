#include "clt-assert.h"
#include "clt.h"

long long factorial(long long num) {
  if (num <= 0)
    return 1;
  return num * factorial(num - 1);
}

CLT_TEST(factorial) {
  clt_assert_equal(1LL, factorial(-1));
  clt_assert_equal(1LL, factorial(0));
  clt_assert_equal(1LL, factorial(1));
  clt_assert_equal(2LL, factorial(2));
  clt_assert_equal(6LL, factorial(3));
  clt_assert_equal(720LL, factorial(6));
}

CLT_MODULE(algorithm,
            CLT_REGISTER(factorial, "Test Recursive Factorial Algorithm"));
