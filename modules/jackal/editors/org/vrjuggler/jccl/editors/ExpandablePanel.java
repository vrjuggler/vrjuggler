package org.vrjuggler.jccl.editors;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Color;
//import java.awt.Graphics;
//import java.util.ArrayList;
//import java.util.Iterator;
import java.util.List;
//import java.util.HashMap;
//import java.util.Map;
//import java.util.Set;
import javax.swing.*;
import javax.swing.border.TitledBorder;

//import new_layout.*;
import info.clearthought.layout.*;
//TODO: Remove these import
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;
//import com.sun.java.swing.plaf.windows.WindowsTreeUI;

import com.sun.java.swing.plaf.windows.WindowsTreeUI;

public class ExpandablePanel extends PropertyComponent
{       
   private TableLayout tableLayout;

   public ExpandablePanel(PropertyComponent cmp, String token, String title, Color color,
         boolean variable, boolean embedded)
   {
      mColor = color;
      //setBorder(BorderFactory.createLineBorder(Color.black));
      if(embedded)
      {
         ClassLoader loader = getClass().getClassLoader();
         mTypeIcon = new ImageIcon(loader.getResource("images/juggler-jigsaw16.gif"));
         mTypeButton.setIcon(mTypeIcon);
         
         setBorder(BorderFactory.createEtchedBorder());

         //setBorder(BorderFactory.createCompoundBorder(BorderFactory.createRaisedBevelBorder(),
         //         BorderFactory.createLoweredBevelBorder()));

         //TitledBorder tb = BorderFactory.createTitledBorder(BorderFactory.createEtchedBorder(), token);
         //tb.setTitleColor(new Color(80, 80, 80));
         //setBorder(tb);
      }
      else
      {
         //setBorder(BorderFactory.createLoweredBevelBorder());
         setBorder(BorderFactory.createEtchedBorder());
      }

        // Create a frame
      setBounds (100, 100, 700, 300);

      setBackground(mColor);

      //System.out.println("XXXX Setting color for: " + title);
      //System.out.println("XXXX R: " + mColor.getRed() + " G: " + mColor.getGreen() + " B: " + mColor.getBlue());

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

      // Create controls and add them.
      mComponent = cmp;

      ClassLoader loader = getClass().getClassLoader();
      mTypeIcon = new ImageIcon(loader.getResource("images/juggler-jigsaw16.gif"));
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
                  TableLayoutConstraints c2 = new TableLayoutConstraints(1, 1, 4, 1, TableLayout.LEFT, TableLayout.FULL);
                  add(mComponent, c2);
                  
                  // Refresh us.
                  refresh();
                  
                  // Refresh our child.
                  mComponent.doLayout();
                  mComponent.repaint();
                  
                  // We are now expanded.
                  mExpanded = true;
                  mExpandButton.setIcon(mExpandedIcon);
               }
               else
               {
                  remove(mComponent);
                  
                  // Refresh ourselves.
                  refresh();
                  
                  // We are now collapsed.
                  mExpanded = false;
                  mExpandButton.setIcon(mCollapsedIcon);
               }

               // We must update the height of our row.
               updateMyRow();
               refresh();
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
     
      // Add title for the list.
      JLabel name = new JLabel(title);
      TableLayoutConstraints c2 = new TableLayoutConstraints(2, 0, 2, 0, TableLayout.LEFT, TableLayout.TOP);
      add(name, c2);
   }
   
   public void updateMyRow()
   {
      if(getParent() != null && getParent() instanceof PropertyComponent)
      {
         PropertyComponent temp = (PropertyComponent)getParent();
         
         if(temp.getParent() != null && temp.getParent() instanceof ExpandablePanel)
         {
            ExpandablePanel temp_list = (ExpandablePanel)temp.getParent();
            temp_list.updateMyRow();
            System.out.println("Updating recursively.");
         }
         if(temp.getLayout() instanceof TableLayout)
         {
            TableLayout tl = (TableLayout)temp.getLayout();
            // Get the row that this panel is in.
            TableLayoutConstraints tlc = tl.getConstraints(this);
            int row = tlc.row1;
         
            Dimension pref_size = this.getPreferredSize();
            int height = (int)pref_size.getHeight();
         
            tl.setRow(row, height);
            temp.doLayout();
            temp.repaint();
            //refresh();
            //System.out.println("Adjusting row: " + row + " to " + height);
         
            //temp.doLayout();
            //temp.repaint();
         }
         temp.invalidate();
         temp.validate();
      }
   }

   public void refresh()
   {
      updateMyRow();
      super.refresh();
      updateMyRow();
   }

   private PropertyComponent  mComponent     = null;
   private Icon               mExpandedIcon  = null;
   private Icon               mTypeIcon      = null;
   private Icon               mCollapsedIcon = null;   
   private JButton            mExpandButton  = new JButton();   
   private JButton            mTypeButton    = new JButton();   
   private boolean            mExpanded      = false;
}
