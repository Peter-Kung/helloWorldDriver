#include <stdio.h>
#include <fcntl.h>

int main()
{
	int fd = open("/dev/blockDriver", O_RDWR);
	char a;
	read(fd, a, 1);
	puts("Read over\n");
	close(fd);

	return 0;	

}
