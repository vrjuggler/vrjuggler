package org.vrjuggler.jccl.editors;

import javax.swing.JPanel;

public class PropertyEditorPanelManager
{
    public static void registerEditor(Class targetType, Class editorClass) 
    {
	if (editorClass == null) 
        {
	    registry.remove(targetType);
	} 
        else 
        {
	    registry.put(targetType, editorClass);
	}
    }

   public static synchronized PropertyEditorPanel findEditor(Class targetType)
   {
      Class editorClass = (Class)registry.get(targetType);
      if (editorClass != null) 
      {
         try 
         {
             Object o = editorClass.newInstance();
             return (PropertyEditorPanel)o;
         } 
         catch (Exception ex) 
         {
             System.err.println("Couldn't instantiate type editor \"" +
                     editorClass.getName() + "\" : " + ex);
         }
      }
      System.out.println("Could not find an editor for class: " + targetType.toString());
      // We couldn't find a suitable Editor.
      return null;
   }
   private static java.util.Hashtable registry = new java.util.Hashtable();
}
