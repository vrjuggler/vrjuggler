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


#ifndef _VJ_OSX_SYSTEM_FACTORY_H_
#define _VJ_OSX_SYSTEM_FACTORY_H_

#include <vjConfig.h>
#include <Kernel/vjSystemFactory.h>
#include <vpr/Util/Singleton.h>
#include <Carbon/Carbon.h>

namespace vrj
{
   
class DisplayManager;

//---------------------------------------------------
//: Concrete class for Mac OS X System specific Factory.
//
//	Concrete class to create Mac OS X system specific objects.
//
// @author Allen Bierbaum
//  Date: 9-9-97
//----------------------------------------------------

static OSErr QuitAppleEventHandler(const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon);

class OSXSystemFactory : public SystemFactory
{
public:

   void CarbonApplicationThread(void* nullParam);

      // --- Singleton stuff --- //
protected:
   /// Constructor:  Hidden, so no instantiation is allowed
   OSXSystemFactory();

   vprSingletonHeader(OSXSystemFactory);



protected:
    Boolean		gQuitFlag;
    Boolean		InitComplete;

    void Initialize(void);
    void EventLoop(void);
    void DoEvent(EventRecord *event);
    void DoMenuCommand(long menuResult);

public:
    void setQuitFlag(Boolean value) { gQuitFlag = value; }
};

};
#endif
