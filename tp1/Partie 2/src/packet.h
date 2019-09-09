//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//	Nom                    : Classe Paquet
//  Fichier				   : packet.h
//  Auteurs                : S�bastien Regimbal
//                         : Olivier Benny
//  Description            
//  Cette classe mod�lise un paquet transigeant dans le syst�me de routage. Le paquet est
//  caract�ris� par une cha�ne de 128 bits comportant 4 champs. 
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
	// Surchage de l'op�rateur de sortie � l'�cran
	friend ostream& operator << (ostream& o, const Packet& p);

private:


	// packet (32 bit d'id, 32 bits d'adresse, 128 bits de message)
	unsigned int m_packet[6];
	
};

#endif
