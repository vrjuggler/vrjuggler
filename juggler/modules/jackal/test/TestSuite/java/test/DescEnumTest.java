package test;

import junit.framework.*;
import org.vrjuggler.jccl.config.*;

/**
 * Test the DescEnum class.
 */
public class DescEnumTest
   extends TestCase
{
   public DescEnumTest( String name )
   {
      super( name );
   }

   /**
    * Tests the clone() method.
    */
   public void testClone()
   {
      DescEnum orig = new DescEnum( "Display", ValType.INT, new Integer(455) );
      DescEnum clone = null;
      try
      {
         clone = (DescEnum)orig.clone();
      }
      catch ( CloneNotSupportedException cnse )
      {
         assertTrue( cnse.getMessage(), false );
      }
      assertEquals( orig.getName(), clone.getName() );
//      assertTrue( orig.getName() != clone.getName() );
      assertTrue( orig.getValue().equals(clone.getValue()) );
      assertTrue( orig.getValue() != clone.getValue() );
   }

   /**
    * Tests the equals() method.
    */
   public void testEquals()
   {
      DescEnum de1 = new DescEnum( "Pipe", ValType.INT, new Integer(1) );
      DescEnum de2 = new DescEnum( "Pipe", ValType.INT, new Integer(1) );
      assertTrue(de1.equals(de1));
      assertTrue(de2.equals(de2));
      assertTrue(! de1.equals(de2));
      assertTrue(! de2.equals(de1));
   }
}
