#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char s[8] = "sh";

int main(int argc, char *argv[])
{
  char buf[4];

  printf("     &s = 0x%08x\n", s);
  printf("&system = 0x%08x\n", &system);

  memcpy(buf, argv[1], strlen(argv[1]));
}
