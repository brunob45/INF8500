#include "server_handler.h"

#include <cstdio>

server_handler::server_handler(const char* context, const char* server_ip, const char* port) : m_context(context) {
    m_error = WSAStartup(MAKEWORD(2, 2), &m_wsa);
    if (m_error) {
        printf("%s WSAStartup failed with error: %d\n", m_context, m_error);
    }

    if (!has_error()) {
        m_error = init(server_ip, port);

        if (has_error())
            printf("%s error during initialization\n", m_context);
    }
}

server_handler::~server_handler() {
    shutdown(m_connect_socket, SD_BOTH);
    closesocket(m_listen_socket);
    closesocket(m_connect_socket);
    WSACleanup();
}

int server_handler::init(const char* server_ip, const char* port) {
    struct addrinfo hints;
    struct addrinfo* result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int error = getaddrinfo(server_ip, port, &hints, &result);
    if (error != 0) {
        printf("%s getaddrinfo failed with error: %d\n", m_context, error);
        return error;
    }

    m_listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listen_socket == INVALID_SOCKET) {
        printf("%s socket failed\n", m_context);
        freeaddrinfo(result);
        return -1;
    }

    error = bind(m_listen_socket, result->ai_addr, (int)result->ai_addrlen);
    if (error == SOCKET_ERROR) {
        printf("%s bind failed\n", m_context);
        freeaddrinfo(result);
        closesocket(m_listen_socket);
        return -1;
    }

    freeaddrinfo(result);

    error = listen(m_listen_socket, SOMAXCONN);
    if (error == SOCKET_ERROR) {
        printf("%s listen failed\n", m_context);
        closesocket(m_listen_socket);
        return 1;
    }

    m_connect_socket = accept(m_listen_socket, NULL, NULL);
    if (m_connect_socket == INVALID_SOCKET) {
        printf("%s m_connect_socket failed\n", m_context);
        closesocket(m_listen_socket);
        return 1;
    }

    closesocket(m_listen_socket);

    return 0;
}