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
package org.vrjuggler.jccl.perfmonitor;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class PerfTreeNodeInfo implements ActionListener
{

   protected class LabeledPanelButton extends JButton
   {
      public LabeledPerfDataCollector collector;
      public LabeledPerfDataCollector.IndexInfo index_info;

      public LabeledPanelButton (LabeledPerfDataCollector _collector, 
                                 LabeledPerfDataCollector.IndexInfo _ii, 
                                 String text)
      {
         super (text);
         collector = _collector;
         index_info = _ii;
      }
   }

   public String sublabel;
   public LabeledPerfDataCollector.IndexInfo ii; // null for folders
   protected double mSumAverages; // sum of averages of children, if ii = null
   protected LabeledPerfDataCollector mCollector;
   protected JComponent mComponent;
   protected JLabel mLabel;
   protected JLabel mValueLabel;
   protected JButton mGraphButton;
   protected Component mPad;
   
   
   /* if ii is null, it's for a collector... */
   public PerfTreeNodeInfo (String _sublabel, LabeledPerfDataCollector.IndexInfo _ii, LabeledPerfDataCollector col)
   {
      sublabel = _sublabel;
      ii = _ii;
      mSumAverages = 0.0;
      mCollector = col;

      mComponent = new JPanel();
      mComponent.setLayout (new BoxLayout (mComponent, BoxLayout.X_AXIS));
      mComponent.setBackground (Color.white);

      mPad = Box.createHorizontalStrut(150);
        
        
      if (ii != null)
      {
//  	    JLabel l = new JLabel(_sublabel);
//  	    mComponent.add (l);
//              mComponent.add (Box.createHorizontalGlue());
//  	    mValueLabel = new JLabel (padFloat(getAverage()/1000.0) + " ms", JLabel.RIGHT);
//  	    mComponent.add (mValueLabel);
//  	    mGraphButton = new LabeledPanelButton (col, ii, "Graph");
//  	    mGraphButton.setActionCommand ("Graph");
//  	    mGraphButton.addActionListener (this);
//  	    //b.addActionListener (PerfAnalyzerPanel.this);
//  	    Insets insets = new Insets (1,1,1,1);
//  	    mGraphButton.setMargin(insets);
//  	    //mComponent.add (mGraphButton);


         mLabel = new JLabel(_sublabel + "            ");
         mValueLabel = new JLabel (padFloat(getAverage()/1000.0) + " ms", JLabel.RIGHT);
         GridBagLayout gbl = new GridBagLayout();
         GridBagConstraints gbc = new GridBagConstraints();
            
         gbc.gridwidth = gbc.RELATIVE;
         gbl.setConstraints (mLabel, gbc);
         mComponent.add(mLabel);

         mComponent.add (mPad);
            
            
         gbc.gridwidth = gbc.REMAINDER;
         gbc.anchor = gbc.EAST;
         gbc.weightx = 1;
         gbc.fill = gbc.BOTH;
         gbl.setConstraints (mValueLabel, gbc);
         mComponent.add (mValueLabel);
            
            
         mGraphButton = new LabeledPanelButton (col, ii, "Graph");
      }
      else
      {
         mLabel = new JLabel ("<html><i>" + sublabel + "</i></html>");
         mComponent.add(mLabel);
         //mComponent.add (Box.createHorizontalGlue());
         mComponent.add (mPad);

         mValueLabel = new JLabel (padFloat(getAverage()/1000.0) + " ms", JLabel.RIGHT);
         mComponent.add (mValueLabel);
         mGraphButton = new LabeledPanelButton (col, null, "Graph");
         mGraphButton.setActionCommand ("Graph");
         mGraphButton.addActionListener (this);
         //b.addActionListener (PerfAnalyzerPanel.this);
         Insets insets = new Insets (1,1,1,1);
         mGraphButton.setMargin(insets);
         //mComponent.add (mGraphButton);
      }
      if (mValueLabel != null) 
      {
         mPad.setSize ((int)(500 - mLabel.getSize().getWidth() - mValueLabel.getSize().getWidth()), 1);
      }
        
   }

   
   public String toString ()
   {
      return sublabel;
   }

   
   public JComponent getComponent()
   {
      return mComponent;
   }
   

   public LabeledPerfDataCollector.IndexInfo getIndexInfo()
   {
      return ii;
   }

   
   public LabeledPerfDataCollector getCollector()
   {
      return mCollector;
   }

   
   public double getAverage ()
   {
      if (ii != null) 
      {
         return ii.getAverage();
      }
      else
      {
         return mSumAverages;
      }
   }

   
   public void setAverage (double avg)
   {
      mSumAverages = avg;
   }


   public JButton getGraphButton ()
   {
      return mGraphButton;
   }

   
   public void update()
   {
      if (mValueLabel != null) 
      {
         mValueLabel.setText (padFloat(getAverage()/1000.0) + " ms");
         mPad.setSize ((int)(500 - mLabel.getSize().getWidth() - mValueLabel.getSize().getWidth()), 1);
      }
      
   }

   
   /** Utility method for various printing routines. */
   private String padFloat (double f)
   {
      // reformats f to a string w/ 3 places after decimal
      String s = Double.toString(f);
      if (s.indexOf('E') != -1)
         return s;
      int i = s.lastIndexOf('.');
      if ((i >= 0) && (i+5 < s.length()))
         s = s.substring (0, i + 5);
      return s;
   }

   
   public void actionPerformed (ActionEvent e)
   {
      System.out.println ("actionevent...");
   }

}
