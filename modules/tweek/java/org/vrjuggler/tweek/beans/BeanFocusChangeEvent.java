package org.vrjuggler.tweek.beans;


/**
 * @since 1.0
 */
public class BeanFocusChangeEvent extends java.util.EventObject
{
   public static final int BEAN_UNFOCUSED = 0;
   public static final int BEAN_FOCUSED   = 1;

   public BeanFocusChangeEvent (Object source, PanelBean _bean, int type)
   {
      super(source);
      bean      = _bean;
      focusType = type;
   }

   public PanelBean getBean ()
   {
      return bean;
   }

   public int getFocusType ()
   {
      return focusType;
   }

   private PanelBean bean = null;
   private int       focusType;
}