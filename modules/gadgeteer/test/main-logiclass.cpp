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

#include <iostream>
#include <iomanip>
#include <SharedMem/vjMemPool.h>
//#include <SharedMem/vjSharedType.h>
#include <Input/vjPosition/logiclass.h>
//#include <Input/ibox/ibox.h>

int main()
{
  vjMemPool* anSgiPool = new vjSharedPool(1024*1024);
  
  vjThreeDMouse* t1 = new(anSgiPool) vjThreeDMouse;

  vjPOS_DATA *data;

  char achar;
  std::cout << "U - Update\n"
            << "S - Start\n"
            << "X - Stop\n"
            << "Q - Quit\n"
            << "O - Output\n";
  do {
   std::cin >> achar;

   std::cout << std::flush;
   switch(achar) {
     case 'u':case 'U': t1->UpdateData(); break;
     case 's':case 'S': t1->StartSampling(); break;
     case 'x':case 'X': t1->StopSampling(); break;
     case 'o':case 'O': 
     for(int z = 0; z < 10; z++){
         t1->UpdateData();
         t1->GetData(data);

         std::cout << std::endl 
                   << "Data: x:" << std::setw(10) << data->pos.vec[0] << std::endl
                   << "      y:" << std::setw(10) << data->pos.vec[1] << std::endl
                   << "      z:" << std::setw(10) << data->pos.vec[2] << std::endl
                   << "    azi:" << std::setw(10) << data->orient.vec[0] << std::endl //lr
                   << "   elev:" << std::setw(10) << data->orient.vec[1] << std::endl //ud
                   << "   roll:" << std::setw(10) <<data->orient.vec[2] << std::endl
                   << std::endl;
         std::cout << std::endl;
         sleep(2);
    }
     break;
   }
   std::cout << achar;
  } while ((achar != 'q') && (achar != 'Q'));
  
  t1->StopSampling();
  
  delete t1;
  delete anSgiPool;
  return 0;
}
