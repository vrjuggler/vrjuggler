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
      assertEquals( ValType.STRING.toString(),           "String" );
      assertEquals( ValType.FLOAT.toString(),            "Float" );
      assertEquals( ValType.INT.toString(),              "Int" );
      assertEquals( ValType.BOOL.toString(),             "Bool" );
      assertEquals( ValType.CHUNK.toString(),            "Chunk" );
      assertEquals( ValType.EMBEDDEDCHUNK.toString(),    "EmbeddedChunk" );
      assertEquals( ValType.INVALID.toString(),          "Invalid" );
   }

   public void testGetValType()
   {
      assertEquals( ValType.getValType("string"),        ValType.STRING );
      assertEquals( ValType.getValType("float"),         ValType.FLOAT );
      assertEquals( ValType.getValType("int"),           ValType.INT );
      assertEquals( ValType.getValType("bool"),          ValType.BOOL );
      assertEquals( ValType.getValType("chunk"),         ValType.CHUNK );
      assertEquals( ValType.getValType("embeddedchunk"), ValType.EMBEDDEDCHUNK );
      assertEquals( ValType.getValType("invalid"),       ValType.INVALID );
   }
}
