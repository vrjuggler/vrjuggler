package org.vrjuggler.jccl.config;

import java.util.*;
import javax.swing.event.EventListenerList;

import org.vrjuggler.jccl.config.event.*;
import javax.swing.undo.*;

public class ConfigElementPropertyEdit extends AbstractUndoableEdit
{
   public ConfigElementPropertyEdit(ConfigElement elm, String name, int index, Object old_val, Object new_val)
   {
      super();
      mElement = elm;
      mPropName = name;
      mPropIndex = index;
      mOldValue = old_val;
      mNewValue = new_val;


      
   }

   public void undo() throws CannotUndoException 
   {      
      System.out.println("undoing..");
      super.undo();
      System.out.println("undoing...");
      
      mElement.setProperty(mPropName, mPropIndex, mOldValue);
   }

   public void redo() throws CannotRedoException
   {
      System.out.println("redoing..");
      super.redo();
      System.out.println("redoing...");
      
      mElement.setProperty(mPropName, mPropIndex, mNewValue);
   }

   private String          mPropName;
   private int             mPropIndex;
   private Object          mNewValue;
   private Object          mOldValue;
   private ConfigElement   mElement;
}
