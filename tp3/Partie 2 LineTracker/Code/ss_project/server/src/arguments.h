#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <cstring>
#include <string>

class arguments {
  public:
    arguments(int argc, char* argv[]);
    ~arguments() {}

    inline std::string get_ip() { return m_ip; }
    inline bool is_simulation() { return m_is_simulation; }
	inline std::string get_vision_path() { return m_vision_path; }
    inline std::string get_error() { return m_error_message; }
    inline bool has_error() { return !m_error_message.empty(); }

  private:
    std::string m_ip;
    bool m_is_simulation;
	std::string m_vision_path;
    std::string m_error_message;
};

#endif