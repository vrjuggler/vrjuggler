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


#ifndef _GADGET_DIGITALDATA_H_
#define _GADGET_DIGITALDATA_H_

#include <gadget/Type/InputData.h>
#include <vrj/Math/Matrix.h>

namespace gadget {

    /** InputData subclass for positional data. */
class GADGET_CLASS_API DigitalData: public InputData {

public:

    /** Constructor. */
    DigitalData (): InputData() {
        mDigitalData = 0;
    }


    int getDigitalData () {
        return mDigitalData;
    }


    void setDigitalData (int d) {
        mDigitalData = d;
    }


    DigitalData& operator= (const DigitalData& pd) {
        InputData::copy (pd);
        mDigitalData = pd.mDigitalData;
        return *this;
    }


    DigitalData& operator= (int data) {
        mDigitalData = data;
        return *this;
    }


protected:

    int mDigitalData;


}; // class DigitalData


}; // namespace gadget


#endif
