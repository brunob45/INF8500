#include "common_read.h"
#include "definitions.h"
#include "server_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <windows.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

static server_handler* m_handler;
static const char* context = "camera";

typedef struct {
    unsigned int x0;
    unsigned int y0;
    unsigned int x1;
    unsigned int y1;
} line_points;

void display_image(unsigned char image_pixels[], unsigned int width, unsigned int height);
bool get_image(unsigned char* image_pixels, unsigned long bytes);
bool get_bytes(unsigned long* nb_lines);

long unsigned int WINAPI camera(void* raw_info) {
    reader_type* info = (reader_type*)raw_info;

    if (info->should_continue)
        m_handler = new server_handler(context, info->server_ip.c_str(), info->port.c_str());

    unsigned long bytes;
    bool is_ok = true;
    while (info->should_continue && !m_handler->has_error()) {
        is_ok = get_bytes(&bytes);
        if (is_ok) {
            if (bytes != info->width * info->height)
                printf("%s received bytes (%d) but expected %d\n", context, bytes, info->width * info->height);
            unsigned char* image_pixels = new unsigned char[bytes];
            is_ok = get_image(image_pixels, bytes);
            display_image(image_pixels, info->width, info->height);
            delete image_pixels;
        }
    }

    delete m_handler;
    return 0;
}

void display_image(unsigned char image_pixels[], unsigned int width, unsigned int height) {
    cv::Mat cv_image(height, width, CV_8UC1, image_pixels);
    cv::namedWindow("Camera");
    cv::moveWindow("Camera", 100, 100);
    cv::imshow("Camera", cv_image);
    cvWaitKey(1);
}

bool get_image(unsigned char* image_pixels, unsigned long bytes) {
    bool is_ok = read_from_socket(m_handler, (char*)image_pixels, bytes);
    if (!is_ok)
        printf("Error while receving image\n");
    return is_ok;
}

bool get_bytes(unsigned long* bytes) {
    bool is_ok = read_from_socket(m_handler, (char*)bytes, sizeof(unsigned long));
    if (!is_ok)
        printf("Error while receving number of bytes\n");
    return is_ok;
}
