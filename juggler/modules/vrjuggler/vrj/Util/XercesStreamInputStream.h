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

#ifndef _VRJ_XERCESSTREAMINPUTSTREAM_H_
#define _VRJ_XERCESSTREAMINPUTSTREAM_H_

#include <vrj/vjConfig.h>
#include <util/BinInputStream.hpp>

namespace vrj
{
   
//: Xerces-XML compatible BinInputStream wrapper for std::istream.
class XercesStreamInputStream: public BinInputStream {

private:

    const char* terminator;
    unsigned int termlength;

    //: How many characters of the terminator we've found so far.
    unsigned int search_state;

    //: true when we've reached the end of stream (marked by terminator).
    bool finished;
    std::istream* in;

public:

    XercesStreamInputStream (std::istream& _in, const char* _terminator);

    virtual ~XercesStreamInputStream();

    virtual unsigned int curPos() const;

    virtual unsigned int readBytes (XMLByte* const buf, const unsigned int _buflen);


};

};

#endif
