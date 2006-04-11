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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

package tweektest;

import junit.framework.*;
import org.vrjuggler.tweek.beans.*;


public class BeanLoaderTest extends TestCase
{
   public BeanLoaderTest (String name)
   {
      super(name);
   }

   public static Test suite ()
   {
      return new TestSuite(BeanLoaderTest.class);
   }

   public void setUp ()
   {
      BeanRegistry registry = BeanRegistry.instance();
      mRegListener.setBeanCount(0);
      registry.addBeanRegistrationListener(mRegListener);
   }

   public void tearDown ()
   {
      BeanRegistry registry = BeanRegistry.instance();
      registry.removeBeanRegistrationListener(mRegListener);
   }

   public void testBeanFinder ()
   {
      XMLBeanFinder finder = new XMLBeanFinder();

      try
      {
         java.util.List beans = finder.find(".");
         assertTrue(beans.size() > 0);
      }
      catch (org.vrjuggler.tweek.beans.BeanPathException e)
      {
         assertTrue(false);
      }
   }

   public void testBeanRegistration ()
   {
      BeanRegistry registry = BeanRegistry.instance();
      XMLBeanFinder finder  = new XMLBeanFinder();

      try
      {
         java.util.List beans = finder.find(".");

         for ( java.util.Iterator itr = beans.iterator(); itr.hasNext(); )
         {
            TweekBean bean = (TweekBean) itr.next();
            registry.registerBean(bean);
         }

         assertTrue(beans.size() == mRegListener.getBeanCount());
      }
      catch (org.vrjuggler.tweek.beans.BeanPathException e)
      {
         assertTrue(false);
      }
   }

   private class RegListener implements BeanRegistrationListener
   {
      public void beanRegistered (BeanRegistrationEvent regEvent)
      {
         assertTrue(regEvent.getBean() != null);
         beanCount++;
      }

      public void setBeanCount (int count)
      {
         beanCount = count;
      }

      public int getBeanCount ()
      {
         return beanCount;
      }

      int beanCount = 0;
   }

   RegListener mRegListener = new RegListener();
}
