#include <stdio.h>

int main(int argc, char **argv)
{
  fork();
  fork();
  fork();
  fork();
  printf("Hi\n");
  return 0;
}
