/* -*- sysc -*-*/
/*
 * Copyright (C) 2011 XILINX, Inc. 
 *
 *
 */

#ifndef _AP_BUS_IF
#define _AP_BUS_IF

#define addr_type unsigned

template<typename _VHLS_DT>
class hls_bus_if : public sc_interface
{
public:
   hls_bus_if(const char* name_ = ""){
   }

   virtual _VHLS_DT& read(addr_type addr)=0;
   virtual void read(addr_type addr, _VHLS_DT *data)=0;
   virtual void burst_read(addr_type addr, int size, _VHLS_DT *data)=0;
   virtual void write(addr_type addr, _VHLS_DT *data)=0;
   virtual void burst_write(addr_type addr, int size, _VHLS_DT *data)=0;
};


//----------------------------------------------------------
// hls_bus_port
//----------------------------------------------------------
template<typename _VHLS_DT>
class hls_bus_port : public sc_port<hls_bus_if<_VHLS_DT> >
{
    // typedefs
    //typedef _VHLS_DT                                             _VHLS_DT;
    typedef hls_bus_if<_VHLS_DT>       if_type;
    typedef sc_port<if_type, 1,SC_ONE_OR_MORE_BOUND>      base_type;
    typedef hls_bus_port<_VHLS_DT>     this_type;

    typedef if_type                                       in_if_type;
    typedef base_type                                     in_port_type;

public:

    hls_bus_port() {
     #ifndef __RTL_SIMULATION__
        cout<<"@W [SIM] Please add name for your hls_bus_port, or RTL simulation will fail."<<endl; 
     #endif
    }
    explicit hls_bus_port( const char* name_ ) { }

    void reset() {}

    _VHLS_DT &read(addr_type addr) {
       return (*this)->read(addr);
    }

    void burst_read(addr_type addr, int size, _VHLS_DT *data) {
       (*this)->burst_read(addr, size, data);
    }

    void write(addr_type addr, _VHLS_DT *data) {
        (*this)->write(addr, data);
    }

    void burst_write(addr_type addr, int size, _VHLS_DT *data) {
       (*this)->burst_write(addr, size, data);
    }
};


//-----------------------------
// bus channel
//-----------------------------
template <typename data_type>
class hls_bus_chn
  :public hls_bus_if<data_type>
  ,public sc_module
{
  private:
    //data_type  mem[END_ADDR - START_ADDR];
    data_type  *mem;
    unsigned int m_start_addr;
    unsigned int m_end_addr;
    std::string name;

  public:
    SC_HAS_PROCESS(hls_bus_chn);
    hls_bus_chn(sc_module_name _name
              ,unsigned int start_addr = 0
              ,unsigned int end_addr = 1024)
    : sc_module(_name)
    , name(_name)
    , m_start_addr(start_addr)
    , m_end_addr(end_addr)
    {
       sc_assert(m_start_addr <= m_end_addr);

       unsigned int size = (m_end_addr-m_start_addr+1);

       mem = new data_type [size];
       for (unsigned int i = 0; i < size; ++i)
         mem[i] = i;
    }

    ///
    /// bus read
    ///
    data_type& read(addr_type addr)
    {
    #ifdef DUMP_BUS_OP
        cout <<"[bus wraper] "<<name<<": read mem["<<addr<<"] = "<< mem[addr] <<endl;
    #endif
        return mem[addr];
    }

    void read(addr_type addr, data_type *data)
    {
        *data = mem[addr];
    #ifdef DUMP_BUS_OP
        cout <<"[bus wraper] "<<name<<": read mem["<<addr<<"] = "<< mem[addr] <<endl;
    #endif
    }

    ///
    /// burst read
    ///
    void burst_read(addr_type addr, int size, data_type *data)
    {
       for(int i=0; i<size; i++)
       {
          data[i] = mem[addr+i];
    #ifdef DUMP_BUS_OP
          cout<<"[bus wraper] "<< name <<": read mem["<<addr+i<<"] = "<< data[i] <<endl;
    #endif
       }
    }

    ///
    /// bus write
    ///
    void write(addr_type addr, data_type *data)
    {
        mem[addr] = *data;
    #ifdef DUMP_BUS_OP
        cout <<"[bus wraper] "<< name <<": write mem["<<addr<<"] = "<< mem[addr] <<endl;
    #endif
    }

    ///
    /// burst write
    ///
    void burst_write(addr_type addr, int size, data_type *data)
    {
       for(int i=0; i<size; i++)
       {
          mem[addr+i] = data[i];
    #ifdef DUMP_BUS_OP
          cout <<"[bus wraper] "<<name<<": write mem["<<addr+i<<"] = "<< mem[addr] <<endl;
    #endif
       }
    }
};

#undef addr_type
#endif
