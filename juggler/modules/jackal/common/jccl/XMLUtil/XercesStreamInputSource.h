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

#ifndef _JCCL_XERCESSTREAMINPUTSOURCE_H_
#define _JCCL_XERCESSTREAMINPUTSOURCE_H_

#include <jccl/jcclConfig.h>

#include <sax/InputSource.hpp>
#include <util/BinInputStream.hpp>

namespace jccl {

//: Xerces-XML compatible InputSource wrapper for std::istream.
//  The stream has some kind of a terminating symbole (e.g. 
//  "&lt;/protocol&gt;").  The InputSource stops reading 
//  <i>immediately</i> upon seeing the end of the terminator string.
//  This is useful for grabbing sections of an XML document out of 
//  a larger data stream, such as we have with VjControl and the
//  JackalServer.
class XercesStreamInputSource: public InputSource {

    std::istream* in;

    //: The terminator-string that demarks the end of the stream.
    char* terminator;

public:

    XercesStreamInputSource (std::istream& _in, char const * _terminator);

    virtual ~XercesStreamInputSource ();

    virtual BinInputStream* makeStream() const;

};

};

#endif


