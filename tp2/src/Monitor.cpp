#include "Monitor.h"
#include "Scoreboard.h"

simple_bus_status Monitor::read(int *data, unsigned int address)
{
	//DO NOTHING (UNIMPLEMENTED)
	return SIMPLE_BUS_ERROR;
}

simple_bus_status Monitor::write(int *data, unsigned int address)
{
    static int cpt_passage = 1;
    static unsigned int buffer[19];

    buffer[(address-768)/4] = *data;

    if (cpt_passage >= 19)
    {
        packet = Packet(buffer);
        start_check.notify();
        cpt_passage = 0;
    }
    cpt_passage ++;
    return SIMPLE_BUS_OK;
}

void Monitor::dispatch()
{
    ScoreBoard sb;

	while(1) 
	{	
		wait(); // Attente évènement start_dispatch
		std::cout << "MONITOR : paquet recu" << std::endl;
        sb.check_uint(&packet.getPacket()[3], packet.getDir());
	}

}