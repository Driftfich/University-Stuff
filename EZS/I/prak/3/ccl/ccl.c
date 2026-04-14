#include <stdint.h>
#include "ccl.h"

uint64_t rdtsc()
{
  uint64_t tsc;

#ifdef __i386__
  __asm__(
          "cpuid         \n"   /* no out-of-order execution */
          "rdtsc         \n" 
          :
          "=A" (tsc)
          :
          : "ebx", "ecx"          /* cpuid scratcht eax, ebx, ecx, edx */
          );
#endif

#ifdef __x86_64__
  __asm__(
          "cpuid         \n"   /* no out-of-order execution */
          "rdtsc         \n" 
          "shl $32,%%rdx \n"    /* x86_64 uses %rax instead of %dx/%ax    */
          "or  %%rdx,%%rax \n"  /* to store 64 bit function return values */
          :
          "=a" (tsc)
          :
          : "ebx", "ecx", "edx", "cc" /* cpuid scratcht eax, ebx, ecx, edx */
                                      /* shl+or change the flag register   */
          );
#endif

#ifdef __ia64__
  __asm__ (
          "srlz.i;        \n"   /* no out-of-order execution */
          "mov %0=ar.itc; \n"   
          :
          "=r" (tsc)
          );
#endif

  return tsc;
}
