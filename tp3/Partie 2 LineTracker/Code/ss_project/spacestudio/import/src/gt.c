#include "gt.h"

#if defined(SPACE_GENX) || defined(SPACE_HLS)
#include "map_helper.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define GT_BASEADDR 0xF8F00200
#define GT_SIZE 0x10

#define LOWER_COUNTER_OFFSET 0x0
#define HIGHER_COUNTER_OFFSET 0x4
#define CONTROL_OFFSET 0x8

static mapping_info info = { 0, GT_BASEADDR, GT_SIZE, 0, 0, 0 };

void map_gt() {
	map_helper(&info);
}

void unmap_gt() {
	unmap_helper(&info);
}

void reset_gt() {
	*((unsigned long*)(unsigned char*)(info.mapped_address + info.mapped_offset + CONTROL_OFFSET)) = 0;
	*((unsigned long*)(unsigned char*)(info.mapped_address + info.mapped_offset + LOWER_COUNTER_OFFSET)) = 0;
	*((unsigned long*)(unsigned char*)(info.mapped_address + info.mapped_offset + HIGHER_COUNTER_OFFSET)) = 0;
}

void start_gt() {
	*((unsigned long*)(unsigned char*)(info.mapped_address + info.mapped_offset + CONTROL_OFFSET)) = 1;
}

void stop_gt() {
	*((unsigned long*)(unsigned char*)(info.mapped_address + info.mapped_offset + CONTROL_OFFSET)) = 0;
}

static unsigned long read_high_value_gt() {
	return *((unsigned long*)(unsigned char*)(info.mapped_address + info.mapped_offset + HIGHER_COUNTER_OFFSET));
}

static unsigned long read_low_value_gt() {
	return *((unsigned long*)(unsigned char*)(info.mapped_address + info.mapped_offset + LOWER_COUNTER_OFFSET));
}

unsigned long long get_time_gt() {
	unsigned long low, high;

	do {
		high = read_high_value_gt();
		low = read_low_value_gt();
	} while(read_high_value_gt() != high);

	return ((unsigned long long)high) << 32 | ((unsigned long long)low);
}
#else
void reset_gt() { }
void start_gt() { }
void stop_gt() { }
void map_gt() { }
void unmap_gt() { }
unsigned long long get_time_gt() { return 0; }
#endif
