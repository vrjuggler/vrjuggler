import junit.framework.*;
import test.*;

import org.vrjuggler.tweek.beans.*;
import org.vrjuggler.jccl.config.ConfigBrokerImpl;

/**
 * This is the main class used to drive the JCCL Java test suite. Its
 * static suite() method adds all the tests currently available.
 */
public class AllTests
{
   public static void main( String[] args )
   {
      junit.swingui.TestRunner.run( AllTests.class );
   }

   /**
    * Adds all the tests currently available.
    */
   public static Test suite()
   {
      registerBeans();
      TestSuite suite = new TestSuite( "All JCCL JUnit Tests" );

      suite.addTestSuite( CategoryTest.class );
      suite.addTestSuite( ConfigElementTest.class );
      suite.addTestSuite( ConfigDefinitionTest.class );
      suite.addTestSuite( PropertyDefinitionTest.class );
      suite.addTestSuite( ConfigElementFactoryTest.class );
      suite.addTestSuite( ConfigDefinitionRepositoryTest.class );
      suite.addTestSuite( ConfigDefinitionReaderTest.class );
      suite.addTestSuite( ConfigurationReaderTest.class );

      return suite;
   }

   private static void registerBeans()
   {
      org.vrjuggler.tweek.TweekCore.registerStaticBeans();

      try
      {
         String base_dir = System.getProperty("JCCL_BASE_DIR");
         BeanRegistry registry = BeanRegistry.instance();
         BeanAttributes attrs =
            new BeanAttributes("ConfigBroker",
                               "file:" + base_dir +
                                 "/share/jccl/beans/jccl_config.jar",
                               "org.vrjuggler.jccl.config.ConfigBrokerImpl",
                               new java.util.ArrayList(),
                               new java.util.ArrayList());
         ServiceBean bean = new ServiceBean(attrs);
         bean.instantiate();
         registry.registerBean(bean);
      }
      catch(org.vrjuggler.tweek.beans.loader.BeanInstantiationException ex)
      {
         System.out.println(ex.getMessage());
      }
   }
}
