package test;

import junit.framework.*;
import org.jdom.Element;
import org.vrjuggler.jccl.config.*;

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
      assertTrue( c.propertyDescsSize() == 0 );
      assertEquals( c.getName(), "" );
      assertEquals( c.getToken(), "" );
      assertEquals( c.getHelp(), "" );
   }

   /**
    * Test constructor w/ JDOM element arg.
    */
   public void testConstructorString()
   {
      String s = "stuff";
      ChunkDesc c = new ChunkDesc();
      c.setToken( s );
      assertTrue( c.propertyDescsSize() == 0 );
      assertEquals( c.getName(), s );
      assertEquals( c.getToken(), s );
      assertEquals( c.getHelp(), "" );
   }

   /**
    * Test cloning capabilities.
    */
   public void testClone()
   {
      ChunkDesc c1 = new ChunkDesc();
      ChunkDesc c2 = (ChunkDesc)c1.clone();
      
      assertEquals( c1.propertyDescsSize(), c2.propertyDescsSize() );
      assertEquals( c1.getName(), c2.getName() );
      assertEquals( c1.getToken(), c2.getToken() );
      assertEquals( c1.getHelp(), c2.getHelp() );
   }

   //--------------------------------------------------------------------------
   // Getters and setters
   //--------------------------------------------------------------------------
   public void testGetName()
   {
      ChunkDesc c = new ChunkDesc();
      assertEquals( c.getName(), "" );

      String name = "stuff";
      c = new ChunkDesc();
      c.setName(name);
      assertEquals( c.getName(), name );
   }

   public void testSetName()
   {
      ChunkDesc c = new ChunkDesc();
      String s = "blah";
      c.setName( s );
      assertEquals( c.getName(), s );
   }

   public void testGetToken()
   {
      ChunkDesc c = new ChunkDesc();
      assertEquals( c.getToken(), "" );

      String t = "mytoken";
      c = new ChunkDesc();
      c.setToken( t );
      assertEquals( c.getToken(), t );
   }

   public void testSetToken()
   {
      ChunkDesc c = new ChunkDesc();
      String t = "blah";
      c.setToken( t );
      assertEquals( c.getToken(), t );
   }

   public void testGetVersion()
   {
      ChunkDesc c = new ChunkDesc();
      assertEquals(c.getVersion(), "1.1");

      c.setVersion("4.5");
      assertEquals(c.getVersion(), "4.5");
   }

   public void testSetVersion()
   {
      ChunkDesc c = new ChunkDesc();
      c.setVersion("34.5");
      assertEquals(c.getVersion(), "34.5");
   }

   // test set before get for help
   public void testSetHelp()
   {
      ChunkDesc c = new ChunkDesc();
      String s = "helpme";
      c.setHelp( s );
      assertEquals( c.getHelp(), s );
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
      p.setValType(ValType.STRING);
      ChunkDesc c = new ChunkDesc();
      c.addPropertyDesc( p );
      assertEquals(c.getPropertyDesc(0), p);
//      assertTrue(c.getPropertyDesc(0).getName().equals(p.getName()));
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
      PropertyDesc pd = new PropertyDesc();
      pd.setName("Stuff");
      pd.setValType(ValType.STRING);
      c2.addPropertyDesc(pd);
      assertTrue( ! c1.equals(c2) );
   }
}
