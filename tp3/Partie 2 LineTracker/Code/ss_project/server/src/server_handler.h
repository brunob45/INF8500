#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WIN32_WINNT 0x0A00 

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

class server_handler {
  public:
    server_handler(const char* context, const char* server_ip, const char* port);
    ~server_handler();

    inline SOCKET get_socket() { return m_connect_socket; }
    inline bool has_error() { return m_error; }

  private:
    int init(const char* server_ip, const char* port);

    SOCKET m_connect_socket;
    SOCKET m_listen_socket;
    WSADATA m_wsa;
    const char* m_context;
    int m_error;
};

#endif