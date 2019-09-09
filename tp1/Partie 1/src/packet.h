//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//	Nom                    : Classe Paquet
//  Fichier				   : packet.h
//  Description            
//  Cette classe mod�lise un paquet transigeant dans le syst�me de routage. Le paquet est
//  caract�ris� par une cha�ne de 128 bits comportant 2 champs. La premier champ est 
//  l'adresse (32 bits) et le deuxi�me champ est le payload (96 bits).
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
	// Surchage de l'op�rateur de sortie � l'�cran
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
