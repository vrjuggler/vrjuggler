/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 */

#include <iostream.h>
#include <vector>
#include <SharedMem/MemPool_alloc.h>

template<class T, class A>
void printVector(std::vector<T, A>& list);

int main(void)
{
    std::vector<int, mempool_alloc> intList;
    
    for(int i=0;i<20;i++)
	intList.push_back(i);
	
    printVector(intList);
	
    if(fork())
    {	// Parent
	sginap(100);
	
	cout << "\n\n---- Parent ----\n";
	printVector(intList);
    }
    else
    {	// Child
    
	cout << "\n\n---- Child ----\n";
	
	intList[5] = 69;
	intList[10] = 71;
	
	//for(i=0;i<15;i++)
	//    intList.pop_back();
	    
	printVector(intList);
    }
}


template<class T, class A>
void printVector(std::vector<T, A>& list)
{
    for(int i=0;i<list.size();i++)
	cout << list[i] << ", "; 
    cout << endl << flush;
}
