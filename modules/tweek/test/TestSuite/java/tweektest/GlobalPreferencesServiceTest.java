package tweektest;

import java.io.File;
import junit.framework.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.BeanAttributes;
import org.vrjuggler.tweek.services.GlobalPreferencesService;


public class GlobalPreferencesServiceTest extends TestCase
{
   public GlobalPreferencesServiceTest (String name)
   {
      super(name);
   }

   public static Test suite ()
   {
      return new TestSuite(GlobalPreferencesServiceTest.class);
   }

/*
   public void testCreationAndLoad ()
   {
      mPrefs.setFile(mTestTweekrc);

      mPrefs.load();
      assertTrue(mTestTweekrc.exists());
   }
*/

   public void testModification ()
   {
      mPrefs.setFile(mTestTweekrc);
      mPrefs.load();

      String bean_viewer = "SomeViewer";
      mPrefs.setBeanViewer(bean_viewer);
      mPrefs.save(true);

      mPrefs.load();
      assertTrue(mPrefs.getBeanViewer().equals(bean_viewer));;

      int user_level = mPrefs.getUserLevel() + 1;
      mPrefs.setUserLevel(user_level);
      mPrefs.save(true);

      mPrefs.load();
      assertTrue(mPrefs.getUserLevel() == user_level);

      String chooser_start = "${HOME}/bin";
      mPrefs.setChooserStartDir(chooser_start);
      mPrefs.save(true);

      mPrefs.load();
      assertTrue(mPrefs.getChooserStartDir().equals(chooser_start));;

      int chooser_open =
         (mPrefs.getChooserOpenStyle() == GlobalPreferencesService.EMACS_CHOOSER
            ? GlobalPreferencesService.EMACS_CHOOSER
            : GlobalPreferencesService.WINDOWS_CHOOSER);
      mPrefs.setChooserOpenStyle(chooser_open);
      mPrefs.save(true);

      mPrefs.load();
      assertTrue(mPrefs.getChooserOpenStyle() == chooser_open);

      boolean lazyinst = ! mPrefs.getLazyPanelBeanInstantiation();
      mPrefs.setLazyPanelBeanInstantiation(lazyinst);
      mPrefs.save(true);

      mPrefs.load();
      assertTrue(mPrefs.getLazyPanelBeanInstantiation() == lazyinst);
   }

   protected void setUp ()
   {
      mPrefs = new GlobalPreferencesService(new BeanAttributes("GlobalPreferences"));

      if ( mTestTweekrc.exists() )
      {
         mTestTweekrc.delete();
      }

      assertTrue(! mTestTweekrc.exists());
   }

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
