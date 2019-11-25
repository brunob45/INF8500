#ifndef CLIENT_SOCKET_HELPER_H
#define CLIENT_SOCKET_HELPER_H

#include "ApplicationDefinitions.h"

bool get_image_by_socket(int socket, unsigned char image[], unsigned long width, unsigned long height);
bool send_lines(int socket, line_points* lines, unsigned int nb_lines);
bool send_image(int socket, unsigned char* image_pixels, unsigned int width, unsigned int heigth);
bool send_duration(int socket, double delta_time);
#endif
