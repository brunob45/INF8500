#include "client_socket_helper.h"

#include <cstdio>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#define close closesocket
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif
	   
bool get_image_by_socket(int socket, unsigned char image[], unsigned long width, unsigned long height) {
	unsigned long remaining_bytes = width*height;
	unsigned long offset = 0;

	while(remaining_bytes>0) {
#ifdef _WIN32
		int bytes = recv(socket, (char*)(&image[offset]), remaining_bytes, 0);
#else
		int bytes = read(socket, &image[offset], remaining_bytes);
#endif
		if (bytes == -1) {
			printf("Error while reading. bytes=%d\n", bytes);
			close(socket);
			return false;
		}
		remaining_bytes-=bytes;
		offset+=bytes;
	}
	return true;
}

static bool send_raw(int socket, char* data, unsigned long bytes) {
	unsigned long remaining_bytes = bytes;
	unsigned long offset = 0;
	while(remaining_bytes>0) {
#ifdef _WIN32
		int bytes_written = send(socket, &data[offset], remaining_bytes, 0);
#else
		int bytes_written = write(socket, &data[offset], remaining_bytes);
#endif
		if (bytes_written == -1) {
			close(socket);
			return false;
		}
		remaining_bytes-=bytes_written;
		offset+=bytes_written;
	}
	return true;
}

static bool send_nb_lines(int socket, unsigned int nb_lines) {
	bool is_ok = send_raw(socket, (char*)&nb_lines, sizeof(unsigned int));
	if (!is_ok)
		printf("Error while sending number of lines\n");
	return is_ok;
}

static bool send_points(int socket, line_points* lines, unsigned int nb_lines) {
	bool is_ok = send_raw(socket, (char*)lines, sizeof(line_points)*nb_lines);
	if (!is_ok)
		printf("Error while sending points\n");
	return is_ok;
}

bool send_lines(int socket, line_points* lines, unsigned int nb_lines) {
	bool is_ok = send_nb_lines(socket, nb_lines);
	if (is_ok)
		is_ok = send_points(socket, lines, nb_lines);
	return is_ok;
}

static bool send_bytes(int socket, unsigned int nb_bytes) {
	bool is_ok = send_raw(socket, (char*)&nb_bytes, sizeof(unsigned int));
	if (!is_ok)
		printf("Error while sending number of bytes\n");
	return is_ok;
}

static bool send_image_pixels(int socket, unsigned char* image_pixels, unsigned int nb_bytes) {
	bool is_ok = send_raw(socket, (char*)image_pixels, nb_bytes);
	if (!is_ok)
		printf("Error while sending image pixels\n");
	return is_ok;
}

bool send_image(int socket, unsigned char* image_pixels, unsigned int width, unsigned int heigth) {
	bool is_ok = send_bytes(socket, width*heigth);
	if (is_ok)
		is_ok = send_image_pixels(socket, image_pixels, width*heigth);
	return is_ok;
}

bool send_duration(int socket, double delta_time) {
	bool is_ok = send_raw(socket, (char*)&delta_time, sizeof(double));
	if (!is_ok)
		printf("Error while sending duration\n");
	return is_ok;
}
