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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

import java.util.List;
import java.util.Vector;
import org.vrjuggler.tweek.beans.loader.*;


/**
 * Abstract base class for all beans that wish to be loaded by Tweek.
 */
public abstract class TweekBean
{
   public abstract void instantiate() throws BeanInstantiationException;

   public boolean isInstantiated()
   {
      return instantiated;
   }

   /**
    * Convenience method for getting the name of this bean.
    *
    * @return  the name of this bean
    */
   public String getName()
   {
      return attrs.getName();
   }
   
   public String toString()
   {
      return getName();
   }

   /**
    * Convenience method for getting the JAR URL string out of the
    * BeanAttributes of this bean.
    *
    * @return  the URL string of the JAR
    */
   public String getJarURL()
   {
      return attrs.getJarURL();
   }

   /**
    * Gets the attributes of this bean.
    *
    * @return  the attributes that describe this bean
    */
   public BeanAttributes getAttributes()
   {
      return attrs;
   }

   /**
    * Gets the actual bean object that this class wraps. This method will return
    * null if the bean has not yet been instantiated.
    *
    * @return  the bean this class wraps or null if the bean has not yet been
    *          instantiated
    */
   public Object getBean()
   {
      return beanObject;
   }

   /**
    * Creates a new TweekBean with the given attributes.
    *
    * @param attrs      the attributes to assign to this bean
    */
   protected TweekBean(BeanAttributes attrs)
   {
      beanObject = null;
      this.attrs = attrs;
   }

   protected void doInstantiation()
      throws BeanInstantiationException
   {
      BeanLoader bean_loader = new BeanLoader();
      beanObject = null;

      try
      {
         String class_path  = null;
         Vector depJarFiles = new Vector();
         List deps          = attrs.getJarDependencies();

         if ( null != deps )
         {
            // build up the class path for the dependencies
            if ( deps.size() >= 1 )
            {
               class_path = ((JarDependency) deps.get(0)).getPath();

               for ( int i = 1; i < deps.size(); i++ )
               {
                  class_path = class_path + ";" +
                               ((JarDependency) deps.get(i)).getPath();
               }
            }

            // build up a list of the JAR files that the bean depends on
            for ( int i = 0; i < deps.size(); i++ )
            {
               depJarFiles.add(((JarDependency) deps.get(i)).getFile());
            }
         }

         bean_loader.loadBeanFromJar(getJarURL(), depJarFiles, class_path);
         beanObject = bean_loader.instantiate(BeanJarClassLoader.instance(),
                                              attrs.getClassname());

         instantiated = true;

         BeanInstantiationCommunicator.instance().fireBeanInstantiationEvent( this );
      }
      catch (java.net.MalformedURLException e)
      {
         String message = "Instantiation of Bean '" + getName() +
                             "' (type " + attrs.getClassname() + ") failed";
         throw new BeanInstantiationException(message, e);
      }
      catch (java.io.IOException e)
      {
         String message = "Instantiation of Bean '" + getName() +
                             "' (type " + attrs.getClassname() + ") failed";
         throw new BeanInstantiationException(message, e);
      }
   }

   /**
    * The common attributes to all Tweek Beans.
    */
   protected BeanAttributes attrs;

   /**
    * The actual bean object that this class wraps.
    */
   protected Object beanObject;

   protected boolean instantiated = false;
}
