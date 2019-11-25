#ifndef MAP_HELPER
#define MAP_HELPER

typedef struct {
	int fd;
	unsigned long address;
	unsigned long size;
	void* mapped_address;
	unsigned long mapped_offset;
	unsigned long mapped_size;
} mapping_info;

void map_helper(mapping_info* info);
void unmap_helper(mapping_info* info);

#endif
