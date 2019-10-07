//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//	Nom                    : Classe Paquet
//  Fichier		       : packet.h
//  Description            
//  Cette classe modélise un paquet transigeant dans le système de routage. Le paquet est
//  caractérisé par une chaîne de 19 mots de 32 bits: 
//                        -----------------------------------------
//        Paquet --->    ID (32 bits) | adresse (32 bits) | direction du tri (32 bits) | payload (512 bits) |


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
	unsigned int getDir() { return m_packet[2]; }


	void putPacket(unsigned int* newpayload) {
		for (int i = 3; i < 19; i++)
			m_packet[i] = newpayload[i-3];
		}

	const Packet& operator=(const Packet& old_packet);
	// Surchage de l'opérateur de sortie à l'écran
	friend ostream& operator << (ostream& o, const Packet& p);

private:


	// packet 
	unsigned int m_packet[19];
	
};

#endif
