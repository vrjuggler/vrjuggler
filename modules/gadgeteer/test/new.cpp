/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
#include <VPR/SharedMem/vjMemPool.h>
//#include <VPR/SharedMem/SharedType.h>
#include <Input/vjPosition/vjFlock.h>
#include <Input/ibox/hci.h>
#include <Input/ibox/ibox.h>
#include <Input/vjInput/vjIbox.h>
#include <Input/vjGlove/vjCyberGlove.h>
#include <vpr/System.h>

int main()
{
  MemPool* anSgiPool = new SharedPool(1024*1024);
  
  Flock* flock = new(anSgiPool) Flock;
  IBox* anIbox = new(anSgiPool) IBox;
  CyberGlove* aGlove = new(anSgiPool) 
          CyberGlove("/home/vr/CAVE/glove","/dev/ttyd45",38400);

  POS_DATA *data,*data2;
 
  IBOX_DATA *iboxData;

  char achar;
  std::cout << "U - Update\n"
            << "S - Start\n"
            << "X - Stop\n"
            << "Q - Quit\n"
            << "O - Output\n";
  do {
   std::cin >> achar;

   std::cout << flush;
   switch(achar) {
     case 'u':case 'U':  break;
     case 's':case 'S': flock->StartSampling(); 
			anIbox->StartSampling();
			aGlove->StartSampling();
			break;
     case 'x':case 'X': flock->StopSampling(); 
			anIbox->StopSampling();	
			aGlove->StopSampling();
			break;
     case 'o':case 'O': 
     for(int z = 0; z < 10; z++){
      C2GLOVE_DATA *gdata;
         flock->UpdateData();
         flock->GetData(data);
	 flock->GetData(data2);
         anIbox->UpdateData();
	 anIbox->GetData(iboxData);
	// aGlove->UpdateData();
//	 aGlove->GetData(gdata);
       system("clear");  
     std::cout << "vjFlock------------------------------------------------------------"
               << std::endl 
               << "Data: x:" << std::setw(10) << data->pos.vec[0]
               << " Data2x:" << std::setw(10) << data2->pos.vec[0] << std::endl
               << "      y:" << std::setw(10) << data->pos.vec[1]
               << "      y:" << std::setw(10) << data2->pos.vec[1] << std::endl
               << "      z:" << std::setw(10) << data->pos.vec[2]
               << "      z:" << std::setw(10) << data2->pos.vec[2] << std::endl
               << "    azi:" << std::setw(10) << data->orient.vec[0]
               << "    azi:" << std::setw(10) << data2->orient.vec[0]
               << std::endl
               << "   elev:" << std::setw(10) << data->orient.vec[1]
               << "   elev:" << std::setw(10) << data2->orient.vec[1]
               << std::endl 
               << "   roll:" << std::setw(10) << data->orient.vec[2]
               << "   roll:" << std::setw(10) <<data2->orient.vec[2]
               << std::endl << std::endl;
     std::cout << "Ibox---------------------------------------------------------------"
               << std::endl
               << " button1: " << iboxData->button[0] << std::endl
               << " button2: " << iboxData->button[1] << std::endl 
               << " button3: " << iboxData->button[2] << std::endl 
               << " button4: " << iboxData->button[3] << endstd::l << std::endl;
/*     std::cout << "Glove--------------------------------------------------------------"
                 << std::endl
                 << " first finger: " << gdata->joints[0][0] << " "
                 << gdata->joints[0][1] << " " << gdata->joints[0][2] << " "
                 << gdata->joints[0][3] << std::endl
                 << "second finger: " << gdata->joints[1][0] << " "
                 << gdata->joints[1][1] << " " << gdata->joints[1][2] << " "
                 << gdata->joints[1][3] << std::endl
                 << " third finger: " << gdata->joints[2][0] << " "
                 << gdata->joints[2][1] << " " << gdata->joints[2][2] << " "
                 << gdata->joints[2][3] << std::endl
                 << " fourth finger: " << gdata->joints[3][0] << " "
                 << gdata->joints[3][1] << " " << gdata->joints[3][2] << " "
                 << gdata->joints[3][3] << std::endl;
*/
         vpr::System::sleep(2);
    }
     break;
   }
   std::cout << achar;
  } while ((achar != 'q') && (achar != 'Q'));
  
  flock->StopSampling();
  anIbox->StopSampling();
  aGlove->StopSampling();
  
  delete flock;
  delete anIbox;
  delete aGlove;
  delete anSgiPool;
  return 0;
}
