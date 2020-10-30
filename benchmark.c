#include "types.h"
#include "user.h"

int number_of_processes = 10;

int main(int argc, char *argv[])
{
  set_priority(0, getpid());
  int j;
  for (j = 0; j < number_of_processes; j++)
  {
    int pid = fork();
    if (pid < 0)
    {
      printf(1, "Fork failed\n");
      continue;
    }
    if (pid == 0)
    {
      volatile int i;
      for (volatile int k = 0; k < number_of_processes; k++)
      {
        if (k <= j)
        {
          // printf(1,"%d %d io time\n",k,j);
          // sleep(50); //io time
        }
        else
        {
          // printf(1,"%d %d cpu time\n",k,j);
          for (i = 0; i < 10000000; i++)
          {
            ; //cpu time
          }
        }
      }
      printf(1, "Process: %d Finished --- %d\n", j, getpid());
      exit();
    }
    else
    {
      //   set_priority(100-(20+j),pid); // will only matter for PBS, comment it out if not implemented yet (better priorty for more IO intensive jobs)
      set_priority(50 - j % 3, pid);
    }
  }
  for (j = 0; j < number_of_processes + 5; j++)
  {
    wait();
  }
  exit();
}
