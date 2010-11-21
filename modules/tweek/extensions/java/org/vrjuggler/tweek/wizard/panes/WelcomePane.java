/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.wizard.panes;

import javax.swing.*;
import java.awt.*;

import org.vrjuggler.tweek.wizard.*;

/**
 * A pane used to welcome the user to a wizard. This is generally the first
 * pane you wish to show in a wizard.
 */
public class WelcomePane
   extends AbstractWizardStep
   implements WizardPane
{
   /**
    * Register ourself with the WizardStepFactory.
    */
   static
   {
      WizardStepFactory.registerWizardStep(WelcomePane.class);
   }

   /**
    * Creates a new WelcomePane with no text.
    */
   public WelcomePane()
   {
      text = "";
      title = "";
      wp = null;
   }

   /**
    * Gets the GUI component for this pane.
    */
   public JComponent getGUI()
   {
      if (wp == null)
      {
         wp = new WelcomePanePanel();
      }
      wp.setTextMessage(text);
      wp.setTitleText(title);
      return wp;
   }

   /**
    * Sets the welcome text.
    */
   public void setText(String text)
   {
      this.text = text;
   }

   /**
    * Gets the welcome text.
    */
   public String getText()
   {
      return text;
   }

   /**
    * Sets the title text.
    */
   public void setTitle(String title)
   {
      this.title = title;
   }

   /**
    * Gets the title text.
    */
   public String getTitle()
   {
      return title;
   }

   /**
    * The welcome text for this pane.
    */
   private String text;

   /**
    * The title of this pane.
    */
   private String title;

   /**
    * The JPanel that makes up this pane's UI.
    */
   private transient WelcomePanePanel wp;
}

/**
 * Defines the UI aspect of the WelcomePane.
 */
class WelcomePanePanel
   extends JPanel
{
   BorderLayout baseLayout = new BorderLayout();
   JLabel titleLbl = new JLabel();
   JPanel centerPnl = new JPanel();
   BorderLayout centerLayout = new BorderLayout();
   JTextArea msgTextArea = new JTextArea();

   public WelcomePanePanel()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   public void setTextMessage(String message)
   {
      msgTextArea.setText(message);
   }

   public String getTextMessage()
   {
      return msgTextArea.getText();
   }

   public void setTitleText(String title)
   {
      titleLbl.setText(title);
   }

   public String getTitleText()
   {
      return titleLbl.getText();
   }

   private void jbInit() throws Exception
   {
      baseLayout.setHgap(5);
      baseLayout.setVgap(5);
      titleLbl.setFont(new java.awt.Font("Serif", 1, 12));
      titleLbl.setHorizontalAlignment(SwingConstants.LEFT);
      titleLbl.setText("Welcome");
      this.setLayout(baseLayout);
      centerPnl.setLayout(centerLayout);
      msgTextArea.setBackground(UIManager.getColor("Label.background"));
      msgTextArea.setForeground(UIManager.getColor("Label.foreground"));
      msgTextArea.setEditable(false);
      msgTextArea.setLineWrap(true);
      msgTextArea.setWrapStyleWord(true);
      this.add(titleLbl,  BorderLayout.NORTH);
      this.add(centerPnl, BorderLayout.CENTER);
      centerPnl.add(msgTextArea, BorderLayout.CENTER);
   }
}
