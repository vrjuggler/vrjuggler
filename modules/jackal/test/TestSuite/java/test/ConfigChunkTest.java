package test;

import junit.framework.*;
import org.jdom.Element;
import org.vrjuggler.jccl.config.*;

/**
 * Test the ConfigChunk class.
 */
 public class ConfigChunkTest
   extends TestCase
{
   public ConfigChunkTest(String name)
   {
      super(name);
   }

   public void setUp()
   {
      // Create a simple chunk desc we can use for the tests
      mDesc = new ChunkDesc();
      mDesc.setName("Test");
      mDesc.setToken("test");
      PropertyDesc prop = new PropertyDesc();
      prop.setName("Property 1");
      prop.setToken("prop1");
      mDesc.addPropertyDesc(prop);
   }

   public void testConstructor()
   {
      ConfigChunk c = new ConfigChunk(mDesc);
      assertEquals(c.getDesc(), mDesc);
   }

   public void testGetName()
   {
      ConfigChunk c = new ConfigChunk(mDesc);
      assertEquals(c.getName(), "test");

      c.setName("stuff");
      assertEquals(c.getName(), "stuff");
   }

   public void testSetName()
   {
      ConfigChunk c = new ConfigChunk(mDesc);
      c.setName("blah");
      assertEquals(c.getName(), "blah");
   }

   public void testGetVersion()
   {
      ConfigChunk c = new ConfigChunk(mDesc);
      assertEquals(c.getVersion(), "1.1");

      c.setVersion("2.4");
      assertEquals(c.getVersion(), "2.4");
   }

   private ChunkDesc mDesc;
}
