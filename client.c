#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "./helloWorld.h"
#define DUT "/dev/helloWorld"

int countLen(char *);

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
    printf("%s\n", buf);
    char *s = strdup("Write test");
    write(fd, s, strlen(s));
	
    int arr[3] = {3, 4, 5};
    int val1 = 1, val2 = 2; 
    ioctl(fd, SETBREAKFIRST, &val1);
    ioctl(fd, SETDINNER, &val2);
    ioctl(fd, SETARRAYTOKERNEL,arr); 
    puts("Done\n");

    close(fd);
    return 0;
}   
