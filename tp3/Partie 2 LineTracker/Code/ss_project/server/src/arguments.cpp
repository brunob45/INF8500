#include "arguments.h"

#include <cstdio>

arguments::arguments(int argc, char* argv[]) {
	char buff[150];
	snprintf(buff, sizeof(buff), "Syntax: %s [SERVER_IP] [IS_SIMULATION? true or false] [PATH TO ROBOT VISION FILES]", argv[0]);
    if (argc < 3) {
        m_error_message = buff;
    }
    if (argc >= 3) {
        m_ip = argv[1];
        m_is_simulation = strcmp(argv[2], "true") == 0;
		
		if (m_is_simulation) {
			if (argc == 4) {
				m_vision_path = argv[3];
			} else {
				m_error_message = buff;
			}
		} else if (argc > 3) {
			m_error_message = buff;
		}
    }
}