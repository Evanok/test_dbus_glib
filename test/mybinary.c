#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main ()
{
  int i, j;


  i = 0;
  for (j = 0; j < 10; j++)
  {
    while (i++ < 10)
    {
      //sleep(1);
      fprintf (stderr, "[%d][%d] error error error error error\n", j, i);
    }
  }
  sleep(3);

  i = 0;
  for (j = 0; j < 10; j++)
  {
    while (i++ < 10)
    {
      sleep(1);
      printf ("[%d][%d] out out out out out out out out\n", j, i);
    }
  }
  return 0;
}
