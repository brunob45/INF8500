///////////////////////////////////////////////////////////////////////////////
///
///         IRT Antoine de Saint-Exupéry - (http://http://www.irt-saintexupery.com/)
///         (c) All Rights Reserved. 2016
///
///         No authorization to modify or use this file for
///         commercial purposes without prior written consentement
///         of its author(s)
///
///////////////////////////////////////////////////////////////////////////////
#include "line_detection.h"

#include "PlatformDefinitions.h"
#include "ApplicationDefinitions.h"
#include "SpaceDisplay.h"
#include "SpaceTypes.h"

#include <math.h>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Kernel of the mask apllied by the filter
const unsigned int KERNEL_DIVISOR = 49;
const unsigned int KERNEL_MATRIX_SIZE = 7; // Size of the kernel (aperture) of the filter
int kernel[KERNEL_MATRIX_SIZE][KERNEL_MATRIX_SIZE] = {1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1,
													 1,1,1,1,1,1,1};

line_detection::line_detection(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose)
:SpaceBaseModule(name, period, unit, id, priority, verbose) {
	SC_THREAD(thread);
}

void line_detection::thread(void) {
	unsigned int height = 0, width = 0, resizing_scale = 0, command_status = 0, width_scaled = 0, height_scaled = 0, input_address = 0, output_address= 0, low_thresh = 0, high_thresh = 0, hough_threshold = 0, maximum_nb_lines = 0;
	float sumResize = 0, sumFilter = 0, sumCanny = 0, sumHough = 0;

	m_nb_frame = 1;

	// Initialize input FIFO buffers
	m_head_in_buff = 0;
	m_tail_out_buff = 0;

	// Initialize output FIFO buffers
	m_head_out_buff = 0;
	m_tail_in_buff = 0;

	computeFor(1); // Wait reset cycle

	while(1) {
		//Update output FIFO head value
		ModuleWrite(DISPLAY0_ID, SPACE_BLOCKING, &m_head_out_buff);
		//Update input FIFO tail value
		ModuleWrite(RECEPTION0_ID, SPACE_BLOCKING, &m_tail_out_buff);

		//Before reading, verify if input FIFO is not empty
		is_input_fifo_empty();

		// Read the original size of the image
		RegisterRead(REGISTERFILE0_ID, 0, &height);
		RegisterRead(REGISTERFILE0_ID, 1, &width);
		RegisterRead(REGISTERFILE0_ID, 2, &resizing_scale);
		RegisterRead(REGISTERFILE0_ID, 3, &height_scaled);
		RegisterRead(REGISTERFILE0_ID, 4, &width_scaled);
		RegisterRead(REGISTERFILE0_ID, 5, &input_address);
		RegisterRead(REGISTERFILE0_ID, 6, &output_address);
		RegisterRead(REGISTERFILE0_ID, 7, &low_thresh);
		RegisterRead(REGISTERFILE0_ID, 8, &high_thresh);
		RegisterRead(REGISTERFILE0_ID, 9, &hough_threshold);
		RegisterRead(REGISTERFILE0_ID, 10, &maximum_nb_lines);
		RegisterRead(REGISTERFILE0_ID, 11, &command_status);

		if(KERNEL_MATRIX_SIZE % 2 == 0) SpaceMessageError("Line detection", "Error kernel_size is divided by 2");


		// Resize
		resize(height, width, resizing_scale, input_address);

		// Filter
		convolution_filter(height_scaled, width_scaled, KERNEL_MATRIX_SIZE);

		//Canny
		convolution_x_y_canny(height_scaled, width_scaled);
		keep_edges(low_thresh, high_thresh, height_scaled, width_scaled);

		//Hough
		unsigned int output_memory_offset = maximum_nb_lines * sizeof(line_points) * m_head_out_buff;
		unsigned int address_2_write = output_address + output_memory_offset;

		hough(address_2_write, hough_threshold, maximum_nb_lines, height_scaled, width_scaled);


		//unsigned int address_2_write = output_address + (height_scaled * width_scaled * m_head_out_buff);
		//unsigned char* tmp = &m_pixels_buffer_out_canny.array[0];
		//DeviceWrite(ZYNQ_DDR_ID, address_2_write, tmp, LINE_DETECTION_OUTPUT_BUFFER_SIZE);
		//m_pixels_buffer_out_canny.memory_offset = 0;


		is_output_fifo_full();

		//Update output FIFO head value
		m_head_out_buff = (m_head_out_buff + 1) % FIFO_LENGTH;

		//Update input FIFO tail value
		m_tail_out_buff=(m_tail_out_buff + 1) % FIFO_LENGTH;

		// For loop while
		computeFor(8);

		m_nb_frame++;
	}
}

