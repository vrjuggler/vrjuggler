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


/**
 * @version 1.0
 */
public class BeanRegistryTest extends TestCase
{
   public BeanRegistryTest (String name)
   {
      super(name);
   }

   public static Test suite ()
   {
      return new TestSuite(BeanRegistryTest.class);
   }

   public void testBeanRegistration ()
   {
      BeanRegistry registry = BeanRegistry.instance();

      BeanAttributes attrs = new BeanAttributes("TestBean");
      GenericBean test_bean = new GenericBean(attrs);
      registry.registerBean(test_bean);

      TweekBean bean = registry.getBean("TestBean");
      assertTrue(test_bean == bean);
   }
}
