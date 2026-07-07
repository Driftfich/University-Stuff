#include <stdio.h>
#include <stdlib.h>

void f(void)
{
  int x;
  char buf[8];

  x = 42;
  gets(buf);
  printf("x=%d\n", x);
  return;
}

int main(int argc, char *argv[])
{
  f();
  exit(0);
}
