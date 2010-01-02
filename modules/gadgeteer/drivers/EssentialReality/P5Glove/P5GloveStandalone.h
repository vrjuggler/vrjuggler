/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _GADGET_P5_GLOVE_STANDALONE_H_
#define _GADGET_P5_GLOVE_STANDALONE_H_

#include <vector>

#include "p5glove.h"


/**
 * Software interface to wireless EssentialReality P5glove hardware.
 */
class P5GloveStandalone
{
public:
   struct Record
   {
      Record();

      bool buttonA;
      bool buttonB;
      bool buttonC;
      double thumb;
      double index;
      double middle;
      double ring;
      double pinky;
      std::vector<double> position;
      std::vector<double> rotationAxis;
      double rotationAngle;
   };

   /** Default constructor. */
   P5GloveStandalone();

   /** Default destructor. */
   ~P5GloveStandalone();

   /** Connects to the data glove hardware */
   bool connectToHardware(const int gloveNumber);

   bool readRecordsFromHardware(Record& rec);

private:
   P5Glove mGlove;
};


#endif /* _GADGET_P5_GLOVE_STANDALONE_H_ */
