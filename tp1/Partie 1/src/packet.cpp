#include "packet.h"

Packet::Packet()
{
	m_Address = 0;
	m_Payload = 0;
}


Packet::Packet(	unsigned a, sc_biguint<64> p )
{
       
	m_Address = a;
	m_Payload = p;
}

Packet::Packet(const Packet& a_packet)
{
	m_Address = a_packet.m_Address;
	m_Payload = a_packet.m_Payload;
}

Packet::~Packet()
{
}

const Packet& Packet::operator=(const Packet& old_packet)
{
	if ( &old_packet != this)
	{
	    
		m_Address = old_packet.m_Address;
		m_Payload = old_packet.m_Payload;
	}
	return *this;

}




ostream& operator<< (ostream& o, const Packet& p)
{
	o << "Adresse du paquet : " << p.m_Address << endl
		<< "Contenu du paquet : " << p.m_Payload << endl;
	return o;
}
