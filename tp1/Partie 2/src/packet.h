//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//	Nom                    : Classe Paquet
//  Fichier				   : packet.h
//  Auteurs                : Sébastien Regimbal
//                         : Olivier Benny
//  Description            
//  Cette classe modélise un paquet transigeant dans le système de routage. Le paquet est
//  caractérisé par une chaîne de 128 bits comportant 4 champs. 
//                        -----------------------------------------
//        Paquet --->    ID (32 bits) | adresse (32 bits) | payload (128 bits) |


#ifndef PACKET_H
#define PACKET_H
#include <bitset>
#include <systemc.h>
class Packet
{
public:
	// CTOR
	Packet();	
	Packet(unsigned int*);
	Packet(unsigned,unsigned int*);
	Packet(unsigned, unsigned int);
	Packet(const Packet&);
	// DTOR
	virtual ~Packet();
 
	//  Retourne le champ d'adresse du paquet

	unsigned getAddress() {return m_packet[1];}
	void setAddress( unsigned address) { m_packet[1] = address; }

	unsigned int* getPacket() { return m_packet; }

	const Packet& operator=(const Packet& old_packet);
	// Surchage de l'opérateur de sortie à l'écran
	friend ostream& operator << (ostream& o, const Packet& p);

private:


	// packet (32 bit d'id, 32 bits d'adresse, 128 bits de message)
	unsigned int m_packet[6];
	
};

#endif
