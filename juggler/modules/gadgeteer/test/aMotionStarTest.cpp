#include <iostream.h>
#include <iomanip.h>
#include <SharedMem/vjMemPool.h>
//#include <SharedMem/vjSharedType.h>
#include <Input/vjPosition/aMotionStar.h>
#include <Math/vjCoord.h>

int main()
{
  vjMemPool* aMemPool = new vjSharedPool(1024*1024);

  // Get configuration information \

  cerr << "-------- Bird Test program -------\n";
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
                  //cout << " Bird: " << bird;
                  cout << " " << setprecision(8)  << motionstar->xPos(bird)
                       << " " << setprecision(8)  << motionstar->yPos(bird)
                       << " " << setprecision(8)  << motionstar->zPos(bird)
                       << " " << setprecision(8)  << motionstar->xRot(bird)
                       << " " << setprecision(8) << motionstar->yRot(bird)
                       << " " << setprecision(8) << motionstar->zRot(bird);
            }
            cout << endl;
            //sleep(2);
         }
         break;
      }
   } while ((achar != 'q') && (achar != 'Q'));

   cerr << "---- Quiting -----\n"
        << "Stoping birds.....\n" << flush;

  motionstar->stop();

  delete motionstar;
  delete aMemPool;
  return 0;
}
