/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _VRJ_DRAW_CPU_AFFINITY_FROM_ENV_H_
#define _VRJ_DRAW_CPU_AFFINITY_FROM_ENV_H_

#include <vrj/vrjConfig.h>

#include <string>
#include <vector>


namespace vrj
{

/**
 * A CPU affinity functor intended for use with multi-threaded Draw Managers
 * that accept a strategy for determining the affinity for render threads.
 * This implementation examines an environment variable (the name of which is
 * VJ_DRAW_THREAD_AFFINITY by default) that must contain a space-separated
 * list of integers identifying the CPU affinity ordering. For each thread
 * that the Draw Manager creates, the affinity will be returned. If more
 * threads are created than there are CPUs identified by the environment
 * variable, then the affinity assignment simply starts over at the beginning
 * of the list.
 *
 * @since 2.3.14
 */
class VJ_CLASS_API CpuAffinityFromEnv
{
public:
   /**
    * Looks up the value of the named environment variable and extracts the
    * CPU affinity indexing from it. The value of the environment variable,
    * if set, must be a space-separated list of integers. Values less than
    * zero can be used to indicate that the draw thread(s) mapped to that
    * index get no CPU affinity.
    *
    * @post \c mCpuList contains the integer indexing extracted from the
    *       named environment variable or is empty if no such indexing is
    *       available.
    *
    * @param envVar The name of the environment variable to examine for
    *               finding the user-supplied draw thread affinity.
    */
   CpuAffinityFromEnv(const std::string& envVar = "VJ_DRAW_THREAD_AFFINITY");

   /**
    * Determines the CPU affinity value for the given thread identifier. The
    * thread identifier must be a zero-based value.
    *
    * @return The CPU index for the identified thread or -1 if no CPU affinity
    *         is to be assigned for the thread.
    */
   int operator()(const unsigned int threadNum);

private:
   std::vector<int> mCpuList;
};

} // end of vrj namespace


#endif /* _VRJ_DRAW_CPU_AFFINITY_FROM_ENV_H_ */
