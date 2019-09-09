#include <systemc.h>
#include "packet.h"		// importation du type Packet

#ifndef COPRO2_H
#define COPRO2_H

SC_MODULE(copro2)
{
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	/* 
	� compl�ter 
	*/

	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
	// Paquet local au module copro2
	Packet pkt;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Ce fonction re�oit un paquet de l'interconnexion et envoit le paquet au display
	void pkt_display(void); //THREAD

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	SC_CTOR(copro2)
	{
	/* 
	� compl�ter 
	*/
	}

};

#endif

