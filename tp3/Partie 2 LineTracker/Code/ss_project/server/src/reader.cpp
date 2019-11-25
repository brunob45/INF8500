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
static const char* context = "reader";

typedef struct {
    unsigned int x0;
    unsigned int y0;
    unsigned int x1;
    unsigned int y1;
} line_points;

void display_lines(line_points* lines, unsigned int nb_lines, unsigned int width, unsigned int height);
bool get_lines(line_points* lines, unsigned long nb_lines);
bool get_nb_lines(unsigned long* nb_lines);
bool get_duration(double* duration);

long unsigned int WINAPI reader(void* raw_info) {
    reader_type* info = (reader_type*)raw_info;

    if (info->should_continue)
        m_handler = new server_handler(context, info->server_ip.c_str(), info->port.c_str());

    unsigned long nb_lines = 0;
    double duration = 0;
    double total_duration = 0;
    int nb_frames = 0;
    bool is_ok = true;
    while (info->should_continue && !m_handler->has_error()) {
        is_ok = get_nb_lines(&nb_lines);
        if (is_ok) {
            line_points* lines = new line_points[nb_lines];
            is_ok = get_lines(lines, nb_lines);
            display_lines(lines, nb_lines, info->width, info->height);
            delete lines;
        }
        is_ok = get_duration(&duration);

        total_duration += duration;
        nb_frames++;

        if (nb_frames % 50 == 0) {
            double average_duration = total_duration / nb_frames;
            printf("Stats for last %d frames:\n", nb_frames);
            printf("    Average duration = %f\n", average_duration);
            printf("    Average FPS = %f\n", 1.0 / average_duration);
        }
    }

    delete m_handler;
    return 0;
}

void display_lines(line_points* lines, unsigned int nb_lines, unsigned int width, unsigned int height) {
    cv::Mat image(height, width, CV_8UC3, cv::Scalar(0));

    for (int index = 0; index < nb_lines; index++) {
        line_points line = lines[index];
        cv::line(image, cv::Point(line.x0, line.y0), cv::Point(line.x1, line.y1), cv::Scalar(0, 255, 0), 1, CV_AA);
    }

    cv::namedWindow("Detected lines");
    cv::moveWindow("Detected lines", 500, 100);
    // cv::resize(image, image, cv::Size(640, 480));
    cv::imshow("Detected lines", image);
    cvWaitKey(1);
}

bool get_lines(line_points* lines, unsigned long nb_lines) {
    bool is_ok = read_from_socket(m_handler, (char*)lines, sizeof(line_points) * nb_lines);
    if (!is_ok)
        printf("Error while receving lines\n");
    return is_ok;
}

bool get_nb_lines(unsigned long* nb_lines) {
    bool is_ok = read_from_socket(m_handler, (char*)nb_lines, sizeof(unsigned long));
    if (!is_ok)
        printf("Error while receving number of lines\n");
    return is_ok;
}

bool get_duration(double* duration) {
    bool is_ok = read_from_socket(m_handler, (char*)duration, sizeof(double));
    if (!is_ok)
        printf("Error while receiving duration\n");
    return is_ok;
}
