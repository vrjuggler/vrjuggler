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
package org.vrjuggler.jccl.editors;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Color;
import java.util.List;
import javax.swing.*;
import javax.swing.border.TitledBorder;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;

import com.sun.java.swing.plaf.windows.WindowsTreeUI;

/**
 * Allows the user to hide/expand a panel that contains property editors.
 */
public class ExpandablePanel extends PropertyComponent
{
   private TableLayout tableLayout;

   public void finalize()
   {
      ActionListener[] listeners = mExpandButton.getActionListeners();
      
      for (int i = 0 ; i < listeners.length ; ++i)
      {
         mExpandButton.removeActionListener(listeners[i]);
      }
      super.finalize();
   }
   
   public ExpandablePanel(Color color)
   {
      mColor = color;

      // Set the color of this panel
      setColor(mColor);
      setBackground(mColor);

      // Create a frame
      setBounds (100, 100, 700, 300);

      // Create a TableLayout for the frame
      double border = 10;
      double f = TableLayout.FILL;
      double p = TableLayout.PREFERRED;

      // The icons are only 16 pixels tall, so we want to enforce the size.
      double size[][] =
         {{ p, p, f, 16, 16}, // Columns
          { 16, p}}; // Rows

      tableLayout = new TableLayout(size);
      //tableLayout.setVGap(10);
      //tableLayout.setHGap(20);

      // Set the layout to use.
      setLayout (tableLayout);

      //mExpandedIcon = (Icon)UIManager.get( "Tree.expandedIcon" );
      //mCollapsedIcon = (Icon)UIManager.get( "Tree.collapsedIcon" );
      mExpandedIcon = WindowsTreeUI.ExpandedIcon.createExpandedIcon();
      mCollapsedIcon = WindowsTreeUI.CollapsedIcon.createCollapsedIcon();

      mExpandButton.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               if(!mExpanded)
               {
                  TableLayoutConstraints c2 = new TableLayoutConstraints(1, 1, 4, 1, TableLayout.FULL, TableLayout.FULL);
                  add(mComponent, c2);

                  // We are now expanded.
                  mExpanded = true;
                  mExpandButton.setIcon(mExpandedIcon);
               }
               else
               {
                  remove(mComponent);

                  // We are now collapsed.
                  mExpanded = false;
                  mExpandButton.setIcon(mCollapsedIcon);
               }

               revalidate();
               repaint();
            }
         });
      
      // Add expand button.
      TableLayoutConstraints c = new TableLayoutConstraints(0, 0, 0, 0, TableLayout.LEFT, TableLayout.TOP);
      mExpandButton.setIcon(mCollapsedIcon);
      mExpandButton.setMargin(new Insets(0,0,0,0));
      mExpandButton.setBorderPainted(false);
      mExpandButton.setFocusPainted(false);
      mExpandButton.setContentAreaFilled(false);
      add(mExpandButton, c);
 
      // Add type icon button.
      TableLayoutConstraints ct = new TableLayoutConstraints(1, 0, 1, 0, TableLayout.LEFT, TableLayout.TOP);
      mTypeButton.setMargin(new Insets(0,0,0,0));
      mTypeButton.setBorderPainted(false);
      mTypeButton.setFocusPainted(false);
      mTypeButton.setContentAreaFilled(false);
      add(mTypeButton, ct);
   }
   
   protected   PropertyComponent mComponent     = null;
   private     Icon              mExpandedIcon  = null;
   private     Icon              mCollapsedIcon = null;   
   private     JButton           mExpandButton  = new JButton();   
   protected   JButton           mTypeButton    = new JButton();   
   private     boolean           mExpanded      = false;
}
