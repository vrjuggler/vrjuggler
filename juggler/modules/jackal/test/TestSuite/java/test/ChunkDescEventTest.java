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
   public static final int TOKEN_CHANGED           = 2;
   public static final int HELP_CHANGED            = 4;
   public static final int CATEGORY_ADDED          = 8;
   public static final int CATEGORY_REMOVED        = 16;
   public static final int PROPERTY_DESC_CHANGED   = 32;
   public static final int PROPERTY_DESC_ADDED     = 64;
   public static final int PROPERTY_DESC_REMOVED   = 128;

   class ChangeListener
      implements ChunkDescListener
   {
      public void nameChanged(ChunkDescEvent evt)           { fired(NAME_CHANGED, evt); }
      public void tokenChanged(ChunkDescEvent evt)          { fired(TOKEN_CHANGED, evt); }
      public void helpChanged(ChunkDescEvent evt)           { fired(HELP_CHANGED, evt); }
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
      elt.setAttribute(ConfigTokens.name_TOKEN, "TestDesc");
      elt.setAttribute(ConfigTokens.token_TOKEN, "test_desc");

      Element cat_elt = new Element(ConfigTokens.category_TOKEN);
      cat_elt.setText("TestCategory");
      elt.addContent(cat_elt);

      Element prop_elt = new Element(ConfigTokens.property_desc_TOKEN);
      prop_elt.setAttribute(ConfigTokens.name_TOKEN, "TestPropertyDesc");
      prop_elt.setAttribute(ConfigTokens.token_TOKEN, "test_prop_desc");
      prop_elt.setAttribute(ConfigTokens.type_TOKEN, ConfigTokens.string_TOKEN);
      elt.addContent(prop_elt);

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
    * Test token change.
    */
   public void testTokenChange()
   {
      mDesc.setToken("new_token");
      assertTrue(mListener.wasFired(TOKEN_CHANGED));
   }

   /**
    * Test help change.
    */
   public void testHelpChange()
   {
      mDesc.setHelp("Yo ... here's some new help.");
      assertTrue(mListener.wasFired(HELP_CHANGED));
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
      assertEquals(mListener.getEvent().getValue(), "NewCategory");

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
      assertEquals(mListener.getEvent().getValue(), "TestCategory");
   }

   /**
    * Test property desc changed.
    */
   public void testPropertyDescChanged()
   {
      mDesc.getPropertyDesc("test_prop_desc").setToken("yo");
      assertTrue(mListener.wasFired(PROPERTY_DESC_CHANGED));
   }

   /**
    * Test property desc added.
    */
   public void testPropertyDescAdded()
   {
      PropertyDesc prop_desc = new PropertyDesc();
      prop_desc.setName("New Prop");
      prop_desc.setToken("new_prop");
      prop_desc.setValType(ValType.STRING);
      mDesc.addPropertyDesc(prop_desc);
      assertTrue(mListener.wasFired(PROPERTY_DESC_ADDED));
   }

   /**
    * Test property desc removed.
    */
   public void testPropertyDescRemoved()
   {
      mDesc.removePropertyDesc(mDesc.getPropertyDesc(0));
      assertTrue(mListener.wasFired(PROPERTY_DESC_REMOVED));
   }

   private ChangeListener mListener;
   private ChunkDesc mDesc;
}