// Verify if input memory FIFO is empty, if so it will block until an image is written to the FIFO
void line_detection::is_input_fifo_empty(){
	ModuleRead(RECEPTION0_ID, SPACE_BLOCKING, &m_head_in_buff);
	if(m_head_in_buff == m_tail_out_buff) {
		SpaceMessageVerbose(GetVerbose(), "Line detection", "Input FIFO is empty  - head = %d tail = %d", m_head_in_buff, m_tail_out_buff);
		ModuleRead(RECEPTION0_ID, SPACE_BLOCKING, &m_head_in_buff);
	}
}

// Verify if output memory FIFO is full, if so it will block until an image is read from the FIFO
void line_detection::is_output_fifo_full(){
	ModuleRead(DISPLAY0_ID, SPACE_BLOCKING, &m_tail_in_buff);
	if((m_head_out_buff + 1)  % FIFO_LENGTH ==  m_tail_in_buff){
		SpaceMessageVerbose(GetVerbose(), "Line detection", "Output FIFO is full - head = %d tail = %d", m_head_out_buff, m_tail_in_buff);
		ModuleRead(DISPLAY0_ID, SPACE_BLOCKING, &m_tail_in_buff);
	}
}

void line_detection::resize(unsigned int height, unsigned int width, unsigned int scale, unsigned int input_address) {
	// Compute the new size of the image
	unsigned int new_size =(height*width) / (scale*scale);
	unsigned int input_memory_offset = width * height * m_tail_out_buff;
	unsigned int address_2_read = input_address + input_memory_offset;

	// Initializing internal buffer used to cache memory values
	unsigned char* tmp = &m_pixels_buffer_in.array[0];
	m_pixels_buffer_in.memory_offset = 0;
	DeviceRead(DDR_ID, address_2_read, (unsigned int*)tmp, LINE_DETECTION_INPUT_BUFFER_SIZE/4);

	m_pixels_buffer_out_resize.memory_offset = 0;

	// Image reduction: we group the pixel by scale*scale packets
A:	for (unsigned int i=0; i < new_size; i++) {
		unsigned int sum = 0;

B:		for (unsigned int j=0; j < scale*scale; j++){
			unsigned int offset = (i*scale) % width + ((i*scale)/width) * scale * width + j % scale + (j/scale)*width;
			sum += m_pixels_buffer_in.array[offset];

			// For loop B
			computeFor(35);
		}

		unsigned int pixel_average =  sum/(scale*scale);

		// Put the result in output memory unsing a buffer
		// Max average value is 255, so just keep one byte
		m_pixels_buffer_out_resize.array[m_pixels_buffer_out_resize.memory_offset] = pixel_average & 0xFF;

		m_pixels_buffer_out_resize.memory_offset++;

		// For loop A
		computeFor(25); //8
	}
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
void line_detection::convolution_filter(int height, int width, int kernel_size)
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
}


/** convolution: Computes the convolution for X-coordinate and Y-coordinate and the gradient norm (here its not really the norm: it's a simple average)
 *
 *  @width 				width of the image
 *  @height 			height of the image
 *
 * **/
