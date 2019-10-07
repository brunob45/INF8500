/*
 * testcase.h
 *
 *  Created on: 31-Dec-2014
 *      Author: Srinivasa
 *      Adapté par Guy Bois en septembre 2019
 */



//#include "include.h"
#include <math.h>
#include <iostream>
#include <string.h>

#include <crave/ConstrainedRandom.hpp>
using crave::Generator;
using crave::distribution;
using crave::dist;
using crave::range;
using crave::rand_obj;
using crave::randv;
using crave::rand_vec;
using crave::_i;
using crave::reference;

////////////////////////////////////////////////////////////////////////////////////
/////////////  Testcase To Generate Data in Ascending Order   //////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Vous devez améliorer ce code


class Test_Random_Asc_data_gen: public rand_obj {
 public:
  randv<uint>  direction;
  Test_Random_Asc_data_gen(rand_obj* parent = 0): rand_obj(parent), direction(this)
  {
//   direction.addRange(0, 1);
  constraint(dist(direction(), distribution<uint>::create(range<uint>(0, 1))));
  }
  int iarray[MAX];
  int * runphase(int a);
};

int * Test_Random_Asc_data_gen::runphase(int a){
  randv<int> x = NULL,y=NULL ,z=NULL;
  Generator gen;
  static unsigned int n = 0;
  gen(x() >=1000  && x()<= 3000 );
  gen(y() >=100  && y()<= 500 );
  gen(z() >=10  && z()<= 100 );
  
  for(int i = 0;i<MAX;i++)
  {
    assert(gen.next());
    if(n<80) n = n+i*2;
    else if(n<512) n = n+z;
    else if(n<2048) n = n+y;
    else n = n+x;
    iarray[i] = n;
  }
  return iarray;
}


///////////////////////////////////////////////////////////////////////////////////////////
///////        Testcase To Generate Data in                ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class Test_Rand_Full_data_gen : public rand_obj
{
public:
  randv<uint>  direction;
  Test_Rand_Full_data_gen(rand_obj* parent = 0): rand_obj(parent), direction(this)
  {
//   direction.addRange(0, 1);
  constraint(dist(direction(), distribution<uint>::create(range<uint>(0, 1))));
  }
  int iarray[MAX];
  int * runphase(int a);
};


int * Test_Rand_Full_data_gen::runphase(int a){
  randv<int> x = NULL;
  Generator gen;
  gen(x()>=1 && x()<=65535);

  for(int i = 0;i<MAX;i++){
    gen.next();
    iarray[i] = x;
  }
  return iarray;
  
}



///////////////////////////////////////////////////////////////////////////////////////////
///////  Testcase To Generate Data in Descending Order         ////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Vous devez améliorer ce code

class Test_Random_Dsc_data_gen: public rand_obj {
 public:
  randv<uint>  direction;
  Test_Random_Dsc_data_gen(rand_obj* parent = 0): rand_obj(parent), direction(this)
  {
//   direction.addRange(0, 1);
  constraint(dist(direction(), distribution<uint>::create(range<uint>(0, 1))));
  }
  int iarray[MAX];
  int * runphase(int a);

};

int * Test_Random_Dsc_data_gen::runphase(int a){
  randv<int> x = NULL,y=NULL ,z=NULL;
  Generator gen;
  static unsigned int n = 65535,j=0;
  gen(x() >=0  && x()<= 2000 );
  gen(y() >=0  && y()<= 100 );
  gen(z() >=0  && z()<= 10 );
  
  for(int i = 0;i<MAX;i++)
  {
   assert(gen.next());
   if(n>2000) n= n-x;
   else if(n>200) n = n-y;
   else n = n - z;
   iarray[i] = n;
  }
  return iarray;
}


//////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Testcase To Generate Continues number in Asc Order//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


class Test_Continues_Asc_data_gen : public rand_obj
{
public:
  randv<uint>  direction;
  Test_Continues_Asc_data_gen(rand_obj* parent = 0): rand_obj(parent), direction(this)
  {
//   direction.addRange(0, 1);
  constraint(dist(direction(), distribution<uint>::create(range<uint>(0, 1))));
  }
  int iarray[MAX];
  int * runphase(int a);

};

int * Test_Continues_Asc_data_gen::runphase(int a){
  randv<int> x = NULL;
  Generator gen;
  gen(x()>=1 && x()<=65535);
  gen.next();

  for(int i = 0;i<MAX;i++)
  {
    iarray[i] = x+i;
  }
  return iarray;
}


//////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Testcase To Generate Continues number in Descending Order//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


class Test_Continues_Desc_data_gen : public rand_obj
{
public:
  randv<uint>  direction;
  Test_Continues_Desc_data_gen(rand_obj* parent = 0): rand_obj(parent), direction(this)
  {
//   direction.addRange(0, 1);
  constraint(dist(direction(), distribution<uint>::create(range<uint>(0, 1))));
  }
  int iarray[MAX];
  int * runphase(int a);

};

int * Test_Continues_Desc_data_gen::runphase(int a){
  randv<int> x = NULL;
  Generator gen;
  gen(x()>=1 && x()<=65535);
  gen.next();

  for(int i = 0;i<MAX;i++)
  {
    iarray[i] = i-x;
  }
  return iarray;
}
/////////////////////////////////////////////////////////////////////////////////////
///////            Register All Test cases Here /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

class TestBase {

public: 
  int iarray[MAX];
  bool direction;
  int * p ;

  int * runphase(int a);
  void chk_testcase(int arg, std::string str);
};

void TestBase::chk_testcase(int argc,std::string str){

 if(argc <= 2) 
  { 
   std::cout<<"Please Specify Testcase name : argv[2] is Null"<<std::endl; 
   assert(0);
  }
 if(str.compare("Full_Random") == 0)
  {
   Test_Rand_Full_data_gen * c;
   c = new Test_Rand_Full_data_gen;
   c->next();
   p = c->runphase(20);
   direction = c->direction; 
  }
  else if(str.compare("Radom_Asc") == 0)
  {
   Test_Random_Asc_data_gen * c;
   c = new Test_Random_Asc_data_gen;
   c->next();
   direction = c->direction; 
   p = c->runphase(20);
  }
  else if(str.compare("Random_Dsc") == 0)
  {
   Test_Random_Dsc_data_gen * c;
   c = new Test_Random_Dsc_data_gen;
   c->next();
   direction = c->direction; 
   p = c->runphase(20);
   }
  else if(str.compare("Continues_Desc") == 0)
  {
   Test_Continues_Desc_data_gen * c;
   c = new Test_Continues_Desc_data_gen;
   c->next();
   direction = c->direction; 
   p = c->runphase(20);
  }
else if(str.compare("Continues_Asc") == 0)
  {
   Test_Continues_Asc_data_gen * c;
   c = new Test_Continues_Asc_data_gen;
   c->next();
   direction = c->direction;
   p = c->runphase(20);
  }
  else {
   std::cout<<"ERROR Wrong Testcase Name"<<std::endl;
   assert(0);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
