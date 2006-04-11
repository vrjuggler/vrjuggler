/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>
#include <iomanip>
#include <gadget/Devices/Logitech/logiclass.h>
#include <vpr/System.h>

int main()
{
  ThreeDMouse* t1 = new ThreeDMouse;

  gadget::POS_DATA *data;

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
     case 'u':case 'U': t1->updateData(); break;
     case 's':case 'S': t1->startSampling(); break;
     case 'x':case 'X': t1->stopSampling(); break;
     case 'o':case 'O':
     for(int z = 0; z < 10; z++){
         t1->updateData();
         t1->getPosData(data);

         std::cout << std::endl
                   << "Data: x:" << std::setw(10) << data->pos.vec[0] << std::endl
                   << "      y:" << std::setw(10) << data->pos.vec[1] << std::endl
                   << "      z:" << std::setw(10) << data->pos.vec[2] << std::endl
                   << "    azi:" << std::setw(10) << data->orient.vec[0] << std::endl //lr
                   << "   elev:" << std::setw(10) << data->orient.vec[1] << std::endl //ud
                   << "   roll:" << std::setw(10) <<data->orient.vec[2] << std::endl
                   << std::endl;
         std::cout << std::endl;
         vpr::System::sleep(2);
    }
     break;
   }
   std::cout << achar;
  } while ((achar != 'q') && (achar != 'Q'));

  t1->stopSampling();

  delete t1;
  return 0;
}
