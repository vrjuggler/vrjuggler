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


/**
 * This class is used to hold information about "generic" Beans loaded into the
 * system.  Such Beans are generic in that they are not viewers, GUI panels, or
 * services.
 *
 * @since 1.0
 */
public class GenericBean extends TweekBean
{
   public GenericBean (String name, String jar_path, String bean_entry_name,
                       java.util.Vector deps)
   {
      super(bean_entry_name, jar_path, deps);
      beanName = name;
   }

   public void instantiate ()
       throws org.vrjuggler.tweek.beans.loader.BeanInstantiationException
   {
      bean = doInstantiation();
   }

   public Object getBean ()
   {
      return bean;
   }

   public String getBeanName ()
   {
      return beanName;
   }

   protected Object bean     = null;
   protected String beanName = null;
}