///////////////////////////////////////////////////////////////////////////////
//
// Filename : filter.cpp
//
// Creation date : Tue Nov 12 16:03:56 EST 2019
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

void filter::thread() {
	unsigned int width_scaled = 0, height_scaled = 0;
	buffer_pixel<LINE_DETECTION_OUTPUT_BUFFER_SIZE> input_address, output_address; //définition LINE_DETECTION_OUTPUT_BUFFER_SIZE ?
	int sum = 0, x = 0, y = 0;

	//Communication line_detection.cpp (Protocole handshaking)
	int ready = 0;
	int ack = 0;

	while(1) {


		//Attente appel Thread filter depuis line_detection.cpp
		do {
			RegisterRead(REGISTERFILE0_ID, 21, &ready);
		} while (ready == 0);

		//Communication line_detection.cpp
		ack = 1;
		RegisterWrite(REGISTERFILE0_ID, 22, &ack);


		//Passage des paramètres par Registre
		RegisterRead(REGISTERFILE0_ID, 17, &height_scaled);
		RegisterRead(REGISTERFILE0_ID, 18, &width_scaled);
		RegisterRead(REGISTERFILE0_ID, 19, &input_address);
		RegisterRead(REGISTERFILE0_ID, 20, &output_address);


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
	D:				for (int kx = 0; kx < kernel_size; kx++){
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
				m_pixels_buffer_out_filter.array[m_pixels_buffer_out_filter.memory_offset] = (unsigned char) std::abs((double)(sum /KERNEL_DIVISOR));
				m_pixels_buffer_out_filter.memory_offset++;

				// For loop B
				computeFor(8);
			}
		}

		//Communication line_detection.cpp : travail fini
		ack = 0;
		RegisterWrite(REGISTERFILE0_ID, 22, &ack);

	}
}
