package test;

import java.util.*;
import junit.framework.*;
import org.vrjuggler.jccl.config.*;

/**
 * Test the PropertyDesc class.
 */
public class PropertyDescTest
   extends TestCase
{
   public PropertyDescTest( String name )
   {
      super( name );
   }

   /**
    * Tests get/setName() methods.
    */
   public void testName()
   {
      PropertyDesc d = new PropertyDesc();
      d.setName( "JimmyJohns" );
      assertEquals( d.getName(), "JimmyJohns" );

      d = new PropertyDesc();
      d.setName( "" );
      assertEquals( d.getName(), "" );
   }

   /**
    * Tests get/setToken() methods.
    */
   public void testToken()
   {
      PropertyDesc d = new PropertyDesc();
      d.setToken( "SomeToken" );
      assertEquals( d.getToken(), "SomeToken" );

      d = new PropertyDesc();
      d.setToken( "" );
      assertEquals( d.getToken(), "" );
   }

   /**
    * Tests get/setHelp() methods.
    */
   public void testHelp()
   {
      PropertyDesc d = new PropertyDesc();
      d.setHelp( "Help me test successfully!" );
      assertEquals( d.getHelp(), "Help me test successfully!" );

      d = new PropertyDesc();
      d.setHelp( "" );
      assertEquals( d.getHelp(), "" );
   }

   /**
    * Tests get/setValType() methods.
    */
   public void testValType()
   {
      PropertyDesc d = new PropertyDesc();
      d.setValType( ValType.STRING );
      assertEquals( d.getValType(), ValType.STRING );

      d = new PropertyDesc();
      d.setValType( ValType.INT );
      assertEquals( d.getValType(), ValType.INT );
   }

   /**
    * Tests get/setHasVariableNumberOfValues() methods.
    */
   public void testHasVariableNumberOfValues()
   {
      PropertyDesc d = new PropertyDesc();
      d.setHasVariableNumberOfValues( true );
      assertEquals( d.hasVariableNumberOfValues(), true );

      d = new PropertyDesc();
      d.setHasVariableNumberOfValues( false );
      assertEquals( d.hasVariableNumberOfValues(), false );
   }

   /**
    * Tests get/setUserLevel() methods.
    */
   public void testUserLevel()
   {
      PropertyDesc d = new PropertyDesc();
      d.setUserLevel( 1 );
      assertEquals( d.getUserLevel(), 1 );

      d = new PropertyDesc();
      d.setUserLevel( 100 );
      assertEquals( d.getUserLevel(), 100 );
   }

   /**
    * Tests value label operations.
    */
   public void testValueLabels()
   {
      PropertyDesc d = new PropertyDesc();
      
      // should initially be empty
      assertEquals( d.getValueLabelsSize(), 1 );

      // request a label outside our range
      assertEquals( d.getValueLabel(1), "" );

      // add a label
      d.appendValueLabel( "SomeLabel" );
      assertEquals( d.getValueLabelsSize(), 2 );
      assertEquals( d.getValueLabel(1), "SomeLabel" );

      // add another label
      d.appendValueLabel( "AnotherLabel" );
      assertEquals( d.getValueLabelsSize(), 3 );
      assertEquals( d.getValueLabel(1), "SomeLabel" );
      assertEquals( d.getValueLabel(2), "AnotherLabel" );

      // replace the labels
      ArrayList list = new ArrayList();
      list.add( "Me" );
      list.add( "Myself" );
      list.add( "I" );
      d.setValueLabels( list );
      assertEquals( d.getValueLabelsSize(), 3 );
      assertEquals( d.getValueLabel(0), "Me" );
      assertEquals( d.getValueLabel(1), "Myself" );
      assertEquals( d.getValueLabel(2), "I" );

      // replace labels with empty list
      list = new ArrayList();
      d.setValueLabels( list );
      assertEquals( d.getValueLabelsSize(), 0 );
      assertEquals( d.getValueLabel(0), "" );
   }

   /**
    * Tests DescEnum operations.
    */
   public void testEnumerations()
   {
      PropertyDesc d = new PropertyDesc();
      d.setValType( ValType.STRING );
      List list = new ArrayList();
      

      // should initially be empty
      assertEquals(d.getNumEnums(), 0);
      list = d.getEnums();
      assertEquals(list.size(), 0);

      // add an enum
      DescEnum enum1 = new DescEnum("Enum1", ValType.STRING, "Val1");
      d.addEnum(enum1);
      assertEquals(d.getNumEnums(), 1);
      assertTrue(d.getEnumAt(0).equals(enum1));
      list = d.getEnums();
      assertTrue(list.get(0).equals(enum1));
//      assertTrue(list.get(0).getName().equals(enum1.getName()));
//      assertTrue(list.get(0).getValue().equals(enum1.getValue()));

      // add another enum
      DescEnum enum2 = new DescEnum("Enum2", ValType.STRING, "Val2");
      d.addEnum(enum2);
      assertEquals(d.getNumEnums(), 2);
      assertTrue(d.getEnumAt(0).equals(enum1));
      assertTrue(d.getEnumAt(1).equals(enum2));
      list = d.getEnums();
      assertTrue(list.get(0).equals(enum1));
      assertTrue(list.get(1).equals(enum2));

      // remove an enum
      d.removeEnum(enum2);
      assertEquals(d.getNumEnums(), 1);
      assertTrue(d.getEnumAt(0).equals(enum1));
      list = d.getEnums();
      assertTrue(list.get(0).equals(enum1));
   }
}
