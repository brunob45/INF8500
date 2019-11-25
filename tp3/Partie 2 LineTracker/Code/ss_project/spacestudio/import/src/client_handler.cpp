#include "client_handler.h"

#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#define SHUT_RDWR SD_BOTH
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#include <cstring>
#include <cstdio>

client_handler::client_handler(const char* context, const char* target_hostname, const char* target_port)
:m_context(context) {
#ifdef _WIN32
	WSADATA wsa;
	int error = WSAStartup(MAKEWORD(2,2), &wsa);
	if (error) {
		printf("%s WSAStartup failed with error: %d\n", m_context, m_error);
	}
#endif
	struct addrinfo hints, *result;

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	m_error = getaddrinfo(target_hostname, target_port, &hints, &result);
	if (m_error) {
		printf("%s getaddrinfo failed with error: %d\n", context, m_error);
		return;
	}

	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_socket == -1) {
		m_error = -1;
		printf("%s failed to open socket\n", context);
		freeaddrinfo(result);
		return;
	}

	m_error = connect(m_socket, result->ai_addr, (int)result->ai_addrlen);
	if (m_error == -1) {
	    close(m_socket);
		printf("%s is unable to connect to server!\n", context);
		return;
	}
	freeaddrinfo(result);
}

client_handler::~client_handler() {
	shutdown(m_socket, SHUT_RDWR);
	close(m_socket);
}
