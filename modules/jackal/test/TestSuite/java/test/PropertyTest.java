package test;

import junit.framework.*;
import org.vrjuggler.jccl.config.*;

/**
 * Test the Property class.
 */
public class PropertyTest
   extends TestCase
{
   public PropertyTest( String name )
   {
      super( name );
   }

   private PropertyDesc mPropDesc1;
   private PropertyDesc mPropDesc2;

   protected void setUp()
   {
      mPropDesc1 = new PropertyDesc();
      mPropDesc1.setName( "MyPropDesc" );
      mPropDesc1.setToken( "MyPropDescToken" );
      mPropDesc1.setHelp( "Help me! I'm meeelllltiiiinng!" );
      mPropDesc1.setValType( ValType.STRING );

      mPropDesc2 = new PropertyDesc();
      mPropDesc2.setName( "MyPropDesc2" );
      mPropDesc2.setToken( "MyPropDesc2Token" );
      mPropDesc2.setHelp( "Help me! I'm meeelllltiiiinng!" );
      mPropDesc2.setValType( ValType.BOOL );
   }

   //--------------------------------------------------------------------------
   // Creators
   //--------------------------------------------------------------------------
   /**
    * Test constructor.
    */
   public void testConstructorPropertyDesc()
   {
      Property p = new Property(mPropDesc1);
      assertEquals(p.getDesc(), mPropDesc1);
      assertEquals(p.getNum(), 1);
   }

   /**
    * Test copy constructor.
    */
   public void testCopyConstructor()
   {
      Property p1 = new Property(mPropDesc1);
      Property p2 = new Property(p1);
      assertEquals(p2.getDesc(), p1.getDesc());
      assertEquals(p2.getNum(), p1.getNum());
   }

   /**
    * Test setValue(String, int).
    */
   public void testSetValueString()
   {
      Property p1 = new Property(mPropDesc1);
      p1.setValue("blah", 0);
      assertEquals( p1.getValue(0).getString(), "blah");
      p1.setValue("blahblah", 0);
      assertEquals(p1.getValue(0).getString(), "blahblah");

      // property desc for this property only takes one val
      p1.setValue("blahblahblah", 1);
      assertEquals(p1.getValue(1), null);
   }

   /**
    * Test setValue(boolean, int).
    */
   public void testSetValueBool()
   {
      Property p1 = new Property(mPropDesc2);
      p1.setValue(true, 0);
      assertEquals( p1.getValue(0).getBoolean(), true);
      p1.setValue(false, 0);
      assertEquals(p1.getValue(0).getBoolean(), false);

      // property desc for this property only takes one val
      p1.setValue(true, 1);
      assertEquals(p1.getValue(1), null);
   }

   /**
    * Test setValue(VarValue, int).
    */
   public void testSetValueVarValue()
   {
      Property p1 = new Property(mPropDesc2);
      VarValue vv = new VarValue(true);
      p1.setValue(vv, 0);
      assertEquals( p1.getValue(0).getBoolean(), true);
      vv = new VarValue(false);
      p1.setValue(vv, 0);
      assertEquals(p1.getValue(0).getBoolean(), false);

      // property desc for this property only takes one val
      vv = new VarValue(true);
      p1.setValue(vv, 1);
      assertEquals(p1.getValue(1), null);
   }

   /**
    * Tests equals.
    */
   public void testEquals()
   {
      Property p1 = new Property(mPropDesc1);
      Property p2 = new Property(p1);

      assertTrue(p1.equals(p2));
      assertTrue(p2.equals(p1));

      // vary value
      p2.setValue("asdf", 0);
      assertTrue(! p1.equals(p2));
      assertTrue(! p2.equals(p1));
   }

   /**
    * Test XML toString.
    */
   public void testToString()
   {
      Property p1 = new Property(mPropDesc1);
      p1.setValue("aaaahhhhh", 0);
      String rep = p1.toString();
      String expected = "<MyPropDescToken>\"aaaahhhhh\" </MyPropDescToken>\n";
      assertEquals(rep, expected);
   }
}
