package test;

import junit.framework.*;
import org.jdom.Element;
import org.vrjuggler.jccl.config.*;

/**
 * Test the ChunkDesc class. Because ChunkDesc and ConfigChunk are tightly
 * coupled, some of these tests may fail if ConfigChunk is broken.
 */
public class ChunkDescEventTest
   extends TestCase
{
   public static final int NAME_CHANGED            = 1;
   public static final int CATEGORY_ADDED          = 2;
   public static final int CATEGORY_REMOVED        = 4;
   public static final int PROPERTY_DESC_CHANGED   = 8;
   public static final int PROPERTY_DESC_ADDED     = 16;
   public static final int PROPERTY_DESC_REMOVED   = 32;

   class ChangeListener
      implements ChunkDescListener
   {
      public void nameChanged(ChunkDescEvent evt)           { fired(NAME_CHANGED, evt); }
      public void categoryAdded(ChunkDescEvent evt)         { fired(CATEGORY_ADDED, evt); }
      public void categoryRemoved(ChunkDescEvent evt)       { fired(CATEGORY_REMOVED, evt); }
      public void propertyDescChanged(ChunkDescEvent evt)   { fired(PROPERTY_DESC_CHANGED, evt); }
      public void propertyDescAdded(ChunkDescEvent evt)     { fired(PROPERTY_DESC_ADDED, evt); }
      public void propertyDescRemoved(ChunkDescEvent evt)   { fired(PROPERTY_DESC_REMOVED, evt); }

      private void fired(int type, ChunkDescEvent evt)
      {
         mFired |= type;
         mEvent = evt;
      }

      public void reset()
      {
         mFired = 0;
         mEvent = null;
      }

      public boolean wasFired(int type)
      {
         if ((mFired & type) == type)
         {
            return true;
         }
         else
         {
            return false;
         }
      }

      public ChunkDescEvent getEvent()
      {
         return mEvent;
      }

      private int mFired = 0;
      private ChunkDescEvent mEvent;
   }


   public ChunkDescEventTest(String name)
   {
      super(name);
   }

   public void setUp()
   {
      // Setup the DOM element for the Chunk Desc
      Element elt = new Element(ConfigTokens.chunk_desc_TOKEN);
      elt.setAttribute("name", "TestDesc");

      Element cat_elt = new Element(ConfigTokens.category_TOKEN);
      cat_elt.setText("TestCategory");
      elt.addContent(cat_elt);

      // Create the chunk desc and its listener
      mListener = new ChangeListener();
      mDesc = new ChunkDesc(elt);
      mDesc.addChunkDescListener(mListener);
   }

   /**
    * Test name change.
    */
   public void testNameChange()
   {
      mDesc.setName("NewName");
      assertTrue(mListener.wasFired(NAME_CHANGED));
   }

   /**
    * Test category add.
    */
   public void testCategoryAdd()
   {
      // Successful insertion
      mDesc.addCategory("NewCategory");
      assertTrue(mListener.wasFired(CATEGORY_ADDED));
      assertEquals(mListener.getEvent().getIndex(), 1);

      // Unsuccessful insertion
      mListener.reset();
      mDesc.addCategory("NewCategory");
      assertTrue(!mListener.wasFired(CATEGORY_ADDED));
   }

   /**
    * Test category removed.
    */
   public void testCategoryRemoved()
   {
      // Unsuccessful removal
      mDesc.removeCategory("NewCategory");
      assertTrue(!mListener.wasFired(CATEGORY_REMOVED));

      // Successful removal
      mListener.reset();
      mDesc.removeCategory("TestCategory");
      assertTrue(mListener.wasFired(CATEGORY_REMOVED));
      assertEquals(mListener.getEvent().getIndex(), 0);
   }



   private ChangeListener mListener;
   private ChunkDesc mDesc;
}