void line_detection::convolution_x_y_canny(int height, int width)
{
	int x = 0, y = 0;
	const int GRAD_KERNEL_SIZE = 3;
	const int KERNEL_DIVISOR = 1;
	const int GRAD_X_KERNEL[GRAD_KERNEL_SIZE][GRAD_KERNEL_SIZE] = {{-1, 0, 1},{-2, 0, 2},{-1, 0, 1}};// Mask of convolution to get the X gradient for each pixel
	const int GRAD_Y_KERNEL[GRAD_KERNEL_SIZE][GRAD_KERNEL_SIZE] = {{1, 2, 1},{0, 0, 0},{-1, -2, -1}};// Mask of convolution to get the Y gradient for each pixel

	// The position of the borders of the mask are located at kernel(center +/- kernel_half)
	int kernel_half = (GRAD_KERNEL_SIZE - 1 ) / 2;

	m_buffer_gradX.memory_offset = 0;
	m_buffer_gradY.memory_offset = 0;
	m_buffer_grad_norm.memory_offset = 0;

	// Beggining of the convolution: it will be calculated for each element input(i,j).
	// the result for each point will be stocked in sum and updated at each iteration in
	// the convolution mask.

A:	for (int i = 0; i < height; i++){
B:		for (int j = 0; j < width; j++){
			// Initialization of sum
			int sumX = 0, sumY = 0;

			// Computation of x and y: it correspond to the position of the corresponding input matrix
			// element to the current mask element
C:			for (int ky=0; ky < GRAD_KERNEL_SIZE; ky++){
D:				for (int kx = 0; kx < GRAD_KERNEL_SIZE; kx++){
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

					int pixel = m_pixels_buffer_out_filter.array[x + y * width];

					sumX += ((int) pixel) * (GRAD_X_KERNEL[ky][kx]);
					sumY += ((int) pixel) * (GRAD_Y_KERNEL[ky][kx]);

					// For loop D
					computeFor(9);
				}
			}

			// Put the result in output memory unsing a buffer
			m_buffer_gradX.array[m_buffer_gradX.memory_offset] = (unsigned char) std::abs(sumX /KERNEL_DIVISOR);
			m_buffer_gradY.array[m_buffer_gradY.memory_offset] = (unsigned char) std::abs(sumY /KERNEL_DIVISOR);
			m_buffer_grad_norm.array[m_buffer_grad_norm.memory_offset] =  (m_buffer_gradX.array[m_buffer_gradX.memory_offset] + m_buffer_gradY.array[m_buffer_gradY.memory_offset]) / 2;

			m_buffer_gradX.memory_offset++;
			m_buffer_gradY.memory_offset++;
			m_buffer_grad_norm.memory_offset++;

			// For loop B
			computeFor(5);
		}
	}
}

/** keep_edges: Keep egdes depending on grad_norm
 *
 *  @low_thresh			low threshold
 *  @high_thresh		high threshold
 *  @width				image width
 *  @height				image height
 *
 * **/
void line_detection::keep_edges(unsigned int low_thresh, unsigned int high_thresh, unsigned int height, unsigned int width)
{
	bool condition1, condition2, condition3, condition4;

	m_pixels_buffer_out_canny.memory_offset = 0;

E:	for (unsigned int i = 0; i < height; i++){
F:		for(unsigned int j = 0; j < width; j++){
			bool edge = false;
			int offset = j + (i*width);

			// We ignore the first/last line and columns  because some condition need to access the pixel [i-width / i + width ]
			if(i != 0 && j != 0 && i != height-1 && j != width-1 ) {
				condition1 = condition2 = condition3 = condition4 = false;
				// Computation of the orientation of the detected edge

				unsigned char gradX = m_buffer_gradX.array[offset];
				unsigned char gradY = m_buffer_gradY.array[offset];
				double angle = atan(gradY/(gradX + 0.000000001));

				unsigned char a0 = m_buffer_grad_norm.array[offset];
				unsigned char a1 = m_buffer_grad_norm.array[offset - 1];
				unsigned char a2 = m_buffer_grad_norm.array[offset + 1];
				unsigned char a3 = m_buffer_grad_norm.array[offset - width - 1];
				unsigned char a4 = m_buffer_grad_norm.array[offset + width + 1];
				unsigned char a5 = m_buffer_grad_norm.array[offset + width - 1];
				unsigned char a6 = m_buffer_grad_norm.array[offset - width + 1];
				unsigned char a7 = m_buffer_grad_norm.array[offset - width];
				unsigned char a8 = m_buffer_grad_norm.array[offset + width];

				// If the gradient has a greater value than its neighbors it will be kept else, it will be set to 0
				condition1 = (((angle < M_PI/8) || (angle > -M_PI/8)) && (a0 > a1) && (a0 > a2));
				condition2 = ((angle >= M_PI/8) && (angle < 3 * M_PI/8) && (a0 > a3) && (a0 > a4));
				condition3 = ((angle > -3 * M_PI/8)  && (angle <= -M_PI/8) && (a0 > a5) && (a0 > a6));
				condition4 = (((angle >= 3 * M_PI/8) || (angle<=-3*M_PI/8)) && (a0 > a7) && (a0 > a8));



				//TODO: This would be enough to find the edges
				/*if (condition1 || condition2 || condition3 || condition4){
					if(grad_norm[i] > high_thresh)
						edge = true
				}
				*/
				if (condition1 || condition2 || condition3 || condition4){
					// Verifies if the pixel has an higher value than high threshold
					if(a0 > high_thresh) {
						edge = true;
					} else {
						// Verifies if the pixel has a neighbor has an higher value than high threshold
						if (a0 > low_thresh) {
							if((a1 > high_thresh) || (a2 > high_thresh) || (a3 > high_thresh) || (a4 > high_thresh) ||
							   (a5 > high_thresh) || (a6 > high_thresh) || (a7 > high_thresh) || (a8 > high_thresh)) {
								edge = true;
							}
						}
					}
				}
			}
			// Put the result in output memory using a buffer
			if(edge)
				m_pixels_buffer_out_canny.array[m_pixels_buffer_out_canny.memory_offset] = 255;
			else
				m_pixels_buffer_out_canny.array[m_pixels_buffer_out_canny.memory_offset] = 0;

			m_pixels_buffer_out_canny.memory_offset++;

			// For loop F - Average from HLS report
			computeFor(65);
		}
	}
}

