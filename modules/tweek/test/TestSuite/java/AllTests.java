import junit.framework.*;
import tweektest.*;


/**
 * This is the main class used to execute the Tweek Java test suite.  Its
 * statis suite() method adds all the tests currently available.
 */
public class AllTests
{
   public static void main (String[] args)
   {
      junit.swingui.TestRunner.run(AllTests.class);
   }

   public static Test suite ()
   {
      TestSuite suite = new TestSuite("All Tweek JUnit tests");

      suite.addTest(BeanRegistryTest.suite());
      suite.addTest(BeanLoaderTest.suite());
      suite.addTest(GlobalPreferencesServiceTest.suite());

      return suite;
   }
}
