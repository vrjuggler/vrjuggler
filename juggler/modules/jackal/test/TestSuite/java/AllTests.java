import junit.framework.*;
import test.*;

/**
 * This is the main class used to drive the VjControl Java test suite. Its
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
      TestSuite suite = new TestSuite( "All VrjConfig JUnit Tests" );

      suite.addTestSuite( ValTypeTest.class );
      suite.addTestSuite( DescEnumTest.class );
      suite.addTestSuite( PropertyDescTest.class );
      suite.addTestSuite( ChunkDescTest.class );
      suite.addTestSuite( ChunkDescEventTest.class );
      suite.addTestSuite( ConfigChunkTest.class );

      suite.addTestSuite( ConfigElementTest.class );
      suite.addTestSuite( ConfigDefinitionTest.class );
      suite.addTestSuite( PropertyDefinitionTest.class );
      suite.addTestSuite( ConfigElementFactoryTest.class );
      suite.addTestSuite( ConfigDefinitionRepositoryTest.class );
      suite.addTestSuite( ConfigDefinitionBuilderTest.class );
      suite.addTestSuite( ConfigurationReaderTest.class );

      return suite;
   }
}
