#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
	int fd = open("/dev/blockDriver", O_RDWR);
	char *a = strdup("Write test");
	write(fd, a, strlen(a));

	close(fd);
	return 0;
}
