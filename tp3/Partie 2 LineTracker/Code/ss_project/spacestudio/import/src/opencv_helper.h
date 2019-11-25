#ifndef OPENCV_HELPER_H
#define OPENCV_HELPER_H

#include "ApplicationDefinitions.h"

int get_image(char* file, unsigned char* image_pixels, unsigned int width, unsigned int height);
void display_lines(line_points* lines, unsigned int width, unsigned int height, unsigned int maximum_nb_lines);

#endif
