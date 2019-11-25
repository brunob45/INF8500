#include "map_helper.h"

#if defined(SPACE_GENX) || defined(SPACE_HLS)
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void map_helper(mapping_info* info) {
	info->fd = open("/dev/mem", O_RDWR);
	if (info->fd < 0)
		printf("Error while opening /dev/mem \n");

	int page_size = 0x10000;
	info->mapped_size = ((info->size / page_size) + 1) * page_size;
	info->mapped_offset = info->address & page_size-1;
	info->mapped_address = mmap(0, info->mapped_size, PROT_READ | PROT_WRITE, MAP_SHARED, info->fd, info->address & ~(page_size-1));
	if (info->mapped_address == MAP_FAILED)
		printf("Error while mapping virtual memory \n");
}

void unmap_helper(mapping_info* info) {
	if(munmap(info->mapped_address, info->mapped_size) == -1)
		printf("Error while unmapping virtual memory\n");
	close(info->fd);

}
#else
void map_helper(mapping_info* info) { }
void unmap_helper(mapping_info* info) { }
#endif
