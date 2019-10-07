/*
 * testcase.h
 *
 *  Created on: 5-Jan-2015
 *      Author: Srinivasa
 */

// A été concu pour plusieurs types, on va utilser seulement int


#include "include.h"
//use templeted class
class ScoreBoard {
 public:
  void check_int(int * a ,int * b, bool direction);
  void check_char(char * a ,char * b, bool direction);
  void check_lgint(long int * a , long int * b, bool direction);
};

void ScoreBoard::check_int(int * a ,int * b, bool direction)
{
 std::cout<<"Score board called"<<std::endl;
 for(int i = 0;i<MAX;i++)
  {
    bool chk = false;
    for(int j = 0;j<MAX;j++) 
    {
     if(*(b+i) == *(a+j) ) chk = true;
    }
    if(!chk) std::cout<<"ScoreBoard ERROR: The array Data Miss Match "<<*(b+i)<<std::endl;
    //else std::cout<<"Data Present in the array  "<< *(b+i)<<std::endl;
    assert(chk);
    if(MAX != i+1 )
    {
      if(!direction) 
       {
        if(*(a+i) < *(a+i+1)) std::cout<<"ScoreBoard ERROR: The Expected Result Not in Descending Order"<<std::endl;
        assert(*(a+i) >= *(a+i+1));
       }
      else
      {
       if(*(a+i) > *(a+i+1)) std::cout<<"ScoreBoard ERROR: The Expected Result Not in Ascending Order"<<std::endl;
       assert(*(a+i) <= *(a+i+1));
      }
    }
  }
 if(direction) std::cout<<"ScoreBoard Info : The Expected Result in Ascending Order"<<std::endl;
 else std::cout<<"ScoreBoard Info : The Expected Result in Descending Order"<<std::endl;

}

void ScoreBoard::check_char(char * a ,char * b, bool direction){

std::cout<<"Score board called"<<std::endl;
 for(int i = 0;i<MAX;i++)
  {
    bool chk = false;
    for(int j = 0;j<MAX;j++) 
    {
     if(*(b+i) == *(a+j) ) chk = true;
    }
    if(!chk) std::cout<<"ScoreBoard ERROR: The array Data Miss Match "<<*(b+i)<<std::endl;
    //else std::cout<<"Data Present in the array  "<< *(b+i)<<std::endl;
    assert(chk);
    chk = false;
    if(MAX != i+1 )
    {
      if(!direction) 
       {
        if(*(a+i) < *(a+i+1)) std::cout<<"ScoreBoard ERROR: The Expected Result Not in Descending Order"<<std::endl;
        assert(*(a+i) >= *(a+i+1));
       }
      else
      {
       if(*(a+i) > *(a+i+1)) std::cout<<"ScoreBoard ERROR: The Expected Result Not in Ascending Order"<<std::endl;
       assert(*(a+i) <= *(a+i+1));
      }
    }
  }
 if(direction) std::cout<<"ScoreBoard Info : The Expected Result in Ascending Order"<<std::endl;
 else std::cout<<"ScoreBoard Info : The Expected Result in Descending Order"<<std::endl;

}

void ScoreBoard::check_lgint(long int * a ,long int * b, bool direction){

std::cout<<"Score board called"<<std::endl;
 for(int i = 0;i<MAX;i++)
  {
    bool chk = false;
    for(int j = 0;j<MAX;j++) 
    {
     if(*(b+i) == *(a+j) ) chk = true;
    }
    if(!chk) std::cout<<"ScoreBoard ERROR: The array Data Miss Match "<<*(b+i)<<std::endl;
    //else std::cout<<"Data Present in the array  "<< *(b+i)<<std::endl;
    assert(chk);
    chk = false;
    if(MAX != i+1 )
    {
      if(!direction) 
       {
        if(*(a+i) < *(a+i+1)) std::cout<<"ScoreBoard ERROR: The Expected Result Not in Descending Order"<<std::endl;
        assert(*(a+i) >= *(a+i+1));
       }
      else
      {
       if(*(a+i) > *(a+i+1)) std::cout<<"ScoreBoard ERROR: The Expected Result Not in Ascending Order"<<std::endl;
       assert(*(a+i) <= *(a+i+1));
      }
    }
  }

 if(direction) std::cout<<"ScoreBoard Info : The Expected Result in Ascending Order"<<std::endl;
 else std::cout<<"ScoreBoard Info : The Expected Result in Descending Order"<<std::endl;

}

