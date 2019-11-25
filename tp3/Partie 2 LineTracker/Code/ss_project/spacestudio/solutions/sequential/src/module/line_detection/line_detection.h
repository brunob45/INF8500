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
#ifndef LINE_DETECTION_H
#define LINE_DETECTION_H

#include "SpaceBaseModule.h"
#include "ApplicationDefinitions.h"

#include "systemc"

class line_detection: public SpaceBaseModule {
	public:
		
	    SC_HAS_PROCESS(line_detection);
		
		line_detection(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose);
		void resize(unsigned int height, unsigned int width, unsigned int scale, unsigned int input_address);
		void convolution_filter(int height, int width, int kernel_size);
		void convolution_x_y_canny(int height, int width);
		void keep_edges(unsigned int low_thresh, unsigned int high_thresh, unsigned int height, unsigned int width);
		void hough(unsigned long output_address, unsigned int hough_threshold, unsigned int maximum_nb_lines, unsigned int height, unsigned int width);
		void create_accumulator(line_points* end_points , unsigned int* accumulator, int accumulator_width, int hough_height, int height, int width);
		int get_lines(unsigned long output_address, line_points* end_points, unsigned int* accumulator, long accumulator_width, long accumulator_height, long threshold,  unsigned long maximum_nb_lines);

	    void thread(void);

	private:
	    int m_nb_frame;

	    // Input FIFO buffers
	    unsigned int m_head_in_buff;
	    unsigned int m_tail_out_buff;

	    // Output FIFO buffers
	    unsigned int m_head_out_buff;
	    unsigned int m_tail_in_buff;

		// Internal cache used to avoid too many memory read
		buffer_pixel<LINE_DETECTION_INPUT_BUFFER_SIZE> m_pixels_buffer_in;
		buffer_pixel<LINE_DETECTION_OUTPUT_BUFFER_SIZE> m_pixels_buffer_out_resize;
		buffer_pixel<LINE_DETECTION_OUTPUT_BUFFER_SIZE> m_pixels_buffer_out_filter;
		buffer_pixel<LINE_DETECTION_OUTPUT_BUFFER_SIZE> m_pixels_buffer_out_canny;
		//Buffer for gradX
		buffer_pixel<LINE_DETECTION_OUTPUT_BUFFER_SIZE> m_buffer_gradX;
		//Buffer for gradY
		buffer_pixel<LINE_DETECTION_OUTPUT_BUFFER_SIZE> m_buffer_gradY;
		//Buffer for grad_norm
		buffer_pixel<LINE_DETECTION_OUTPUT_BUFFER_SIZE> m_buffer_grad_norm;
		void is_input_fifo_empty();
		void is_output_fifo_full();

};

#endif
