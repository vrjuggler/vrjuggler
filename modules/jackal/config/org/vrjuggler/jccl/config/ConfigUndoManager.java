package org.vrjuggler.jccl.config;

import javax.swing.undo.*;

/**
 * Allows the user to undo/redo changes made to ConfigElements.
 */
public class ConfigUndoManager extends UndoManager
{
   /**
    * This class is a singleton, use instance() instead.
    */
   protected ConfigUndoManager ()
   {
   
   }


   /**
    * Gets the singleton instance of this class. This implementation is thread
    * safe.
    */
   public static ConfigUndoManager instance ()
   {
      synchronized (ConfigUndoManager.class)
      {
         if ( mInstance == null )
         {
            mInstance = new ConfigUndoManager();
         }
      }
      return mInstance;
   }
   private static ConfigUndoManager mInstance = null;
}
