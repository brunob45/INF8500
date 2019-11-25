#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>

typedef union {
    unsigned char bytes[4];
    unsigned short half_word[2];
    unsigned long value;
} record_32;

typedef struct {
    std::string server_ip;
    std::string port;
    unsigned long width;
    unsigned long height;
    unsigned long number_of_frames;
    std::string basepath;
    bool should_continue;
} sender_type;

typedef struct {
    std::string server_ip;
    std::string port;
    unsigned long width;
    unsigned long height;
    bool should_continue;
} reader_type;

#endif