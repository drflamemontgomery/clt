#include "clit-assert.h"
#include "clit.h"

static CLIT_TEST(assert_pass) { CLIT_ASSERT(true); }

CLIT_MODULE(external, CLIT_REGISTER(assert_pass, "Test external c file test"));
