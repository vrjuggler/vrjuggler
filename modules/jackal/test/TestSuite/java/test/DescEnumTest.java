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
    * Test the toString representation.
    */
   public void testToString()
   {
      DescEnum de = new DescEnum( "ShoeSize", new VarValue(13) );
      assertEquals( de.toString(), "ShoeSize=13" );

      // null value case
      de.val = null;
      assertEquals( de.toString(), "ShoeSize" );
   }

   /**
    * Tests the copy constructor.
    */
   public void testCopyConstructor()
   {
      DescEnum orig = new DescEnum( "PizzaTopping", new VarValue("Pepperoni") );
      DescEnum copy = new DescEnum( orig );
      assertEquals( orig.str, copy.str );
//      assertTrue( orig.str != copy.str );
      assertTrue( orig.val.equals(copy.val) );
      assertTrue( orig.val != copy.val );
   }

   /**
    * Tests the clone() method.
    */
   public void testClone()
   {
      DescEnum orig = new DescEnum( "Display", new VarValue(455) );
      DescEnum clone = null;
      try
      {
         clone = (DescEnum)orig.clone();
      }
      catch ( CloneNotSupportedException cnse )
      {
         assertTrue( cnse.getMessage(), false );
      }
      assertEquals( orig.str, clone.str );
//      assertTrue( orig.str != clone.str );
      assertTrue( orig.val.equals(clone.val) );
      assertTrue( orig.val != clone.val );
   }

   /**
    * Tests the equals() method.
    */
   public void testEquals()
   {
      DescEnum de1 = new DescEnum( "Pipe", new VarValue(1) );
      DescEnum de2 = new DescEnum( "Pipe", new VarValue(1) );
      assertTrue( de1.equals(de2) );
      de2.str = "Display";
      assertTrue( ! de1.equals(de2) );
      de2.str = "Pipe";
      de2.val.set(2);
      assertTrue( ! de1.equals(de2) );
      de2.val = new VarValue( 1.0f );
      assertTrue( ! de1.equals(de2) );
   }
}
