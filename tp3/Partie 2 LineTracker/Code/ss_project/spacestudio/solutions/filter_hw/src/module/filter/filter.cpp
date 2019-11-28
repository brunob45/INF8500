///////////////////////////////////////////////////////////////////////////////
//
// Filename : filter.cpp
//
// Creation date : Mon Nov 25 10:23:56 EST 2019
//
///////////////////////////////////////////////////////////////////////////////
#include "filter.h"

#include "SpaceDisplay.h"
#include "SpaceTypes.h"

// Kernel of the mask applied by the filter
const unsigned int KERNEL_DIVISOR = 49;
const unsigned int KERNEL_MATRIX_SIZE = 7; // Size of the kernel (aperture) of the filter
int kernel[KERNEL_MATRIX_SIZE][KERNEL_MATRIX_SIZE] = {1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1};

filter::filter(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose)
:SpaceBaseModule(name, period, unit, id, priority, verbose) {
	SC_THREAD(thread);
}

unsigned char my_abs(int value) {
  return (value < 0) ? -value : value;
}

/** convolution: Computes the convolution between the input matrix and the convolution mask (kernel) of size kernel_size
 *
 *  @input_address		address of the input image in the memory
 *  @output_address		address of the output image in the memory
 *  @width 				width of the image
 *  @height 			height of the image
 *  @kernel_size 		size of the mask of convolution
 *
 * **/
void filter::convolution_filter(int height, int width, int kernel_size)
{
	int sum = 0, x = 0, y = 0;

	// The position of the borders of the mask are located at kernel(center +/- kernel_half)
	int kernel_half = (kernel_size - 1 ) / 2;

	m_pixels_buffer_out_filter.memory_offset = 0;

	// Beginning of the convolution: it will be calculated for each element input(i,j).
	// the result for each point will be stocked in sum and updated at each iteration in
	// the convolution mask.

A:	for (int i = 0; i < height; i++){
B:		for (int j = 0; j < width; j++){
			// Initialization of sum
			sum=0;

			// Computation of x and y: it correspond to the position of the corresponding input matrix
			// element to the current mask element
C:			for (int ky=0; ky < kernel_size; ky++){
				#pragma HLS unroll
D:				for (int kx = 0; kx < kernel_size; kx++){
					#pragma HLS unroll
						if (j + kx - kernel_half < 0){
							x = 0;
						} else {
							if (j + kx - kernel_half > width - 1){
								x = width - 1;
							} else {
								x = j + kx - kernel_half;
							}
						}
						if (i + ky - kernel_half < 0){
							y = 0;
						} else {
							if (i + ky - kernel_half > height- 1 ){
								y = height - 1;
							} else {
								y = i + ky - kernel_half;
							}
						}
					int pixel = m_pixels_buffer_out_resize.array[x + y * width];

					sum += ((int) pixel) * kernel[ky][kx];

					// For loop D
					computeFor(7);
				}
			}

			// Put the result in output memory using a buffer
			m_pixels_buffer_out_filter.array[m_pixels_buffer_out_filter.memory_offset] = my_abs(sum / KERNEL_DIVISOR);
			m_pixels_buffer_out_filter.memory_offset++;

			// For loop B
			computeFor(8);
		}
	}
}

void filter::thread() {
	unsigned int width_scaled = 0, height_scaled = 0, address_2_read, address_2_write;
	int sum = 0, x = 0, y = 0;

	while(1) {

		//Passage des paramètres par Registre
		ModuleRead(LINE_DETECTION0_ID, SPACE_BLOCKING, &height_scaled);
		ModuleRead(LINE_DETECTION0_ID, SPACE_BLOCKING, &width_scaled);
		ModuleRead(LINE_DETECTION0_ID, SPACE_BLOCKING, &address_2_read);
		ModuleRead(LINE_DETECTION0_ID, SPACE_BLOCKING, &address_2_write);

		DeviceRead(DDR_ID, address_2_read, (int*) m_pixels_buffer_out_resize.array, LINE_DETECTION_OUTPUT_BUFFER_SIZE/4);

		convolution_filter(height_scaled, width_scaled, KERNEL_MATRIX_SIZE);

		DeviceWrite(DDR_ID, address_2_write, (int*) m_pixels_buffer_out_filter.array, LINE_DETECTION_OUTPUT_BUFFER_SIZE/4);

		int ack = 1;
		ModuleWrite(LINE_DETECTION0_ID, SPACE_BLOCKING, &ack);
//		ComputeFor(1);

	}
}
