#include <stdio.h>

long long factorial(long long);

void run_factorial() {
  long long value = -1;
  do {
    printf("\nPick a number greater than or equal to 0\n");
    printf("> ");
    scanf("%lld", &value);
  } while (value < 0);

  printf("The factorial of %lld is %lld\n", value, factorial(value));
}
