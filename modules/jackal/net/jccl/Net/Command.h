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



// commands used by connections...

#ifndef _JCCL_COMMAND_H_
#define _JCCL_COMMAND_H_

#include <jccl/jcclConfig.h>
#include <jccl/Plugins/PerformanceMonitor/TimeStamp.h>

namespace jccl {


    class JCCL_CLASS_API Command {

    public:
        
        Command ();

        virtual void call (std::ostream& out) const = 0;

        virtual const std::string& getProtocolName () const = 0;

    };

    
    class JCCL_CLASS_API PeriodicCommand: public Command {

    public:

        float next_fire_time;
        const float refresh_time;

        PeriodicCommand (float _refresh_time);

        void resetFireTime (TimeStamp& ts);

        int operator < (const PeriodicCommand& cmd2) const;

    };



};

#endif
