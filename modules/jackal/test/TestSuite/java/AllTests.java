import junit.framework.*;
import test.*;

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
}
