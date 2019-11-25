#include "arguments.h"
#include "definitions.h"

#include <cstdio>
#include <iostream>
#include <windows.h>

HANDLE m_threads[3];
sender_type sender_info;
reader_type reader_info;
reader_type camera_info;

long unsigned int WINAPI sender(void*);
long unsigned int WINAPI reader(void*);
long unsigned int WINAPI camera(void*);
void create_thread(HANDLE* thread, long unsigned int(WINAPI* start_routine)(void*), void* data);
BOOL WINAPI console_handler(DWORD);

int main(int argc, char* argv[]) {
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)console_handler, TRUE)) {
        fprintf(stderr, "Unable to install handler!\n");
        return EXIT_FAILURE;
    }

    arguments argument_parser(argc, argv);
    if (argument_parser.has_error()) {
        printf("%s", argument_parser.get_error().c_str());
        return 1;
    }

    printf("TCP/IP server for LineTracker running in %s mode\n", argument_parser.is_simulation() ? "simulation" : "implementation");

    sender_info = {argument_parser.get_ip(), "5015", 320, 240, 20, argument_parser.get_vision_path(), argument_parser.is_simulation()};
    reader_info = {argument_parser.get_ip(), "5016", 320, 240, true};
    camera_info = {argument_parser.get_ip(), "5017", 320 / 2, 240 / 2, !argument_parser.is_simulation()};

    create_thread(&m_threads[0], &sender, (void*)&sender_info);
    create_thread(&m_threads[1], &reader, (void*)&reader_info);
    create_thread(&m_threads[2], &camera, (void*)&camera_info);

    std::cout << "Press enter at any point to shutdown the server" << std::endl;
    std::cin.get();

    return 0;
}

void create_thread(HANDLE* thread, long unsigned int(WINAPI* start_routine)(void*), void* data) {
    unsigned long descriptor;
    *thread = CreateThread(NULL, 0, start_routine, data, 0, &descriptor);

    if (thread == NULL) {
        std::cout << "error while creating thread" << std::endl;
    }
}

BOOL WINAPI console_handler(DWORD type) {
    switch (type) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
        sender_info.should_continue = false;
        reader_info.should_continue = false;
        camera_info.should_continue = false;
        CloseHandle(m_threads[0]);
        CloseHandle(m_threads[1]);
        CloseHandle(m_threads[3]);
        break;
    default:
        break;
    }
    return TRUE;
}