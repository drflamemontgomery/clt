#include "clit-runner.h"
#include "clit.h"
#include <stdio.h>

CLIT_INCLUDE_MODULE(algorithm);

enum option {
  OPT_UNKNOWN = 0,
  OPT_FACTORIAL,
  OPT_END,
};

void run_factorial();

int main(void) {
#if CLIT_TEST_ENABLE
  CLIT_ENTRY();
  CLIT_RUN_MODULE(algorithm);
  return CLIT_END();
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
