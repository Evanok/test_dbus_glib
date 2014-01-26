#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main ()
{
  int i = 0;
  while (i++ < 20)
  {
    sleep(1);
    printf ("out out out out out out out out\n");
    fprintf (stderr, "error error error error error\n");
  }
  return 0;
}
