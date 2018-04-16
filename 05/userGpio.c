#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

#define REG_ADDR_BASE (0x3F000000)
#define REG_ADDR_GPIO_BASE (REG_ADDR_BASE + 0x00200000)
#define REG_ADDR_GPIO_LENGTH 4096
#define REG_ADDR_GPIO_GPFSEL_0 0x0000
#define REG_ADDR_GPIO_OUTPUT_SET_0 0x001C
#define REG_ADDR_GPIO_OUTPUT_CLR_0 0x0028
#define REG_ADDR_GPIO_LEVEL_0      0x0034
#define REG(addr) (*((volatile unsigned int*)(addr)))
#define DUMP_REG(addr) printf("0x%08x\n", REG(addr));


int main(int ac, char**av)
{
	int address;
	int fd;

	if((fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0 ) {
		perror("open");
		return -1;
	}

	address = (int)mmap(NULL, REG_ADDR_GPIO_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, REG_ADDR_GPIO_BASE);
	if(address == (int)MAP_FAILED) {
		perror("mmap");
		close(fd);
		return -1;
	}

	REG(address + REG_ADDR_GPIO_GPFSEL_0) = 1 << 12;

	REG(address + REG_ADDR_GPIO_OUTPUT_SET_0) = 1 << 4;
	DUMP_REG(address + REG_ADDR_GPIO_LEVEL_0);

	REG(address + REG_ADDR_GPIO_OUTPUT_CLR_0) = 1 << 4;
	DUMP_REG(address + REG_ADDR_GPIO_LEVEL_0);

	munmap((void*)address, REG_ADDR_GPIO_LENGTH);
	close(fd);

	return 0;
}

