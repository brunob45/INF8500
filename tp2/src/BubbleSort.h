/*
 * BubbleSort.h
 *
 *  Created on: 26-Dec-2014
 *      Author: gaurav
 */

#ifndef BUBBLESORT_H_
#define BUBBLESORT_H_

#include "include.h"
#include "Sort.h"

	template <class TypeA> class cBubbleSort : public cSort<TypeA>
	{
//		protected:
//			TypeA			*m_pdata;
//			unsigned long	m_size;
		public:
			cBubbleSort(TypeA *pdata, unsigned long size) : cSort<TypeA>(pdata, size){}
			virtual ~cBubbleSort(){};
			int doSort(bool inAscending=true)
			{
				int retval = SORT_NO_ERROR;
				unsigned long i=0, limit=0;
				limit = this->m_size - 1;
				bool cont = false;
				while (1)
				{
					for (i=0; i < limit; i++)
					{
						if ( swap(this->m_pdata[i], this->m_pdata[i+1], inAscending) == true)
						{
							cont = true;
							TypeA temp;
							temp = this->m_pdata[i];
							this->m_pdata[i] = this->m_pdata[i+1];
							this->m_pdata[i+1] = temp;
						}
					}
					if (limit <= 0)
					{
						break;
					}

					limit--;
					if ((cont == false))
					{
						break;
					}
				}
				return retval;
			}
		private:
			bool swap(TypeA dataA, TypeA dataB, bool isAscending=true)
			{
				if (isAscending == true)
				{
					if (dataA > dataB)
					{
						return true;
					}
				}
				else
				{
					if (dataA < dataB)
					{
						return true;
					}
				}
				return false;
			}
	};


#endif /* BUBBLESORT_H_ */
