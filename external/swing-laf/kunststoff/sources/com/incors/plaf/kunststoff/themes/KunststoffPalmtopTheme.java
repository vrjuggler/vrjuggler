
package com.incors.plaf.kunststoff.themes;

import java.awt.Font;
import javax.swing.plaf.ColorUIResource;
import javax.swing.plaf.FontUIResource;
import javax.swing.UIDefaults;
import javax.swing.UIManager;

/**
 */
public class KunststoffPalmtopTheme extends com.incors.plaf.kunststoff.KunststoffTheme
{
   // primary colors
   private final ColorUIResource primary1 = new ColorUIResource(32, 32, 32);
   private final ColorUIResource primary2 = new ColorUIResource(160, 160, 180);
   private final ColorUIResource primary3 = new ColorUIResource(200, 200, 224);

   // secondary colors
   private final ColorUIResource secondary1 = new ColorUIResource(130, 130, 130);
   private final ColorUIResource secondary2 = new ColorUIResource(180, 180, 180);
   private final ColorUIResource secondary3 = new ColorUIResource(224, 224, 224);
   //  private final ColorUIResource secondary3 = new ColorUIResource(224, 224, 224);

   // fonts
   private FontUIResource controlFont;
   private FontUIResource menuFont;
   private FontUIResource windowTitleFont;
   private FontUIResource monospacedFont;

   /**
    * Creates this Theme
    */
   public KunststoffPalmtopTheme()
   {
      menuFont        = new FontUIResource("Tahoma", Font.PLAIN, 10);
      controlFont     = new FontUIResource("Tahoma", Font.PLAIN, 9);
      windowTitleFont = new FontUIResource("Tahoma", Font.BOLD, 10);
      monospacedFont  = new FontUIResource("Monospaced", Font.PLAIN, 9);
   }

   public String getName()
   {
      return "Palmtop";
   }

   /**
    * The Font of Labels in many cases
    */
   public FontUIResource getControlTextFont()
   {
      return controlFont;
   }

   /**
    * The Font of Menus and MenuItems
    */
   public FontUIResource getMenuTextFont()
   {
      return menuFont;
   }

   /**
    * The Font of Nodes in JTrees
    */
   public FontUIResource getSystemTextFont()
   {
      return controlFont;
   }

   /**
    * The Font in TextFields, EditorPanes, etc.
    */
   public FontUIResource getUserTextFont()
   {
      return controlFont;
   }

   /**
    * The Font of the Title of JInternalFrames
    */
   public FontUIResource getWindowTitleFont()
   {
      return windowTitleFont;
   }

   protected ColorUIResource getPrimary1()
   {
      return primary1;
   }

   protected ColorUIResource getPrimary2()
   {
      return primary2;
   }

   protected ColorUIResource getPrimary3()
   {
      return primary3;
   }

   protected ColorUIResource getSecondary1()
   {
      return secondary1;
   }

   protected ColorUIResource getSecondary2()
   {
      return secondary2;
   }

   protected ColorUIResource getSecondary3()
   {
      return secondary3;
   }

   public void addCustomEntriesToTable(UIDefaults table)
   {
      super.addCustomEntriesToTable(table);
      UIManager.getDefaults().put("PasswordField.font", monospacedFont);
      UIManager.getDefaults().put("TextArea.font", monospacedFont);
      UIManager.getDefaults().put("TextPane.font", monospacedFont);
      UIManager.getDefaults().put("EditorPane.font", monospacedFont);
   }
}
