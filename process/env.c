#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char path[5] = "DEMO";
    printf("DEMO=%s\n", getenv(path));
    setenv(path, "helloProcess", 1);
    printf("DEMO=%s\n", getenv(path));
    return 0;
}