/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.vrjconfig.ui;

import java.awt.*;
import javax.swing.*;

import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.vrjconfig.ui.DisplayPlacer;

/**
 * A custom editor for the displays and the surfaces contained therein.
 */
public class DisplayEditor
   extends JPanel
{
   /**
    * Creates a new DisplayEditor acting on the currently active configuration.
    */
   public DisplayEditor()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Init the icons
      ClassLoader loader = BeanJarClassLoader.instance();
      displayEditorBasicBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/ui/images/displayeditor_basic32.png")));
      displayEditor3DBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/ui/images/displayeditor_3d32.png")));
   }

   /**
    * Automatically generated JBuilder GUI init.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(baseLayout);
      toolbar.setBackground(Color.black);
      toolbar.setFloatable(false);
      titleLbl.setFont(new java.awt.Font("SansSerif", 3, 16));
      titleLbl.setForeground(Color.white);
      titleLbl.setText("Display Editors");
      displayEditorBasicBtn.setEnabled(false);
      displayEditorBasicBtn.setBorderPainted(false);
      displayEditorBasicBtn.setContentAreaFilled(false);
      displayEditor3DBtn.setBorderPainted(false);
      displayEditor3DBtn.setContentAreaFilled(false);
      mainPanel.setLayout(mainLayout);
      this.add(toolbar, BorderLayout.NORTH);
      toolbar.add(titleLbl, null);
      toolbar.add(Box.createHorizontalGlue(), null);
      toolbar.add(displayEditorBasicBtn, null);
      toolbar.add(Box.createHorizontalStrut(8));
      toolbar.add(displayEditor3DBtn, null);
      toolbar.add(Box.createHorizontalStrut(8), null);
      this.add(mainPanel, BorderLayout.CENTER);
      mainPanel.add(displayPlacer, BorderLayout.CENTER);
   }

   //--- JBuilder GUI variables ---//
   private BorderLayout baseLayout = new BorderLayout();
   private JToolBar toolbar = new JToolBar();
   private JLabel titleLbl = new JLabel();
   private JButton displayEditorBasicBtn = new JButton();
   private JButton displayEditor3DBtn = new JButton();
   private BorderLayout mainLayout = new BorderLayout();
   private JPanel mainPanel = new JPanel();
   private DisplayPlacer displayPlacer = new DisplayPlacer();
}
