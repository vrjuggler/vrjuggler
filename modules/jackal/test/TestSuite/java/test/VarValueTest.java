package test;

import junit.framework.*;
import org.vrjuggler.jccl.config.*;

/**
 * Tests the VarValue class.
 */
public class VarValueTest
   extends TestCase
{
   public VarValueTest( String name )
   {
      super( name );
   }

   /**
    * Test the boolean internals.
    */
   public void testBool()
   {
      VarValue t = new VarValue( true );
      assertEquals( t.getValType(), ValType.BOOL );
      assertEquals( t.toString(), "1" );
      assertEquals( t.getBoolean(), true );
      assertTrue( t.get() instanceof Boolean );
      assertEquals( ((Boolean)t.get()).booleanValue(), true );
      t.set( false );
      assertEquals( t.getBoolean(), false );

      VarValue f = new VarValue( false );
      assertEquals( f.getValType(), ValType.BOOL );
      assertEquals( f.toString(), "0" );
      assertEquals( f.getBoolean(), false );
      assertTrue( f.get() instanceof Boolean );
      assertEquals( ((Boolean)f.get()).booleanValue(), false );
      f.set( true );
      assertEquals( f.getBoolean(), true );
   }

   /**
    * Test the float internals.
    */
   public void testFloat()
   {
      VarValue t = new VarValue( 1.0f );
      assertEquals( t.getValType(), ValType.FLOAT );
      assertEquals( t.toString(), String.valueOf(1.0f) );
      assertEquals( t.getFloat(), 1.0f, 0.0f );
      assertTrue( t.get() instanceof Float );
      assertEquals( ((Float)t.get()).floatValue(), 1.0f, 0.0f );
      t.set( 2.0f );
      assertEquals( t.getFloat(), 2.0f, 0.0f );

      VarValue f = new VarValue( -50.0f );
      assertEquals( f.getValType(), ValType.FLOAT );
      assertEquals( f.toString(), String.valueOf(-50.0f) );
      assertEquals( f.getFloat(), -50.0f, 0.0f );
      assertTrue( f.get() instanceof Float );
      assertEquals( ((Float)f.get()).floatValue(), -50.0f, 0.0f );
      f.set( -1234.56789f );
      assertEquals( f.getFloat(), -1234.56789f, 0.0f );
   }

   /**
    * Test the int internals.
    */
   public void testInt()
   {
      VarValue t = new VarValue( 1 );
      assertEquals( t.getValType(), ValType.INT );
      assertEquals( t.toString(), String.valueOf(1) );
      assertEquals( t.getInt(), 1 );
      assertTrue( t.get() instanceof Integer );
      assertEquals( ((Integer)t.get()).intValue(), 1 );
      t.set( 2 );
      assertEquals( t.getInt(), 2 );

      VarValue f = new VarValue( -50 );
      assertEquals( f.getValType(), ValType.INT );
      assertEquals( f.toString(), String.valueOf(-50) );
      assertEquals( f.getInt(), -50 );
      assertTrue( f.get() instanceof Integer );
      assertEquals( ((Integer)f.get()).intValue(), -50 );
      f.set( -1234567 );
      assertEquals( f.getInt(), -1234567 );
   }

   /**
    * Test the string internals.
    */
   public void testString()
   {
      VarValue t = new VarValue( "whassup!" );
      assertEquals( t.getValType(), ValType.STRING );
      assertEquals( t.toString(), "whassup!" );
      assertEquals( t.getString(), "whassup!" );
      assertTrue( t.get() instanceof String );
      assertEquals( (String)t.get(), "whassup!" );
      t.set( "doing tests" );
      assertEquals( t.getString(), "doing tests" );

      VarValue f = new VarValue( "" );
      assertEquals( f.getValType(), ValType.STRING );
      assertEquals( f.toString(), "" );
      assertEquals( f.getString(), "" );
      assertTrue( f.get() instanceof String );
      assertEquals( (String)f.get(), "" );
      f.set( "writing tests is fun" );
      assertEquals( f.getString(), "writing tests is fun" );
   }

   /**
    * Tests the string set specially since it converts the valtype as needed.
    */
   public void testSetString()
   {
      // boolean
      VarValue b = new VarValue( false );
      b.set( "True" );
      assertEquals( b.getBoolean(), true );
      b.set( "blah" );
      assertEquals( b.getBoolean(), false );

      // float
      VarValue f = new VarValue( 3.141579f );
      f.set( "-1980" );
      assertEquals( f.getFloat(), -1980f, 0f );
      try
      {
         f.set( "blah" );
         assertTrue( "shouldn't be able to set a FLOAT type to 'blah'", false );
      }
      catch ( VarValueException vve )
      {
         assertTrue( true );
      }

      // integer
      VarValue i = new VarValue( 1492 );
      i.set( "-6845" );
      assertEquals( i.getInt(), -6845 );
      try
      {
         i.set( "blah" );
         assertTrue( "shouldn't be able to set an INT type to 'blah'", false );
      }
      catch ( VarValueException vve )
      {
         assertTrue( true );
      }

      // string
      VarValue s = new VarValue( "cherrycoke" );
      s.set( "mountaindew" );
      assertEquals( s.getString(), "mountaindew" );
   }

   /**
    * Test the equals() method.
    */
   public void testEquals()
   {
      VarValue i1 = new VarValue( 20 );
      VarValue i2 = new VarValue( 30 );
      assertTrue( ! i1.equals(i2) );
      i2.set( 20 );
      assertTrue( i1.equals(i2) );

      VarValue f1 = new VarValue( 20.0f );
      VarValue f2 = new VarValue( 30.0f );
      assertTrue( ! f1.equals(f2) );
      f2.set( 20.0f );
      assertTrue( f1.equals(f2) );

      VarValue b1 = new VarValue( true );
      VarValue b2 = new VarValue( false );
      assertTrue( ! b1.equals(b2) );
      b2.set( true );
      assertTrue( b1.equals(b2) );

      VarValue s1 = new VarValue( "aba" );
      VarValue s2 = new VarValue( "cac" );
      assertTrue( ! s1.equals(s2) );
      s2.set( "aba" );
      assertTrue( s1.equals(s2) );
   }
}
