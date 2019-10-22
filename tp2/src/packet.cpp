#include "packet.h"

Packet::Packet()
{
	for (int i = 0; i < SIZE; i++)
		m_packet[i] = 0;
}

Packet::Packet(unsigned* raw)
{
	for (int i = 0; i < SIZE; i++)
		m_packet[i] = raw[i];
}


Packet::Packet(unsigned address, unsigned dir, unsigned* payload)
{
	m_packet[0] = 0;
	m_packet[1] = address;
	m_packet[2] = dir;
	for (int i = 3; i < SIZE; i++)
		m_packet[i] = payload[i-3];
}

Packet::Packet(const Packet& a_packet)
{
	m_packet[0] = a_packet.m_packet[0]+1;
	for (int i = 1; i < SIZE; i++)
		m_packet[i] = a_packet.m_packet[i];
}

Packet::~Packet()
{
}

const Packet& Packet::operator=(const Packet& old_packet)
{
	if (&old_packet != this)
	{
		m_packet[0] = old_packet.m_packet[0]+1;
		for (int i = 1; i < SIZE; i++)
			m_packet[i] = old_packet.m_packet[i];
	}
	return *this;

}

ostream& operator<< (ostream& o, const Packet& p)
{
	o << "Adresse du paquet : " << p.m_packet[1] << endl;
	o << "Direction du paquet : " << (p.m_packet[2]?"ascendant":"descendant") << endl;
	o << "Contenu du paquet : " << endl;
	for (int i = 3; i < 19; i++)
	{
		std::bitset<32> set(p.m_packet[i]);
		o << hex << set << " (" << p.m_packet[i] << ")" << endl;
	}	
	return o;
}
