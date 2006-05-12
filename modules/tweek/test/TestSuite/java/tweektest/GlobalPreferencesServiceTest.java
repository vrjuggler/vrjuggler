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

package tweektest;

import java.io.File;
import junit.framework.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.BeanAttributes;
import org.vrjuggler.tweek.services.GlobalPreferencesServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesService;


public class GlobalPreferencesServiceTest extends TestCase
{
   public GlobalPreferencesServiceTest(String name)
   {
      super(name);
   }

   public static Test suite()
   {
      return new TestSuite(GlobalPreferencesServiceTest.class);
   }

   /**
    * Tests creation of a new Tweek RC file.
    */
   public void testCreation()
   {
      mPrefs.setFile(mTestTweekrc);      // Specify an alternate file to load
      mPrefs.load();                     // Load it--this creates it first
      assertTrue(mTestTweekrc.exists());
   }

   /**
    * Tests modification of the default settings in a fresh Tweek RC file.
    */
   public void testModification()
   {
      mPrefs.setFile(mTestTweekrc);
      mPrefs.load();

      String bean_viewer = "SomeViewer";
      mPrefs.setBeanViewer(bean_viewer);
      mPrefs.save();

      mPrefs.load();
      assertTrue(mPrefs.getBeanViewer().equals(bean_viewer));;

      int user_level = mPrefs.getUserLevel() + 1;
      mPrefs.setUserLevel(user_level);
      mPrefs.save();

      mPrefs.load();
      assertTrue(mPrefs.getUserLevel() == user_level);

      String chooser_start = "${HOME}/bin";
      mPrefs.setChooserStartDir(chooser_start);
      mPrefs.save();

      mPrefs.load();
      assertTrue(mPrefs.getChooserStartDir().equals(chooser_start));;

      boolean lazyinst = ! mPrefs.getLazyPanelBeanInstantiation();
      mPrefs.setLazyPanelBeanInstantiation(lazyinst);
      mPrefs.save();

      mPrefs.load();
      assertTrue(mPrefs.getLazyPanelBeanInstantiation() == lazyinst);
   }

   /**
    * Ensures that a fresh GlobalPreferencesService object is available for
    * each test and that the test Tweek RC file does not exist.
    */
   protected void setUp()
   {
      mPrefs = new GlobalPreferencesServiceProxy();

      if ( mTestTweekrc.exists() )
      {
         mTestTweekrc.delete();
      }

      assertTrue(! mTestTweekrc.exists());
   }

   /**
    * Removes the test Tweek RC file that a given test may have created.
    */
   protected void tearDown ()
   {
      if ( mTestTweekrc.exists() )
      {
         mTestTweekrc.delete();
      }
   }

   private GlobalPreferencesService mPrefs = null;
   private File mTestTweekrc = new File("test.tweekrc");
}
