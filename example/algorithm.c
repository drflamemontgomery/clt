#include "clt-assert.h"
#include "clt.h"

long long factorial(long long num) {
  if (num <= 0)
    return 1;
  return num * factorial(num - 1);
}

CLT_TEST(factorial) {
  CLT_ASSERT_EQUAL(1LL, factorial(-1));
  CLT_ASSERT_EQUAL(1LL, factorial(0));
  CLT_ASSERT_EQUAL(1LL, factorial(1));
  CLT_ASSERT_EQUAL(2LL, factorial(2));
  CLT_ASSERT_EQUAL(6LL, factorial(3));
  CLT_ASSERT_EQUAL(720LL, factorial(6));
}

CLT_MODULE(algorithm,
            CLT_REGISTER(factorial, "Test Recursive Factorial Algorithm"));