void line_detection::hough(unsigned long output_address, unsigned int hough_threshold, unsigned int maximum_nb_lines, unsigned int height, unsigned int width){
	// Create the accumulator (Hough space construction)
	double hough_height = ((std::sqrt(2.0) * (double)(height > width ? height:width)) / 2.0);
	// Height and Width of the hough space
	int accumulator_height = (int)hough_height * 2.0;
	const int accumulator_width = 180;

	// Accumulator buffers (r,Theta)
	unsigned int* accumulator = new unsigned int[accumulator_width*accumulator_height];
	std::memset(accumulator,0,accumulator_width*accumulator_height*sizeof(unsigned int));

	// Buffer containing the coordinates of the points at the extremities of the lines
	line_points* end_points = new line_points[accumulator_width*accumulator_height];
A:		for (int i = 0; i < accumulator_width*accumulator_height; i++) {
		end_points[i].x0 = width;
		end_points[i].y0 = height;
		end_points[i].x1 = 0;
		end_points[i].y1 = 0;
	}
	// For loop A
	computeFor(226);

	create_accumulator(end_points , accumulator, accumulator_width, hough_height, height, width);

	// Extraction of the lines on the hough space (threshold etc..)
	int line_counter = get_lines(output_address, end_points, accumulator, accumulator_width, accumulator_height, hough_threshold, maximum_nb_lines);
I:	for(unsigned int i = line_counter; i < maximum_nb_lines; i++) {
		line_points point = {0, 0, 0, 0};
		DeviceWrite(DDR_ID, output_address + (i * sizeof(line_points)), &point, 1);
	}

	// For loop while
	computeFor(20);

	delete[] accumulator;
	delete[] end_points;

}

