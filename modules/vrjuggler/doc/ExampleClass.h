/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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



//-------------------------------------
//: This is an example class.
//
// It is an example of how the VR Juggler
// classes should be written
//---------------------------------------
class ExampleClass
{
public:
   //  Example constructor
   //
   //! PRE: None
   //! POST: Object is constructed
   ExampleClass()
   {
      mIntVariable = 0;
      mFloatVariable = 0.0f;
   }

public:
   //:  Brief header comment
   //  More detailed header comment here.  This more detailed
   // comment tells more information about what the function
   // is doing and maybe how it does it.
   //
   //! PRE: No Header Precondition
   //! POST: No Header Post conditions
   //
   //! ARGS: junkValue - This is a vlaue that passes in
   //+                     some junk of some sort
   //! ARGS: junkValue2 - This is some other value that
   //+                     we really don't need to use.
   //
   //! RETURNS: 1 - Successfully did it's thing
   //! RETURNS: 0 - Failed miserably
   //
   //! NOTE: This is a small notice of some exceptional
   //+       thing this function does.
   //
   //! MODIFIES: mIntVariable will be modified
   int doSomething(int junkValue, int junkValue2);

private:
   int   mIntVariable;     // Example integer variable
   float mFloatVariable;   // Example float variable
};
