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

import java.util.Vector;
import org.vrjuggler.tweek.beans.loader.*;


/**
 * @since 1.0
 */
public abstract class TweekBean
{
   public abstract void instantiate() throws BeanInstantiationException;

   public boolean isInstantiated ()
   {
      return instantiated;
   }

   public String getJarPath ()
   {
      return jarPath;
   }

   protected TweekBean (String name)
   {
      m_bean_name = name;
   }

   protected TweekBean (String name, String jar_path, Vector deps)
   {
      m_bean_name = name;
      jarPath     = jar_path;
      m_deps      = deps;
   }

   protected Object doInstantiation () throws BeanInstantiationException
   {
      BeanLoader bean_loader = new BeanLoader();
      Object bean = null;

      try
      {
         String class_path = null;

         if ( m_deps.size() >= 1 )
         {
            class_path = ((BeanDependency) m_deps.elementAt(0)).getPath();

            for ( int i = 1; i < m_deps.size(); i++ )
            {
               class_path = class_path + ";" +
                            ((BeanDependency) m_deps.elementAt(i)).getPath();
            }
         }

         Vector deps = new Vector();

         for ( int i = 0; i < m_deps.size(); i++ )
         {
            deps.add(((BeanDependency) m_deps.elementAt(i)).getFile());
         }

         bean_loader.loadBeanFromJar(jarPath, deps, class_path);
         bean = bean_loader.instantiate(BeanJarClassLoader.instance(),
                                        m_bean_name);

         instantiated = true;

         BeanInstantiationCommunicator.instance().fireBeanInstantiationEvent(bean);
      }
      catch (java.net.MalformedURLException e)
      {
         throw new BeanInstantiationException(e.getMessage());
      }
      catch (java.io.IOException e)
      {
         throw new BeanInstantiationException(e.getMessage());
      }

      return bean;
   }

   protected String m_bean_name = null;
   protected String jarPath     = null;
   protected Vector m_deps      = null;

   protected boolean instantiated = false;
}