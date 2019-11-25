#include "definitions.h"
#include "server_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <windows.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

static server_handler* m_handler;
static const char* context = "sender";

int send_frame(unsigned char image[], int frame_index, sender_type* info);
int get_image(char* file, unsigned char image_pixels[], unsigned int width, unsigned int height);

long unsigned int WINAPI sender(void* raw_info) {
    sender_type* info = (sender_type*)raw_info;
    unsigned char* image = new unsigned char[info->width * info->height];

    if (info->should_continue)
        m_handler = new server_handler(context, info->server_ip.c_str(), info->port.c_str());

    int error = 0;
    while (info->should_continue && !m_handler->has_error() && !error) {
        for (int frame_index = 1; frame_index <= info->number_of_frames; frame_index++) {
            error = send_frame(image, frame_index, info);
            if (error) {
                printf("%s send_frame detected an error\n", context);
                break;
            }
        }
    }

    delete m_handler;
    return 0;
}

int send_frame(unsigned char image[], int frame_index, sender_type* info) {
    char filename[100];
    sprintf(filename, "%s\\frame_%d.bmp", info->basepath.c_str(), frame_index);

    int error = get_image(&filename[0], image, info->width, info->height);
    if (error) {
        printf("%s error while getting the image\n", context);
        return error;
    }

    for (int pixel = 0; pixel < info->width * info->height; pixel += 4) {
        record_32 packed_pixels;
        for (int index = 0; index < 4; index++)
            packed_pixels.bytes[index] = image[pixel + index];

        int bytes = send(m_handler->get_socket(), (const char*)&(packed_pixels.value), sizeof(record_32), 0);
        if (bytes < 0 || bytes != sizeof(record_32)) {
            printf("%s error while transfering the image. bytes=%d\n", context, bytes);
            closesocket(m_handler->get_socket());
            return -1;
        }
    }

    return 0;
}

int get_image(char* file, unsigned char image_pixels[], unsigned int width, unsigned int height) {
    cv::Mat image, gray_image;
    image = cv::imread(file, cv::IMREAD_COLOR);
    if (!image.data) {
        printf("No image data \n");
        return -1;
    }

    cv::cvtColor(image, gray_image, CV_BGR2GRAY);
    for (int i = 0; i < width * height; i++) {
        image_pixels[i] = gray_image.ptr()[i];
    }

    cv::namedWindow("Vision");
    cv::moveWindow("Vision", 100, 100);

    cv::Mat image_gray(height, width, CV_8UC1, image_pixels);
	// cv::resize(image_gray, image_gray, cv::Size(640, 480));
    cv::imshow("Vision", image_gray);
    cvWaitKey(1);

    return 0;
}
