# CLT - C Lightweight Testing

[![Build](https://github.com/drflamemontgomery/clt/actions/workflows/release.yml/badge.svg)](https://github.com/drflamemontgomery/clt/actions/workflows/release.yml)

CLT is a lightweight unit testing framework built to be used inline with C project code. 
Keep all your tests and logic in one file. 
This project is inspired by [Unity Test](https://github.com/ThrowTheSwitch/Unity).

## Usage

```c
#include "clt/clt-runner.h"
#include "clt/clt-assert.h"
#include "clt/clt.h"

// Include the test module foo
CLT_INCLUDE_MODULE(foo);

int main(void) {
    // Initialize CLT
    clt_begin();

    // Run all tests inside foo
    clt_run_module(foo);

    // Test information and CLT cleanup
    return clt_end();
}

// Define a test using any function modifiers
CLT_TEST(foobar) { clt_assert(true); }
static CLT_TEST(barfoo) { clt_assert(false); }
extern CLT_TEST(foofoo);

// Define a Test Module
CLT_MODULE(foo,
    // Register tests with name, description, fail/ignore flags
    CLT_REGISTER(foobar, "A test description"),
    CLT_REGISTER(barfoo, "A test description", CLT_SHOULD_FAIL),
    CLT_REGISTER(foofoo, "A test description", CLT_SHOULD_IGNORE)
);
```

```bash
$ gcc main.c clt/clt.c -o test_runner
$ ./test_runner

====== foo ======
```
## Compatibility

### Tested:

| OS | Compiler | Architecture | C Standard |
|---|---|---|---|
| Arch Linux | GCC 16.0.0 | x86_64 | C11 |

### Expected to work

- Linux distributions using GCC or Clang
- macOS using Clang
- Windows using MinGW

### Currently untested

- MSVC
- Non-x86 architectures

## Notes

The internals use custom sections to allow for dead-code elimination in production builds. Refer to the [Makefile](Makefile) for information on the flags to use for linking.

## Dedication

Dedicated to my amazing girlfriend, who always encourages me to finish my programming projects.

Someday I will get around to finishing them all.
