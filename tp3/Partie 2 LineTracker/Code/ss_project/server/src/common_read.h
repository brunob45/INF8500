#ifndef COMMON_READ_H
#define COMMON_READ_H

#include "server_handler.h"

bool read_from_socket(server_handler* handler, char* data, unsigned long bytes);

#endif