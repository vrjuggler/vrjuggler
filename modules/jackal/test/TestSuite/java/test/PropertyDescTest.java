package test;

import java.util.ArrayList;
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
    * Tests get/setNumAllowed() methods.
    */
   public void testNumAllowed()
   {
      PropertyDesc d = new PropertyDesc();
      d.setNumAllowed( 20 );
      assertEquals( d.getNumAllowed(), 20 );

      d = new PropertyDesc();
      d.setNumAllowed( -1 );
      assertEquals( d.getNumAllowed(), -1 );
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
      assertEquals( d.getValueLabelsSize(), 0 );

      // request a label outside our range
      assertEquals( d.getValueLabel(0), "" );

      // add a label
      d.appendValueLabel( "SomeLabel" );
      assertEquals( d.getValueLabelsSize(), 1 );
      assertEquals( d.getValueLabel(0), "SomeLabel" );

      // add another label
      d.appendValueLabel( "AnotherLabel" );
      assertEquals( d.getValueLabelsSize(), 2 );
      assertEquals( d.getValueLabel(0), "SomeLabel" );
      assertEquals( d.getValueLabel(1), "AnotherLabel" );

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
      DescEnum[] deArray = null;

      // should initially be empty
      assertEquals( d.getEnumerationsSize(), 0 );
      deArray = d.getEnumerations();
      assertEquals( deArray.length, 0 );

      // add an enum
      d.appendEnumeration( "Enum1", "Val1" );
      assertEquals( d.getEnumerationsSize(), 1 );
      assertTrue( d.getEnumAtIndex(0).equals(new DescEnum("Enum1", new VarValue("Val1"))) );
      deArray = d.getEnumerations();
      assertTrue( deArray[0].equals(new DescEnum("Enum1", new VarValue("Val1"))) );

      // add another enum
      d.appendEnumeration( "Enum2", "Val2" );
      assertEquals( d.getEnumerationsSize(), 2 );
      assertTrue( d.getEnumAtIndex(0).equals(new DescEnum("Enum1", new VarValue("Val1"))) );
      assertTrue( d.getEnumAtIndex(1).equals(new DescEnum("Enum2", new VarValue("Val2"))) );
      deArray = d.getEnumerations();
      assertTrue( deArray[0].equals(new DescEnum("Enum1", new VarValue("Val1"))) );
      assertTrue( deArray[1].equals(new DescEnum("Enum2", new VarValue("Val2"))) );

      // replace the enums
      ArrayList list = new ArrayList();
      list.add( new DescEnum("Me", new VarValue("Me")) );
      list.add( new DescEnum("Myself", new VarValue("Myself")) );
      list.add( new DescEnum("I", new VarValue("I")) );
      d.setEnumerations( list );
      assertEquals( d.getEnumerationsSize(), 3 );
      assertTrue( d.getEnumAtIndex(0).equals(new DescEnum("Me", new VarValue("Me"))) );
      assertTrue( d.getEnumAtIndex(1).equals(new DescEnum("Myself", new VarValue("Myself"))) );
      assertTrue( d.getEnumAtIndex(2).equals(new DescEnum("I", new VarValue("I"))) );
      deArray = d.getEnumerations();
      assertTrue( deArray[0].equals(new DescEnum("Me", new VarValue("Me"))) );
      assertTrue( deArray[1].equals(new DescEnum("Myself", new VarValue("Myself"))) );
      assertTrue( deArray[2].equals(new DescEnum("I", new VarValue("I"))) );

      // replace with the empty list
      list = new ArrayList();
      d.setEnumerations( list );
      assertEquals( d.getEnumerationsSize(), 0 );
      deArray = d.getEnumerations();
      assertEquals( deArray.length, 0 );
   }
}
