#include <stdio.h>

int main(int ac, char** av)
{
	extern unsigned bcm_host_get_sdram_address(void);
	printf("0x%08x\n", bcm_host_get_sdram_address() );
	extern unsigned bcm_host_get_peripheral_address(void);
	printf("0x%08x\n", bcm_host_get_peripheral_address() );
	return 0;
}

