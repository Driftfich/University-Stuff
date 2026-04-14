/*
 * Interface to Cycle Count Library
 */

#ifndef _ccl_h_
#define _ccl_h_

#include <stdint.h>

/* depending on the CPU architecture, exactly one of the following
   symbols must be defined:
   __i386__    for Intel IA-32 (Pentium, -II, -III, 4)
   __ia64__    for Intel Itanium
   __x86_64__  for AMD64 (Athlon64, Opteron) + Intel EM64T (i.e. 'newer' Xeons)
*/

/*
   - flushes processor pipeline by cpuid
   - reads the Time Stamp Counter (TSC) register

   RETURN VALUE:
     all 64 bits of TSC

*/
uint64_t rdtsc();

#endif
