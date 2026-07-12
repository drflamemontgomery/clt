# C Lightweight Testing

CLT is a lightweight unit testing framework built to be used inline with C project code. 
Keep all your tests and logic in one file. 
This project is inspired by [Unity Test](https://github.com/ThrowTheSwitch/Unity).

# Usage

```c
#include "clt/clt-runner.h"
#include "clt/clt.h"

// Include the test module foo
CLT_INCLUDE_MODULE(foo);

int main(void) {
    // Initialize CLT
    CLT_ENTRY();

    // Run all tests inside foo
    CLT_RUN_MODULE(foo);

    // Test information and CLT cleanup
    return CLT_END();
}

// Define a test using any function modifiers
CLT_TEST(foobar) { CLT_ASSERT(true); }
static CLT_TEST(barfoo) { CLT_ASSERT(false); }
extern CLT_TEST(foofoo);

// Define a Test Module
CLT_MODULE(foo,
    // Register tests with name, description, fail/ignore flags
    CLT_REGISTER(foobar, "A test description"),
    CLT_REGISTER(barfoo, "A test description", CLT_SHOULD_FAIL),
    CLT_REGISTER(foofoo, "A test description", CLT_SHOULD_IGNORE),
);
```

```bash
$ gcc -DCLT_TEST_ENABLE=1 main.c -o test_runner && ./test_runner
```

# Dedication

To my amazing girlfriend who always pushes me to finish my programming projects. 
Someday I will get around to finishing them.
