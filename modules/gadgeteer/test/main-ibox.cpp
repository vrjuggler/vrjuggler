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
#include <iomanip.h>
#include <unistd.h>

#include <SharedMem/vjMemPool.h>
//#include <SharedMem/vjSharedType.h>
#include <Input/vjInput/vjIbox.h>
#include <Input/ibox/hci.h>
#include <Input/ibox/ibox.h>

int main()
{
  vjMemPool* anSgiPool = new vjSharedPool(1024*1024);
  
  vjIBox* t1 = new(anSgiPool) vjIBox;
 
  vjIBOX_DATA *data;

  char achar;
  cout << "U - Update\n"
       << "S - Start\n"
       << "X - Stop\n"
       << "Q - Quit\n"
       << "O - Output\n";
  do {
   cin >> achar;

   cout << flush;
   switch(achar) {
     case 'u':case 'U': t1->updateData(); break;
     case 's':case 'S': t1->startSampling(); break;
     case 'x':case 'X': t1->stopSampling(); break;
     case 'o':case 'O': 
     for(int z = 0; z < 10; z++){
         t1->updateData();
         
	
	cout << t1->getDigitalData(0);
	cout << t1->getDigitalData(1);
	cout << t1->getDigitalData(2);
	cout << t1->getDigitalData(3);        
         cout << endl; 
         sleep(2);
    }
     break;
   }
   cout << achar;
  } while ((achar != 'q') && (achar != 'Q'));
  t1->stopSampling();
  delete t1;
  delete anSgiPool;
  return 0;
}
