#include "packet.h"

Packet::Packet()
{
	m_packet[1] = 0;
	for (int i = 2; i < 19; i++)
		m_packet[i] = 0;
}

Packet::Packet(unsigned int* p)
{

	for (int i = 0; i < 19; i++)
		m_packet[i] = *(p+i);
}

Packet::Packet(unsigned a, unsigned int* p)
{
       
	m_packet[1] = a;
	for (int i = 2; i < 19; i++)
		m_packet[i] = (*(p+i-2)*i);
}

Packet::Packet(unsigned a, unsigned int p)
{
	m_packet[1] = a;
	for (int i = 2; i < 19; i++)
		m_packet[i] = (p*i) * 3500 % 3001 *57;
}
Packet::Packet(const Packet& a_packet)
{
	m_packet[1] = a_packet.m_packet[1];
	for (int i = 2; i < 19; i++)
		m_packet[i] = a_packet.m_packet[i];
}

Packet::~Packet()
{
}

const Packet& Packet::operator=(const Packet& old_packet)
{
	if ( &old_packet != this)
	{
		m_packet[1] = old_packet.m_packet[1];
		for (int i = 2; i < 19; i++)
			m_packet[i] = old_packet.m_packet[i];
	}
	return *this;

}




ostream& operator<< (ostream& o, const Packet& p)
{
	o << "Adresse du paquet : " << p.m_packet[1] << endl
     << "Contenu du paquet : " << endl;
	for (int i = 2; i < 19; i++)
	{
		std::bitset<32> set(p.m_packet[i]);
		o << hex << set << " (" << p.m_packet[i] << ")" << endl;
	}	
	return o;
}
