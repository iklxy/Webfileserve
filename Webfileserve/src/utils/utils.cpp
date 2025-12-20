#include "../include/utils/utils.h"
#include <fcntl.h>  // fcntl
#include <unistd.h> // close

int set_nonblocking(int fd)
{
    int oldflags = fcntl(fd, F_GETFL);
    if (oldflags < 0)
    {
        return -1;
    }
    int newflags = oldflags | O_NONBLOCK;
    int ret = fcntl(fd, F_SETFL, newflags);
    return ret;
}