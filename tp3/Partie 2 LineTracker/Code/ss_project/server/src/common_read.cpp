#include "common_read.h"

#include <windows.h>

bool read_from_socket(server_handler* handler, char* data, unsigned long bytes) {
    unsigned long remaining_bytes = bytes;
    unsigned long offset = 0;

    while (remaining_bytes > 0) {
        int bytes_received = recv(handler->get_socket(), &data[offset], remaining_bytes, 0);
        if (bytes_received == 0) {
            closesocket(handler->get_socket());
            return false;
        }
        remaining_bytes -= bytes_received;
        offset += bytes_received;
    }
    return true;
}