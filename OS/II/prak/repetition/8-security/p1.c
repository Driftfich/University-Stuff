#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
  char key[8] = "GEHEIM!";
  char out[80];
  char msg[80];
  char cmd[20];
  
  strncpy(cmd, argv[1], 20);

  sprintf(msg, "Wrong command: %60s\n", cmd);
  sprintf(out, msg);
  printf("out = %s\n", out);
  printf("key = %s\n", key); 
}

