#include "clt-assert.h"
#include "clt.h"

static CLT_TEST(assert_pass) { CLT_ASSERT(true); }

CLT_MODULE(external, CLT_REGISTER(assert_pass, "Test external c file test"));
