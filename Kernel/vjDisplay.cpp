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


#include <vjConfig.h>
#include <Kernel/vjDebug.h>

#include <Kernel/vjDisplay.h>
#include <Kernel/vjKernel.h>


void vjDisplay::config(vjConfigChunk* chunk)
{
   vjASSERT(chunk != NULL);

   // -- Get config info from chunk -- //
    int originX = chunk->getProperty("origin", 0);
    int originY = chunk->getProperty("origin", 1);
    int sizeX   = chunk->getProperty("size", 0);
    int sizeY   = chunk->getProperty("size", 1);
    std::string name  = chunk->getProperty("name");
    mBorder     = chunk->getProperty("border");
    int pipe    = chunk->getProperty("pipe");
    mView    = (vjDisplay::DisplayView)(int)chunk->getProperty("view");
    mActive  = chunk->getProperty("active");

    mLatencyMeasure = new vjPerfDataBuffer ("Head Latency " + name, 500, 4);
    vjKernel::instance()->getEnvironmentManager()->addPerfDataBuffer (mLatencyMeasure);

    // -- Check for error in configuration -- //
    // NOTE: If there are errors, set them to some default value
   if(sizeX <= 0)
   {
      vjDEBUG(vjDBG_DISP_MGR,2) << "WARNING: window sizeX set to: " << sizeX
                                << ".  Setting to 10." << std::endl
                                << vjDEBUG_FLUSH;
      sizeX = 10;
   }

   if(sizeY <= 0)
   {
      vjDEBUG(vjDBG_DISP_MGR,2) << "WARNING: window sizeY set to: " << sizeY
                                << ".  Setting to 10." << std::endl
                                << vjDEBUG_FLUSH;
      sizeY = 10;
   }

   if(pipe < 0)
   {
      vjDEBUG(vjDBG_DISP_MGR,2) << "WARNING: pipe was negative, pipe set to: " << pipe << ".  Setting to 0.\n" << vjDEBUG_FLUSH;
      pipe = 0;
   }

      // -- Set local window attributes --- //
    setOriginAndSize(originX, originY, sizeX, sizeY);

    // Get the user for this display
    std::string user_name = chunk->getProperty("user");
    mUser = vjKernel::instance()->getUser(user_name);

    if(NULL == mUser)
    {
       vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << " User not found named: "
                              << user_name.c_str() << std::endl
                              << vjDEBUG_FLUSH;
      vjASSERT(false);
    }

    setName(name);
    setPipe(pipe);

    mDisplayChunk = chunk;        // Save the chunk for later use
}


    // ---- FRIEND FUNCTIONS ---- //
//! PRE: disp != NULL
//+      disp->mUser != NULL
std::ostream& vjDisplay::outStream(std::ostream& out)
{
   vjASSERT(mUser != NULL);

    out << std::setw(15) << mName.c_str() << std::endl
        << "  org:" << _xo << ", " << _yo
        << "  sz:" << _xs << ", " << _ys
        << "  p:" << mPipe
        << "  view:" << ((mView == vjDisplay::LEFT_EYE) ? "Left" : ((mView==vjDisplay::RIGHT_EYE)?"Right" : "Stereo") )
        << "  act:" << (mActive ? "Y" : "N")
        << "  usr:" << mUser->getName().c_str();

    return out;
}


std::ostream& operator<<(std::ostream& out,  vjDisplay& disp)
{
   return disp.outStream(out);
}
