#ifndef PACKET_GEN_ADAPT_H
#define PACKET_GEN_ADAPT_H

#include <systemc.h>
// importation du type Packet
#include "packet.h"	
#include "simple_bus_types.h"
#include "simple_bus_blocking_if.h"

SC_MODULE(packet_gen_adapt_master)
{
    /* *******************************************************************
    // MODULE PORTS
    ******************************************************************** */
    sc_in_clk clock;
    sc_port<simple_bus_blocking_if> bus_port;
    
    // signal indiquant que le routeur est prêt à recevoir un nouveau paquet
    sc_out<bool> next_packet;
    // Sortie indiquant qu'un paquet est prêt pour le routeur
    sc_in<bool> packet_ready;
    // Sortie du paquet à transmettre au routeur
    sc_in<Packet*> packet_in;

    /* *******************************************************************
    // LOCAL VARIABLES
    ******************************************************************** */
    // Paquet local au module pkt_terminator
    Packet pkt;

    /* *******************************************************************
    // MODULE METHODS
    ******************************************************************** */
    
    SC_HAS_PROCESS(packet_gen_adapt_master);
    // Ce thread reçoit un paquet du générateur, l'identifie et l'envoie à l'adaptateur du coprocesseur
    void pkt_dispatch(void); //THREAD

    /* *******************************************************************
    // MODULE CONSTRUCTOR
    ******************************************************************** */
    packet_gen_adapt_master(sc_module_name name_
        , unsigned int unique_priority
        , unsigned int address
        , bool lock
        , int timeout)
        : sc_module(name_)
        , m_unique_priority(unique_priority)
        , m_address(address)
        , m_lock(lock)
        , m_timeout(timeout)
    {
        // process declaration
        SC_THREAD(pkt_dispatch);
        sensitive << clock.pos();
    }
private:
    unsigned int m_unique_priority;
    unsigned int m_address;
    bool m_lock;
    int m_timeout;
};

#endif

