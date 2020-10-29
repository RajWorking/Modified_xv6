#include "types.h"
#include "fcntl.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf(2, "timechk: Fatal error: No arguments provided\n");
        exit();
    }

    int pid;
    pid = fork();
    if (pid < 0)
    {
        printf(2, "timechk: Fatal error: Error in forking\n");
        exit();
    }
    else if (pid == 0)
    {
        printf(1, "Timing %s\n", argv[1]);
        if (exec(argv[1], argv + 1) < 0)
        {
            printf(2, "timechk: Fatal error: Unable to execute command\n");
            exit();
        }
    }
    else if (pid > 0)
    {
        int wtime, rtime;
        int wid = waitx(&wtime, &rtime);

        if (wtime < 0 || rtime < 0)
            printf(1, "Time report for %s: Faulty report\n", argv[1]);
        else
            printf(1, "Time report for %s\nProcess ID: %d\nWaiting time: %d\nRunning time: %d\n\n", argv[1], wid, wtime, rtime);

        exit();
    }
}
