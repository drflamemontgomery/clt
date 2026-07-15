#ifndef CLT_ASSERT_H
#define CLT_ASSERT_H

#include <stdio.h>

#ifdef CLT_TEST_ENABLE

#define CLT_FAIL() clt_fail()
int clt_fail();

#else

#define CLT_FAIL() 1

#endif

#define CLT_EXPECTED_STR(VAR)                                                 \
  _Generic((VAR),                                                              \
      char: "%c",                                                              \
      short: "%d",                                                             \
      int: "%d",                                                               \
      long: "%ld",                                                             \
      float: "%.2f",                                                           \
      double: "%.2f",                                                          \
      default: "%ld")

#define CLT_LOG_FAIL(message, ...)                                            \
  fprintf(stderr, "Failure: " message "\n" __VA_OPT__(, ) __VA_ARGS__)
#define _CLT_ASSERT(TRUE, MSG, ...)                                           \
  if (!(TRUE) && CLT_FAIL())                                                  \
    CLT_LOG_FAIL(MSG __VA_OPT__(, ) __VA_ARGS__);

#define CLT_ASSERT(TRUE) _CLT_ASSERT(TRUE, #TRUE " Was false")

#define CLT_ASSERT_EQUAL(EXPECTED, VALUE)                                     \
  if ((EXPECTED) != (VALUE) && CLT_FAIL())                                    \
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

#define CLT_ASSERT_NOT_EQUAL(EXPECTED, VALUE)                                 \
  if ((EXPECTED) == (VALUE) && CLT_FAIL())                                    \
    fprintf(stderr,                                                            \
            _Generic((EXPECTED),                                               \
                char: "Failure: Expected any value other than %c\n",           \
                unsigned char: "Failure: Expected any value other than %u\n",  \
                short: "Failure: Expected any value other than %d\n",          \
                unsigned short: "Failure: Expected any value other than %u\n", \
                int: "Failure: Expected any value other than %d\n",            \
                unsigned int: "Failure: Expected any value other than %u\n",   \
                long: "Failure: Expected any value other than %ld\n",          \
                unsigned long: "Failure: Expected any value other than %lu\n", \
                long long: "Failure: Expected any value other than %lld\n",    \
                unsigned long long: "Failure: Expected any value other than "  \
                                    "%llu\n",                                  \
                float: "Failure: Expected any value other than %g\n",          \
                double: "Failure: Expected any value other than %lg\n",        \
                long double: "Failure: Expected any value other than %Lg\n",   \
                default: "Failure: Expected any value other than %p\n"),       \
            EXPECTED);

#endif
