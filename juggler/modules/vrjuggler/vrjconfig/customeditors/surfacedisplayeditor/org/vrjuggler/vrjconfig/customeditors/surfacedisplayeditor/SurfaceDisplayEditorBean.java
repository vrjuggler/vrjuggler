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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.surfacedisplayeditor;

import org.vrjuggler.tweek.beans.BeanAttributes;
import org.vrjuggler.tweek.beans.ServiceBean;

import org.vrjuggler.jccl.editors.*;
//import org.vrjuggler.tweek.beans.*;

/**
 * @since 1.0
 */
public class SurfaceDisplayEditorBean 
   //extends ServiceBean 
   //implements java.io.Serializable 
{
   /**
    * Creates a new Service bean with the given common TweekBean attributes.
    *
    * @param attrs   the TweekBean attributes of this bean
    */
   public SurfaceDisplayEditorBean ()
   {
//      CustomEditorRegistry.registerEditor("display_window",
//         SurfaceDisplayEditor.class);
//      CustomEditorRegistry.registerEditor("surface_viewport",
//         SurfaceDisplayEditor.class);
   }

   /*
   public void instantiate ()
      throws org.vrjuggler.tweek.beans.loader.BeanInstantiationException
   {
      System.out.println("Before we try instantiating.");
      super.instantiate();
      System.out.println("Before we try instantiating.");
      
      if ( ! instantiated )
      {
         System.out.println("Registering display_window");
         ConfigElementCustomEditorRegistry.registerEditor("display_window",
               SurfaceDisplayEditor.class);
      }
   }
   */
}
