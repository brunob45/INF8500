//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//	Nom                    : Classe Paquet
//  Fichier				   : packet.h
//  Description            
//  Cette classe modélise un paquet transigeant dans le système de routage. Le paquet est
//  caractérisé par une chaîne de 128 bits comportant 2 champs. La premier champ est 
//  l'adresse (32 bits) et le deuxième champ est le payload (96 bits).
//                        -----------------------------------------
//        Paquet --->     | adresse (32 bits) | payload (96 bits) |

#include <systemc.h>

#ifndef PACKET_H
#define PACKET_H

class Packet
{
public:
	// CTOR
	Packet();	
	Packet(unsigned, sc_biguint<64>);
	Packet(const Packet&);
	// DTOR
	virtual ~Packet();
 
	//  Retourne le champ d'adresse du paquet
	unsigned getAddress() {return m_Address;}

	
	const Packet& operator=(const Packet& old_packet);
	// Surchage de l'opérateur de sortie à l'écran
	friend ostream& operator << (ostream& o, const Packet& p);

private:
	// ID unique pour un paquet
	unsigned long m_PacketId;
	// champ adresse
	unsigned	m_Address;
	// champ payload
	sc_biguint<64> m_Payload;
	
};

#endif
