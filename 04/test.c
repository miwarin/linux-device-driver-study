#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int ac, char** av)
{
	char buff[256];
	int fd0_A, fd0_B, fd1_A;

	printf("0x%08x\n", buff);

	if((fd0_A = open("/dev/mydevice0", O_RDWR)) < 0) perror("open fd0_A");
	if((fd0_B = open("/dev/mydevice0", O_RDWR)) < 0) perror("open fd0_B");
	if((fd1_A = open("/dev/mydevice1", O_RDWR)) < 0) perror("open fdf1_A");

	if(write(fd0_A, "0_A", 4) < 0) perror("write fd0_A");
	if(write(fd0_B, "0_B", 4) < 0) perror("write fd0_B");
	if(write(fd1_A, "1_A", 4) < 0) perror("write fd1_A");

	if(read(fd0_A, buff, 4) < 0) perror("read fd0_A");
	printf("%s\n", buff);
	if(read(fd0_B, buff, 4) < 0) perror("read fd0_B");
	printf("%s\n", buff);
	if(read(fd1_A, buff, 4) < 0) perror("read fd1_A");
	printf("%s\n", buff);

	if(close(fd0_A) != 0) perror("close fd0_A");
	if(close(fd0_B) != 0) perror("close fd0_B");
	if(close(fd1_A) != 0) perror("close fd1_A");

	return 0;
}

