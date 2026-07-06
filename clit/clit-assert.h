#ifndef CLIT_ASSERT_H
#define CLIT_ASSERT_H

#include <stdio.h>

#ifdef CLIT_TEST_ENABLE

#define CLIT_FAIL() __clit_fail()
int __clit_fail();

#else

#define CLIT_FAIL() 1

#endif

#define CLIT_LOG_FAIL(message, ...)                                            \
  fprintf(stderr, "Failure: " message "\n" __VA_OPT__(, ) __VA_ARGS__)
#define _CLIT_ASSERT(TRUE, MSG, ...)                                           \
  if (!(TRUE) && CLIT_FAIL())                                                  \
    CLIT_LOG_FAIL(MSG __VA_OPT__(, ) __VA_ARGS__);

#define CLIT_ASSERT(TRUE) _CLIT_ASSERT(TRUE, #TRUE " Was false")

#define CLIT_ASSERT_EQUAL(EXPECTED, VALUE)                                     \
  _CLIT_ASSERT((EXPECTED) == (VALUE), "Expected %d Was %d", EXPECTED, VALUE)

#endif
