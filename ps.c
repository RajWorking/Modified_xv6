#include "types.h"
#include "user.h"

int main(int argc, char** argsv)
{
    if(argc != 1)
    {
        printf(2,"Incorrect way of calling ps :(\n");
        exit();
    }

    printf(2,"List of all active processes: \n");

    proc_info();

    exit();
}