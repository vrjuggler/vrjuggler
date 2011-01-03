/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <Performer/pr.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pfutil.h>

#include "pfTerryCollide.h"

// Set up intersection masks for collision detection using 'mask' as the
// intersection mask. Cache normals and such inside geosets if the geometry
// is static, but not if it is dynamic, i.e., its vertices change.
void pfTerryCollide::nodeSetup(pfNode *node, const int mode, const int mask)
{
   int fullmode;

   if (mode == COLLIDE_DYNAMIC)
      fullmode = PFTRAV_SELF|PFTRAV_DESCEND|PFTRAV_IS_UNCACHE;
   else
      fullmode = PFTRAV_SELF|PFTRAV_DESCEND|PFTRAV_IS_CACHE;

   node->setTravMask(PFTRAV_ISECT, mask, fullmode, PF_SET);
}
