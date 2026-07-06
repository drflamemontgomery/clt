#ifndef CLIT_ASSERT_H
#define CLIT_ASSERT_H

#include <stdio.h>

#ifdef CLIT_TEST_ENABLE

#define CLIT_FAIL() __clit_fail()
int __clit_fail();

#else

#define CLIT_FAIL() 1

#endif

#define CLIT_EXPECTED_STR(VAR)                                                 \
  _Generic((VAR),                                                              \
      char: "%c",                                                              \
      short: "%d",                                                             \
      int: "%d",                                                               \
      long: "%ld",                                                             \
      float: "%.2f",                                                           \
      double: "%.2f",                                                          \
      default: "%ld")

#define CLIT_LOG_FAIL(message, ...)                                            \
  fprintf(stderr, "Failure: " message "\n" __VA_OPT__(, ) __VA_ARGS__)
#define _CLIT_ASSERT(TRUE, MSG, ...)                                           \
  if (!(TRUE) && CLIT_FAIL())                                                  \
    CLIT_LOG_FAIL(MSG __VA_OPT__(, ) __VA_ARGS__);

#define CLIT_ASSERT(TRUE) _CLIT_ASSERT(TRUE, #TRUE " Was false")

#define CLIT_ASSERT_EQUAL(EXPECTED, VALUE)                                     \
  if ((EXPECTED) != (VALUE) && CLIT_FAIL())                                    \
    fprintf(stderr,                                                            \
            _Generic((EXPECTED),                                               \
                char: "Failure: Expected %c Was %c\n",                         \
                unsigned char: "Failure: Expected %u Was %u\n",                \
                short: "Failure: Expected %d Was %d\n",                        \
                unsigned short: "Failure: Expected %u Was %u\n",               \
                int: "Failure: Expected %d Was %d\n",                          \
                unsigned int: "Failure: Expected %u Was %u\n",                 \
                long: "Failure: Expected %ld Was %ld\n",                       \
                unsigned long: "Failure: Expected %lu Was %lu\n",              \
                long long: "Failure: Expected %lld Was %lld\n",                \
                unsigned long long: "Failure: Expected %llu Was %llu\n",       \
                float: "Failure: Expected %g Was %g\n",                        \
                double: "Failure: Expected %lg Was %lg\n",                     \
                long double: "Failure: Expected %Lg Was %Lg\n",                \
                default: "Failure: Expected %p Was %p\n"),                     \
            EXPECTED, VALUE);

#endif
