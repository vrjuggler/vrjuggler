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



// vjDummyPosition - a dummy child of vjPosition.

#include <vjConfig.h>
#include <Input/vjPosition/vjDummyPosition.h>
#include <Math/vjCoord.h>
#include <Math/vjMatrix.h>

bool vjDummyPosition::config(vjConfigChunk *c)
{
   if(!vjPosition::config(c))
      return false;

   vjCoord coord;

   vjDEBUG(vjDBG_INPUT_MGR,3) << "  vjDummyPosition::config(vjConfigChunk*) " << endl << vjDEBUG_FLUSH;

   coord.pos[VJ_X] = c->getProperty("position",0);
   coord.pos[VJ_Y] = c->getProperty("position",1);
   coord.pos[VJ_Z] = c->getProperty("position",2);
   coord.orient[VJ_X] = c->getProperty("ori",0);
   coord.orient[VJ_Y] = c->getProperty("ori",1);
   coord.orient[VJ_Z] = c->getProperty("ori",2);

   mydata = vjMatrix(coord);
   mTimeStamp.set();

   active = 1;

   return true;
}


vjDummyPosition::~vjDummyPosition() {

}

int vjDummyPosition::startSampling() {
    return 1;
}

int vjDummyPosition::stopSampling() {
    return 1;
}


vjMatrix* vjDummyPosition::getPosData (int d) {
    return (&mydata);
}


vjTimeStamp* vjDummyPosition::getPosUpdateTime(int d) {
    return &mTimeStamp;
}


void vjDummyPosition::updateData () {

}

int vjDummyPosition::sample() {
    return 1;
}
