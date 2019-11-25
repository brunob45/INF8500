#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

class client_handler {
	public:
		client_handler(const char* context, const char* target_hostname, const char* target_port);
		~client_handler();
		
		inline int get_socket() { return m_socket; }
		inline bool has_error() { return m_error; }
		
	private:
	
		int m_socket;
		const char* m_context;
		int m_error;	
};

#endif