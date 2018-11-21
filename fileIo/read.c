#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int arg, char* argv[])
{
    char buf[64];
    int len;

    len = read(STDIN_FILENO, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, len);
    return 0;
}