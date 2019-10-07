/*
 * Sort.h
 *
 *  Created on: 26-Dec-2014
 *      Author: gaurav
 */

#ifndef SORT_H_
#define SORT_H_
#include "include.h"


	template <class TypeA> class cSort
	{
		protected:
			TypeA			*m_pdata;
			unsigned long 	m_size;
		public:
			cSort(TypeA *pdata, unsigned long size)	:m_pdata(pdata), m_size(size){}
			virtual ~cSort(){}
			virtual int doSort(bool inAscending=true)=0;
			virtual void print()
			{
				unsigned long i=0;
				for(i=0; i<this->m_size;i++)
				{
					std::cout<<this->m_pdata[i]<<"\t";
				}
				std::cout<<std::endl;
			}
 
     virtual TypeA *get_pointer_to_sorted_array(){return this->m_pdata;}

	};



#endif /* SORT_H_ */
