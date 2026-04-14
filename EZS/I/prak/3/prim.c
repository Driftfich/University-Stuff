#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  unsigned long a,b, maxcount;

  if (argc==1) {
    printf("Usage: %s <max_count>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  maxcount = strtoul(argv[1], NULL, 10);

  printf("2\n3\n");

  for (a=5; a<maxcount; a+=2) {
    for (b=3; b*b<=a && !(a%b==0); b+=2) {}
    if (b*b>a) {
      printf("%ld\n", a);
    }
  }

  exit(EXIT_SUCCESS);
}
