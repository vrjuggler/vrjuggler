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

package org.vrjuggler.tweek.beans;

import org.xml.sax.SAXParseException;
import org.xml.sax.SAXException;


/**
 * An implementation of org.xml.sax.ErrorHandler for use with JDOM.  This is
 * based on code from Chapter 7 of Elliotte Rusty Harold's forthcoming book,
 * <i>Processing XML with Java</i>.
 */
public class BestSAXChecker implements org.xml.sax.ErrorHandler
{
   public void warning(SAXParseException exception)
   {
      System.err.println("Warning: " + exception.getMessage());
      System.err.println(" at line " + exception.getLineNumber()
                         + ", column " + exception.getColumnNumber());
      System.err.println(" in entity " + exception.getSystemId());
   }

   public void error(SAXParseException exception)
   {
      System.err.println("Error: " + exception.getMessage());
      System.err.println(" at line " + exception.getLineNumber()
                         + ", column " + exception.getColumnNumber());
      System.err.println(" in entity " + exception.getSystemId());
   }

   public void fatalError(SAXParseException exception)
   {
      System.err.println("Fatal Error: " + exception.getMessage());
      System.err.println(" at line " + exception.getLineNumber()
                         + ", column " + exception.getColumnNumber());
      System.err.println(" in entity " + exception.getSystemId());
   }
}
