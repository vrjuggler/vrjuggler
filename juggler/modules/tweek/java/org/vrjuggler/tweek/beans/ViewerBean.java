/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

import org.vrjuggler.tweek.beans.loader.BeanInstantiationException;


/**
 * @since 1.0
 *
 * @see org.vrjuggler.tweek.beans.BeanCollectionBuilder
 */
public class ViewerBean extends TweekBean
{
   public ViewerBean (String viewer_name, String jar_path,
                      String bean_entry_name, java.util.Vector deps)
   {
      super(bean_entry_name, jar_path, deps);
      viewerName = viewer_name;
   }

   public void instantiate () throws BeanInstantiationException
   {
      try
      {
         viewer = (BeanModelViewer) doInstantiation();
      }
      catch (ClassCastException e)
      {
         throw new BeanInstantiationException("org.vrjuggler.tweek.beans.BeanModelViewer " +
                                              "is not a superclass of " +
                                              m_bean_name.replace('/', '.'));
      }
   }

   public String getViewerName ()
   {
      return viewerName;
   }

   public BeanModelViewer getViewer ()
   {
      return viewer;
   }

   protected BeanModelViewer viewer     = null;
   protected String          viewerName = null;
}