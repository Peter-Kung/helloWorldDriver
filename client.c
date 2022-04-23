#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
 #include <unistd.h>
#define DUT "/dev/helloWorld"

int main()
{
    puts("This is test file");
    int fd = open(DUT, O_RDWR);
    if (fd < 0) {
        puts("fail open device\n");
        exit(1);
    }
    
    char buf[100] = {0};
    read(fd, buf, 100);
    printf("%s", buf);

    return 0;
}   
