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


#include <sax/SAXParseException.hpp>
#include <jccl/XMLUtil/vjXercesXMLError.h>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <dom/DOMString.hpp>

//based on Xerces DOMPrint source code

namespace jccl {

void vjXercesXMLError::warning(const SAXParseException&)
{
    //
    // Ignore all warnings.
    //
}

void vjXercesXMLError::error(const SAXParseException& toCatch)
{
//      cerr << "Error at file \"" << DOMString(toCatch.getSystemId())
//  		 << "\", line " << toCatch.getLineNumber()
//  		 << ", column " << toCatch.getColumnNumber()
//           << "\n   Message: " << DOMString(toCatch.getMessage()) << endl;
    throw SAXParseException(toCatch);  // Copy the 'toCatch' object before throwing - 
                                       //   otherwise we would be throwing a reference to
                                       //   a local object that gets destroyed before
                                       //   the catch.
}

void vjXercesXMLError::fatalError(const SAXParseException& toCatch)
{
//      cerr << "Fatal Error at file \"" << DOMString(toCatch.getSystemId())
//  		 << "\", line " << toCatch.getLineNumber()
//  		 << ", column " << toCatch.getColumnNumber()
//           << "\n   Message: " << DOMString(toCatch.getMessage()) << endl;
    throw SAXParseException(toCatch);
}

void vjXercesXMLError::resetErrors()
{
    // No-op in this case
}


};
