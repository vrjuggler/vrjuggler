package test;

import junit.framework.*;
import VjConfig.*;

/**
 * Test the ChunkDesc class. Because ChunkDesc and ConfigChunk are tightly
 * coupled, some of these tests may fail if ConfigChunk is broken.
 */
public class ChunkDescTest
   extends TestCase
{
   public ChunkDescTest( String name )
   {
      super( name );
   }

   //--------------------------------------------------------------------------
   // Creators
   //--------------------------------------------------------------------------
   /**
    * Test constructor.
    */
   public void testConstructor()
   {
      ChunkDesc c = new ChunkDesc();
      assertTrue( c.propertyDescsSize() == 1 );
      assertEquals( c.name, "" );
      assertEquals( c.token, "" );
      assertEquals( c.help, "" );
   }

   /**
    * Test constructor w/ string arg.
    */
   public void testConstructorString()
   {
      String s = "stuff";
      ChunkDesc c = new ChunkDesc( s );
      assertTrue( c.propertyDescsSize() == 1 );
      assertEquals( c.name, s );
      assertEquals( c.token, s );
      assertEquals( c.help, "" );
   }

   /**
    * Test cloning capabilities.
    */
   public void testClone()
   {
      String name = "gnome";
      ChunkDesc c1 = new ChunkDesc( name );
      ChunkDesc c2 = (ChunkDesc)c1.clone();
      
      assertEquals( c1.propertyDescsSize(), c2.propertyDescsSize() );
      assertEquals( c1.name, c2.name );
      assertEquals( c1.token, c2.token );
      assertEquals( c1.help, c2.help );
   }

   //--------------------------------------------------------------------------
   // Getters and setters
   //--------------------------------------------------------------------------
   public void testGetName()
   {
      ChunkDesc c = new ChunkDesc();
      assertEquals( c.getName(), "" );

      String name = "stuff";
      c = new ChunkDesc( name );
      assertEquals( c.getName(), name );
   }

   public void testSetName()
   {
      ChunkDesc c = new ChunkDesc();
      String s = "blah";
      c.setName( s );
      assertEquals( c.name, s );
   }

   public void testGetToken()
   {
      ChunkDesc c = new ChunkDesc();
      assertEquals( c.getToken(), "" );

      String t = "mytoken";
      c = new ChunkDesc( t );
      assertEquals( c.getToken(), t );
   }

   public void testSetToken()
   {
      ChunkDesc c = new ChunkDesc();
      String t = "blah";
      c.setToken( t );
      assertEquals( c.token, t );
   }

   // test set before get for help
   public void testSetHelp()
   {
      ChunkDesc c = new ChunkDesc();
      String s = "helpme";
      c.setHelp( s );
      assertEquals( c.help, s );
   }

   public void testGetHelp()
   {
      ChunkDesc c = new ChunkDesc();
      String s = "helpme";
      assertEquals( c.getHelp(), "" );
      c.setHelp( s );
      assertEquals( c.getHelp(), s );
   }

   //--------------------------------------------------------------------------
   // PropertyDesc tests
   //--------------------------------------------------------------------------
   public void testAddPropertyDesc()
   {
      PropertyDesc p = new PropertyDesc();
      p.setName( "myproperty" );
      ChunkDesc c = new ChunkDesc();
      c.addPropertyDesc( p );
      assertEquals( c.getPropertyDesc( 1 ), p );
   }

   //--------------------------------------------------------------------------
   // Equality tests
   //--------------------------------------------------------------------------
   public void testEquals()
   {
      ChunkDesc c1 = new ChunkDesc();
      ChunkDesc c2 = (ChunkDesc)c1.clone();

      // vary name
      c2.setName( "blah" );
      assertTrue( ! c1.equals(c2) );

      // vary token
      c2 = (ChunkDesc)c1.clone();
      c2.setToken( "blah" );
      assertTrue( ! c1.equals(c2) );

      // vary help
      c2 = (ChunkDesc)c1.clone();
      c2.setHelp( "blah" );
      assertTrue( ! c1.equals(c2) );

      // vary property descs
      c2 = (ChunkDesc)c1.clone();
      c2.addPropertyDesc( new PropertyDesc() );
      assertTrue( ! c1.equals(c2) );
   }
}
