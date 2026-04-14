/*
 * simple example for ccl lib usage
 */

#include <stdio.h>
#include <stdint.h>
#include "ccl.h"

int main(int argc, char*argv[])
{
  uint64_t start, stop;
  unsigned long long duration;

#ifdef DOIT
  unsigned long x, y;
#endif

  start = rdtsc();

  /* -- Code to measure --------------------------------------------------- */

#ifdef DOIT
  y = 1;
  for(x=0; x<100; x++) {
    y *= 2;
  }
#endif

  /* ---------------------------------------------------------------------- */

  stop = rdtsc();
  duration=stop-start;
  printf("Length in Cycles: %llu\n", duration);

  return 0;
}
