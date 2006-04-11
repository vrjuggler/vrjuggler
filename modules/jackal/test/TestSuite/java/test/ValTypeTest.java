package test;

import junit.framework.*;
import org.vrjuggler.jccl.config.*;

/**
 * Tests the ValType class.
 */
public class ValTypeTest
   extends TestCase
{
   public ValTypeTest( String name )
   {
      super( name );
   }

   public void testToString()
   {
      assertEquals( ValType.STRING.toString(),        ConfigTokens.string_TOKEN );
      assertEquals( ValType.FLOAT.toString(),         ConfigTokens.float_TOKEN );
      assertEquals( ValType.INT.toString(),           ConfigTokens.int_TOKEN );
      assertEquals( ValType.BOOL.toString(),          ConfigTokens.bool_TOKEN );
      assertEquals( ValType.CHUNK.toString(),         ConfigTokens.chunk_TOKEN );
      assertEquals( ValType.EMBEDDEDCHUNK.toString(), ConfigTokens.embeddedchunk_TOKEN );
      assertEquals( ValType.INVALID.toString(),       ConfigTokens.invalid_TOKEN );
   }

   public void testGetValType()
   {
      assertEquals( ValType.getValType(ConfigTokens.string_TOKEN),        ValType.STRING );
      assertEquals( ValType.getValType(ConfigTokens.float_TOKEN),         ValType.FLOAT );
      assertEquals( ValType.getValType(ConfigTokens.int_TOKEN),           ValType.INT );
      assertEquals( ValType.getValType(ConfigTokens.bool_TOKEN),          ValType.BOOL );
      assertEquals( ValType.getValType(ConfigTokens.chunk_TOKEN),         ValType.CHUNK );
      assertEquals( ValType.getValType(ConfigTokens.embeddedchunk_TOKEN), ValType.EMBEDDEDCHUNK );
      assertEquals( ValType.getValType(ConfigTokens.invalid_TOKEN),       ValType.INVALID );
   }
}
