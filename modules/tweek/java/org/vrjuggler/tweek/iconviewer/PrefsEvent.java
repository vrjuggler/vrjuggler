package org.vrjuggler.tweek.iconviewer;

import java.util.EventObject;


public class PrefsEvent extends EventObject
{
   public PrefsEvent(Object source, boolean smallGui)
   {
      super(source);
      this.smallGui = smallGui;
   }

   public boolean isSmallGui()
   {
      return smallGui;
   }

   private boolean smallGui;
}