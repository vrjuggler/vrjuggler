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
#include <sys/types.h>
#include <unistd.h>
#include <SharedMem/C2Memory.h>
#include <SharedMem/SharedType.h>

void main(void)
{
    MemPool* anSgiPool = new MemPoolSGI(1024*1024);
    
    SharedType<int> a(2);
    SharedType<int> b(10);
    SharedType<int>* c = new(anSgiPool) SharedType<int>(2);
    SharedType<int>* d = new(anSgiPool) SharedType<int>(10);
    
    cout << endl << endl;
    cout << "a:" << a << "  b:" << b << "\t\tShould be: a:2  b:10\n";
    cout << "c:" << *c << "  d:" << *d << "\t\tShould be: c:2  d:10\n";

    if(fork())
    {	// Parent
	sginap(100);
	
	cout << "\n\n---- Parent ----\n";
	cout << "a:" << a << "  b:" << b << "\t\tShould be: a:3  b:12 (If it was shared)\n";
	cout << "c:" << *c << "  d:" << *d << "\t\tShould be: c:3  d:12\n";
    } else 
    {	// Child
	a = a + 1;
	b = b + 2;
	*c = *c + 1;
	*d = *d + 2;
	cout << "\n\n---- Child ----\n";
	cout << "a:" << a << "  b:" << b << "\t\tShould be: a:3  b:12 (If it was shared)\n";
	cout << "c:" << *c << "  d:" << *d << "\t\tShould be: c:3  d:12\n";
    }
    
    delete anSgiPool;
}
