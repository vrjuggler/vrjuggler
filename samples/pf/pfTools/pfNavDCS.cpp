/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001, 2002
 *   by Iowa State University
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

#include <Kernel/Pf/vjPfUtil.h>

#include <pfNavDCS.h>


pfNavDCS::pfNavDCS() : pfDCS()
{
   setType(classType);  // Set the type
   mActive = true;
   mNaver = NULL;
}


// Get the current position from the navigator
// and set the DCS with it
void pfNavDCS::updateTransformMatrix()
{
   vjASSERT(mNaver != NULL);     // We mut have a naver

   // Set the navigation DCS to the new navigation matrix
   // cur_pos = modelspace_M_user
   vjMatrix cur_pos_inv, cur_pos;
   cur_pos = mNaver->getCurPos();
   cur_pos_inv.invert(cur_pos);
   pfMatrix model_move = vjGetPfMatrix( cur_pos_inv );
   this->setMat( model_move );
}

// app() - APP traversal function.  This overloads the standard pfNode
// app() method, which will be called each frame during the APP
// traversal of the scene graph (*only if* needsApp() (below) returns
// TRUE).
// app() is called automatically by Performer; it is not called directly
// by a program.
int pfNavDCS::app(pfTraverser *trav)
{
   if(mNaver != NULL)
   {
      if(isActive())
      {
         // Get input and update the state baed upon that input
         mNaver->updateInteraction();

         // Update the state of the navigation
         mNaver->update();

         updateTransformMatrix();
      }
   }
   else
   {
      vjDEBUG(vjDBG_ALL,0) << "pfNavDCS::app: No naver set\n" << vjDEBUG_FLUSH;
   }

   return pfDCS::app(trav);        /* Finish by calling the parent class's app() */
}



//---------------------------------------------------------------------//
// Performer type data - this part is required for any class which
// is derived from a Performer class.  It creates a new pfType
// which identifies objects of this class.  All constructors for
// this class must then call setType(classType_).
pfType *pfNavDCS::classType = NULL;

void pfNavDCS::init(void)
{
 if (classType == NULL)
   {
        pfDCS::init();           // Initialize my parent
        classType =  new pfType(pfDCS::getClassType(), "pfNavDCS");  // Create the new type
   }
}
//----------------------------------------------------------------------//
