/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h> // for DEG2RAD macro
#include <fstream>        // for ifstream
#include "KeyFramer.h"
#include <vrj/Util/Debug.h>
#include <vrj/Math/Math.h>

namespace kev
{
   class KeyFramerExporter
   {
   public:
      KeyFramerExporter()
      {
      }

      void execute( const char* const filename, const kev::KeyFramer& kf )
      {
         std::ofstream frames_file( filename, std::ios::out );

         if (!frames_file.rdbuf()->is_open())
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << clrSetNORM(clrYELLOW) << "WARNING: "
                           << clrRESET
                           << "couldn't open keyframe file: "
                           << filename <<"\n"
                           << vprDEBUG_FLUSH;
            return;
         }

         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Writing keyframe(s) to file: "
                <<filename<<"\n"<<vprDEBUG_FLUSH;

         std::map<float, kev::KeyFramer::Key>::const_iterator it1 = kf.keys().begin();
         for (; it1 != kf.keys().end(); ++it1)
         {
            const kev::KeyFramer::Key& key = (*it1).second;
            const float& time = key.time();
            const vrj::Vec3& pos = key.position();
            const Quat& quat = key.rotation();
            float deg;
            vrj::Vec3 vec;
            quat.setRot( deg, vec[0], vec[1], vec[2] );
            deg = vrj::Math::rad2deg( deg );
            frames_file<<time<<" "<<pos[0]<<" "<<pos[1]<<" "
                       <<pos[2]<<" "<<deg<<" "<<vec[0]<<" "
                       <<vec[1]<<" "<<vec[2]<<"\n";
         }
         frames_file.close();
      }
   };
};
