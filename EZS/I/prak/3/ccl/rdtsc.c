#include <stdio.h>
#include <stdint.h>
#include "ccl.h"

int main(int argc, char*argv[])
{
  uint64_t c;

  c=rdtsc();
  printf("%llu\n", (unsigned long long) c);
  return 0;
}
