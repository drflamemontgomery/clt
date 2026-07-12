#include "clt-runner.h"
#include "clt.h"
#include <stdio.h>

CLT_INCLUDE_MODULE(algorithm);

enum option {
  OPT_UNKNOWN = 0,
  OPT_FACTORIAL,
  OPT_END,
};

void run_factorial();

int main(void) {
#if CLT_TEST_ENABLE
  CLT_ENTRY();
  CLT_RUN_MODULE(algorithm);
  return CLT_END();
#else

  enum option opt = 0;
  while (opt <= OPT_UNKNOWN || opt >= OPT_END) {
    printf("\nPick an option\n");
    printf("1) Factorial\n");
    printf("> ");
    scanf("%d", (int *)&opt);
  }

  switch (opt) {
  case OPT_FACTORIAL:
    run_factorial();
    break;
  default:
    break;
  }
  return 0;
#endif
}
