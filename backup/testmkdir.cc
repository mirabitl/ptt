#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
struct stat st = {0};

if (stat("/dev/shm/laurent", &st) == -1) {
    mkdir("/dev/shm/laurent", 0700);
}
}