void line_detection::create_accumulator(line_points* end_points , unsigned int* accumulator, int accumulator_width, int hough_height, int height, int width){
	// Computing the coordinates of the "origin" of the Hough space
	int center_x = width/2;
	int center_y = height/2;
/*
	// Loops over every point in the edge image and computes r for every t
B:	for(int nb_buffer=0; nb_buffer < height*width; nb_buffer = nb_buffer + LINE_DETECTION_OUTPUT_BUFFER_SIZE) {
		//Loops over the cache memory
C:		for(unsigned int j = 0; j < LINE_DETECTION_OUTPUT_BUFFER_SIZE; j++) {
			int nb_pixel = j;
			int x = nb_pixel % width;
			int y = (nb_pixel - (nb_pixel % width)) / width;

			if(m_pixels_buffer_out_canny.array[j] > 250 ) {
				// Computing the distance between the origin and all the lines passing by the point
				// of the image input (x,y) in the polar space. t is the orientation in degree
D:				for(int t = 0; t < accumulator_width; t++){
					// radians = degree * (pi / 180)
					double r = ( ((double)x - center_x) * cos((double)t * M_PI/180)) + (((double)y - center_y) * sin((double)t * M_PI/180));
					// r can be negative, then we add hough_height
					int offset = (int)((round(r + hough_height) * accumulator_width)) + t;
					accumulator[offset]++;

					// Verifies if the point is an end point of the detected segment
					if (x < end_points[offset].x0){
						end_points[offset].x0 = x;
						end_points[offset].y0 = y;
					}
					if (x > end_points[offset].x1){
						end_points[offset].x1 = x;
						end_points[offset].y1 = y;
					}
					// For loop D
					computeFor(220);
				}
			}
			// For loop C
			computeFor(10);
		}
	}

*/

	int edge_pixel_size = 0;
	unsigned char* edges_pixels = new unsigned char[width*height*2] ;
	// Loops over every point in the edge image and computes r for every t
B:	for(int nb_buffer=0; nb_buffer < height*width; nb_buffer = nb_buffer + LINE_DETECTION_OUTPUT_BUFFER_SIZE) {
		//Loops over the cache memory
C:		for(unsigned int j = 0; j < LINE_DETECTION_OUTPUT_BUFFER_SIZE; j++) {
			int nb_pixel = j;
			int x = nb_pixel % width;
			int y = (nb_pixel - (nb_pixel % width)) / width;

			if(m_pixels_buffer_out_canny.array[j] > 250 ) {
				edges_pixels[edge_pixel_size++] = x;
				edges_pixels[edge_pixel_size++] = y;
			}
			// For loop C
			computeFor(10);
		}
	}

	// Computing the distance between the origin and all the lines passing by the point
	// of the image input (x,y) in the polar space. t is the orientation in degree
D:	for(int t = 0; t < accumulator_width; t++){
		for(int edge = 0; edge < edge_pixel_size; edge += 2){
			int x = edges_pixels[edge];
			int y = edges_pixels[edge+1];

			// radians = degree * (pi / 180)
			double r = ( ((double)x - center_x) * cos((double)t * M_PI/180)) + (((double)y - center_y) * sin((double)t * M_PI/180));
			// r can be negative, then we add hough_height
			int offset = (int)((round(r + hough_height) * accumulator_width)) + t;
			accumulator[offset]++;

			// Verifies if the point is an end point of the detected segment
			if (x < end_points[offset].x0){
				end_points[offset].x0 = x;
				end_points[offset].y0 = y;
			}
			if (x > end_points[offset].x1){
				end_points[offset].x1 = x;
				end_points[offset].y1 = y;
			}
			// For loop D
			computeFor(220);
		}
	}

	delete[] edges_pixels;
}

/** get_lines: Get lines from accumulator and write to the memory
 *
 *  @end_points				begin and end point of a line
 *  @accumulator			accumulator
 *  @accumulator_width		accumulator width
 *  @accumulator_height		accumulator height
 *  @threshold				accumulator threshold
 *  @maximum_nb_lines		maximum number of lines
 *  @output_address			address of the output image in the memory
 *
 * **/
int line_detection::get_lines(unsigned long output_address, line_points* end_points, unsigned int* accumulator, long accumulator_width, long accumulator_height, long threshold,  unsigned long maximum_nb_lines) {
	unsigned int line_counter = 0;

E:	for(int r = 0; r < accumulator_height; r++) {
F:		for(int t = 0;t < accumulator_width; t++) {
			int accumulator_index = (r*accumulator_width) + t;
			int max = accumulator[accumulator_index];
			if( max >= threshold) {
        		// Verify if the point is a local maxima
G:        		for(int ly = -4; ly <= 4; ly++) {
H:					for(int lx = -4; lx <= 4; lx++) {
						if((ly + r >= 0 && ly + r < accumulator_height) && (lx + t >= 0 && lx + t < accumulator_width)){
							if(accumulator[((r + ly) * accumulator_width) + (t + lx)] > max ){
								max = accumulator[((r + ly) * accumulator_width) + (t + lx)];
								ly = lx = 5;
								// For loop H
								computeFor(20);
							}
						}
					}
				}

        		if(max >accumulator[accumulator_index])
        			continue;

				// Put the coordinates of the end_points of the line in the lines buffer
        		if(line_counter < maximum_nb_lines) {
    				DeviceWrite(DDR_ID, output_address + (line_counter * sizeof(line_points)), &end_points[accumulator_index], 1);
    			}
        		else{
        			return line_counter;
        		}

				line_counter++;
			}
		}
	}

	return line_counter;

}


