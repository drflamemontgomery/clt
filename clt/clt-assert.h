#ifndef CLT_ASSERT_H
#define CLT_ASSERT_H

#include <stdio.h>

#include "clt-internal.h"

CLT_TEXT_SECTION int clt_fail();
#define CLT_FAIL() clt_fail()

#define CLT_LOG_FAIL(message, ...)                                             \
  fprintf(stderr, "Failure: " message "\n" __VA_OPT__(, ) __VA_ARGS__)
#define _CLT_ASSERT(TRUE, MSG, ...)                                            \
  if (!(TRUE) && CLT_FAIL())                                                   \
    CLT_LOG_FAIL(MSG __VA_OPT__(, ) __VA_ARGS__);

#define CLT_ASSERT(TRUE) _CLT_ASSERT(TRUE, #TRUE " Was false")

#define DEF_CLT_ASSERT_EQUAL(TYPE, NAME, SPECIFIER)                            \
  inline CLT_TEXT_SECTION void clt_assert_equal_##NAME(TYPE expected,          \
                                                       TYPE value) {           \
    if (expected != value && CLT_FAIL())                                       \
      fprintf(stderr, "Failure: Expected " SPECIFIER " Was " SPECIFIER "\n",   \
              expected, value);                                                \
  }

DEF_CLT_ASSERT_EQUAL(char, char, "%c")
DEF_CLT_ASSERT_EQUAL(short, short, "%d")
DEF_CLT_ASSERT_EQUAL(int, int, "%d")
DEF_CLT_ASSERT_EQUAL(long, long, "%ld")
DEF_CLT_ASSERT_EQUAL(long long, llong, "%lld")

DEF_CLT_ASSERT_EQUAL(unsigned char, uchar, "%u")
DEF_CLT_ASSERT_EQUAL(unsigned short, ushort, "%u")
DEF_CLT_ASSERT_EQUAL(unsigned int, uint, "%u")
DEF_CLT_ASSERT_EQUAL(unsigned long, ulong, "%lu")
DEF_CLT_ASSERT_EQUAL(unsigned long long, ullong, "%llu")

DEF_CLT_ASSERT_EQUAL(float, float, "%g")
DEF_CLT_ASSERT_EQUAL(double, double, "%lg")
DEF_CLT_ASSERT_EQUAL(long double, ldouble, "%Lg")

DEF_CLT_ASSERT_EQUAL(void *, pointer, "%p")

#undef DEF_CLT_ASSERT_EQUAL

#define clt_assert_equal(EXPECTED, VALUE)                                      \
  _Generic((EXPECTED),                                                         \
      char: clt_assert_equal_char,                                             \
      unsigned char: clt_assert_equal_uchar,                                   \
      short: clt_assert_equal_short,                                           \
      unsigned short: clt_assert_equal_ushort,                                 \
      int: clt_assert_equal_int,                                               \
      unsigned int: clt_assert_equal_uint,                                     \
      long: clt_assert_equal_long,                                             \
      unsigned long: clt_assert_equal_ulong,                                   \
      long long: clt_assert_equal_llong,                                       \
      unsigned long long: clt_assert_equal_ullong,                             \
      float: clt_assert_equal_float,                                           \
      double: clt_assert_equal_double,                                         \
      long double: clt_assert_equal_ldouble,                                   \
      default: clt_assert_equal_pointer)(EXPECTED, VALUE)

#define DEF_CLT_ASSERT_NOT_EQUAL(TYPE, NAME, SPECIFIER)                        \
  inline void clt_assert_not_equal_##NAME(TYPE expected, TYPE value) {         \
    if (expected == value && CLT_FAIL())                                       \
      fprintf(stderr,                                                          \
              "Failure: Expected any value other than " SPECIFIER "\n",        \
              expected);                                                       \
  }

DEF_CLT_ASSERT_NOT_EQUAL(char, char, "%c")
DEF_CLT_ASSERT_NOT_EQUAL(short, short, "%d")
DEF_CLT_ASSERT_NOT_EQUAL(int, int, "%d")
DEF_CLT_ASSERT_NOT_EQUAL(long, long, "%ld")
DEF_CLT_ASSERT_NOT_EQUAL(long long, llong, "%lld")

DEF_CLT_ASSERT_NOT_EQUAL(unsigned char, uchar, "%u")
DEF_CLT_ASSERT_NOT_EQUAL(unsigned short, ushort, "%u")
DEF_CLT_ASSERT_NOT_EQUAL(unsigned int, uint, "%u")
DEF_CLT_ASSERT_NOT_EQUAL(unsigned long, ulong, "%lu")
DEF_CLT_ASSERT_NOT_EQUAL(unsigned long long, ullong, "%llu")

DEF_CLT_ASSERT_NOT_EQUAL(float, float, "%g")
DEF_CLT_ASSERT_NOT_EQUAL(double, double, "%lg")
DEF_CLT_ASSERT_NOT_EQUAL(long double, ldouble, "%Lg")

DEF_CLT_ASSERT_NOT_EQUAL(void *, pointer, "%p")

#undef DEF_CLT_ASSERT_NOT_EQUAL

#define clt_assert_not_equal(EXPECTED, VALUE)                                  \
  _Generic((EXPECTED),                                                         \
      char: clt_assert_not_equal_char,                                         \
      unsigned char: clt_assert_not_equal_uchar,                               \
      short: clt_assert_not_equal_short,                                       \
      unsigned short: clt_assert_not_equal_ushort,                             \
      int: clt_assert_not_equal_int,                                           \
      unsigned int: clt_assert_not_equal_uint,                                 \
      long: clt_assert_not_equal_long,                                         \
      unsigned long: clt_assert_not_equal_ulong,                               \
      long long: clt_assert_not_equal_llong,                                   \
      unsigned long long: clt_assert_not_equal_ullong,                         \
      float: clt_assert_not_equal_float,                                       \
      double: clt_assert_not_equal_double,                                     \
      long double: clt_assert_not_equal_ldouble,                               \
      default: clt_assert_not_equal_pointer)(EXPECTED, VALUE)

#endif
