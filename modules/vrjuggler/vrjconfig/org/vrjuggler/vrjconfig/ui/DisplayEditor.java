/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.ui;

import java.awt.*;
import javax.swing.*;

import org.vrjuggler.jccl.config.ConfigChunk;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.editors.ConfigChunkPropertySheet;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.vrjconfig.ui.DisplayPlacer;
import org.vrjuggler.vrjconfig.ui.placer.*;

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
      displayPlacer.setDesktopSize(new Dimension(1600,1200));

      // Init the icons
      ClassLoader loader = BeanJarClassLoader.instance();
      displayEditorBasicBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/ui/images/displayeditor_basic32.png")));
      displayEditor3DBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/ui/images/displayeditor_3d32.png")));

      // Connect the selections in the display placer to the property sheet
      displayPlacer.getPlacer().addPlacerSelectionListener(new PlacerSelectionListener()
      {
         public void valueChanged(PlacerSelectionEvent evt)
         {
            displaySelectionChanged();
         }
      });

//      mainScrollPane.resetToPreferredSizes();
   }

   public void setConfigContext(ConfigContext context)
   {
      displayPlacer.setConfigContext(context);
   }

   public ConfigContext getConfigContext()
   {
      return displayPlacer.getConfigContext();
   }

   /**
    * Called by our listener on the display placer whenever the selection on
    * the display placer has changed.
    */
   protected void displaySelectionChanged()
   {
      ConfigChunk display_chunk = displayPlacer.getSelectedDisplay();
      propSheet.setConfigChunk(display_chunk);
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
//      mainPanel.setLayout(mainLayout);
      mainSplitPane.setOrientation(JSplitPane.VERTICAL_SPLIT);
      mainSplitPane.setResizeWeight(1.0);
      this.add(toolbar, BorderLayout.NORTH);
      toolbar.add(titleLbl, null);
      toolbar.add(Box.createHorizontalGlue(), null);
      toolbar.add(displayEditorBasicBtn, null);
      toolbar.add(Box.createHorizontalStrut(8));
      toolbar.add(displayEditor3DBtn, null);
      toolbar.add(Box.createHorizontalStrut(8), null);
      this.add(mainSplitPane, BorderLayout.CENTER);
      mainSplitPane.add(displayPlacer, JSplitPane.TOP);
      mainSplitPane.add(propSheetScrollPane, JSplitPane.BOTTOM);
//      this.add(mainPanel, BorderLayout.CENTER);
//      mainPanel.add(displayPlacer, BorderLayout.CENTER);
//      mainPanel.add(propSheetScrollPane, BorderLayout.SOUTH);
      propSheetScrollPane.setViewportView(propSheet);
   }

   //--- JBuilder GUI variables ---//
   private BorderLayout baseLayout = new BorderLayout();
   private JToolBar toolbar = new JToolBar();
   private JLabel titleLbl = new JLabel();
   private JButton displayEditorBasicBtn = new JButton();
   private JButton displayEditor3DBtn = new JButton();
//   private BorderLayout mainLayout = new BorderLayout();
//   private JPanel mainPanel = new JPanel();
   private JSplitPane mainSplitPane = new JSplitPane();
   private DisplayPlacer displayPlacer = new DisplayPlacer();
   private JScrollPane propSheetScrollPane = new JScrollPane();
   private ConfigChunkPropertySheet propSheet = new ConfigChunkPropertySheet();
}
