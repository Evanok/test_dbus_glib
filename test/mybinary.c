#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main ()
{
  sleep(10);
  printf ("TUTU\n");
  fprintf (stderr, "TITI\n");
  exit (0);
}
