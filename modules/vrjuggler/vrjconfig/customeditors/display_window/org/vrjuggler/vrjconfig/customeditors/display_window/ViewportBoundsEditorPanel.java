/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.*;
import javax.swing.*;
import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.ConfigElement;


public class ViewportBoundsEditorPanel extends JPanel
{
   public ViewportBoundsEditorPanel()
   {
      this(null);
   }

   public ViewportBoundsEditorPanel(ConfigElement element)
   {
      double[][] bounds_size = {{TableLayout.PREFERRED, 10,
                                 TableLayout.PREFERRED, 3,
                                 TableLayout.PREFERRED, 5,
                                 TableLayout.PREFERRED, 3,
                                 TableLayout.PREFERRED},
                                {TableLayout.PREFERRED, TableLayout.PREFERRED}};
      mLayout = new TableLayout(bounds_size);

      try
      {
         jbInit();

         if ( element == null )
         {
            setSpinnerModel(mOriginXSpinner, 0, 0, 99);
            setSpinnerModel(mOriginYSpinner, 0, 0, 99);
            setSpinnerModel(mWidthSpinner, 100, 0, 100);
            setSpinnerModel(mHeightSpinner, 100, 0, 100);
         }
         else
         {
            double val;
            val = ((Number) element.getProperty("origin", 0)).doubleValue() * 100.0;
            setSpinnerModel(mOriginXSpinner, (int) val, 0, 99);
            val = ((Number) element.getProperty("origin", 1)).doubleValue() * 100.0;
            setSpinnerModel(mOriginYSpinner, (int) val, 0, 99);
            val = ((Number) element.getProperty("size", 0)).doubleValue() * 100.0;
            setSpinnerModel(mWidthSpinner, (int) val, 0, 100);
            val = ((Number) element.getProperty("size", 1)).doubleValue() * 100.0;
            setSpinnerModel(mHeightSpinner, (int) val, 0, 100);
         }

         // Set up a pairing between the origin spinners and the dimensions
         // spinners.  In this pairing, whenever the origin is changed, the
         // maximum allowable dimensions for the viewport will be changed.
         mOriginXSpinner.getModel().addChangeListener(new SpinnerChangeListener(((SpinnerNumberModel) mWidthSpinner.getModel())));
         mOriginYSpinner.getModel().addChangeListener(new SpinnerChangeListener(((SpinnerNumberModel) mHeightSpinner.getModel())));
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   private void setSpinnerModel(JSpinner spinner, int value, int min, int max)
   {
      SpinnerNumberModel model =
         new SpinnerNumberModel(new Integer(value), new Integer(min),
                                new Integer(max), new Integer(1));
      spinner.setModel(model);
   }

   public Rectangle getViewportBounds()
   {
      int x = ((Integer) mOriginXSpinner.getModel().getValue()).intValue();
      int y = ((Integer) mOriginYSpinner.getModel().getValue()).intValue();
      int width = ((Integer) mWidthSpinner.getModel().getValue()).intValue();
      int height = ((Integer) mHeightSpinner.getModel().getValue()).intValue();
      return new Rectangle(x, y, width, height);
   }

   private void jbInit() throws Exception
   {
      this.setToolTipText("Define the bounds of the viewport within the containing display window");
      this.setLayout(mLayout);
      mOriginLabel.setText("Origin (%)");
      mOriginXLabel.setLabelFor(mOriginXSpinner);
      mOriginXLabel.setText("X");
      mSizeLabel.setToolTipText("");
      mSizeLabel.setText("Size (%)");
      mOriginYLabel.setLabelFor(mOriginYSpinner);
      mOriginYLabel.setText("Y");
      mHeightLabel.setLabelFor(mHeightSpinner);
      mHeightLabel.setText("Height");
      mWidthLabel.setLabelFor(mWidthSpinner);
      mWidthLabel.setText("Width");
      this.add(mOriginLabel,
               new TableLayoutConstraints(0, 0, 0, 0,
                                          TableLayout.RIGHT,
                                          TableLayout.FULL));
      this.add(mOriginXLabel,
               new TableLayoutConstraints(2, 0, 2, 0,
                                          TableLayout.RIGHT,
                                          TableLayout.FULL));
      this.add(mOriginXSpinner,
               new TableLayoutConstraints(4, 0, 4, 0,
                                          TableLayout.FULL,
                                          TableLayout.FULL));
      this.add(mOriginYLabel,
               new TableLayoutConstraints(6, 0, 6, 0,
                                          TableLayout.RIGHT,
                                          TableLayout.FULL));
      this.add(mOriginYSpinner,
               new TableLayoutConstraints(8, 0, 8, 0,
                                          TableLayout.FULL,
                                          TableLayout.FULL));
      this.add(mSizeLabel,
               new TableLayoutConstraints(0, 1, 0, 1,
                                          TableLayout.RIGHT,
                                          TableLayout.FULL));
      this.add(mWidthLabel,
               new TableLayoutConstraints(2, 1, 2, 1,
                                          TableLayout.RIGHT,
                                          TableLayout.FULL));
      this.add(mWidthSpinner,
               new TableLayoutConstraints(4, 1, 4, 1,
                                          TableLayout.FULL,
                                          TableLayout.FULL));
      this.add(mHeightLabel,
               new TableLayoutConstraints(6, 1, 6, 1,
                                          TableLayout.RIGHT,
                                          TableLayout.FULL));
      this.add(mHeightSpinner,
               new TableLayoutConstraints(8, 1, 8, 1,
                                          TableLayout.FULL,
                                          TableLayout.FULL));
      mOriginXSpinner.setMinimumSize(new Dimension(50, 20));
      mOriginXSpinner.setPreferredSize(new Dimension(50, 20));
      mOriginYSpinner.setMinimumSize(new Dimension(50, 20));
      mOriginYSpinner.setPreferredSize(new Dimension(50, 20));
      mWidthSpinner.setMinimumSize(new Dimension(50, 20));
      mWidthSpinner.setPreferredSize(new Dimension(50, 20));
      mHeightSpinner.setMinimumSize(new Dimension(50, 20));
      mHeightSpinner.setPreferredSize(new Dimension(50, 20));
   }

   private TableLayout mLayout;
   private JLabel mOriginLabel = new JLabel();
   private JLabel mOriginXLabel = new JLabel();
   private JSpinner mOriginXSpinner = new JSpinner();
   private JLabel mSizeLabel = new JLabel();
   private JLabel mOriginYLabel = new JLabel();
   private JSpinner mOriginYSpinner = new JSpinner();
   private JSpinner mHeightSpinner = new JSpinner();
   private JLabel mHeightLabel = new JLabel();
   private JSpinner mWidthSpinner = new JSpinner();
   private JLabel mWidthLabel = new JLabel();

   private class SpinnerChangeListener
      implements javax.swing.event.ChangeListener
   {
      public SpinnerChangeListener(SpinnerNumberModel pairModel)
      {
         mPairModel = pairModel;
         mOrigMax   = ((Number) pairModel.getMaximum()).intValue();
      }

      /**
       * Updates the maximum possible value for our paired spinner model based
       * on our current value.
       *
       * @param e ChangeEvent
       */
      public void stateChanged(javax.swing.event.ChangeEvent e)
      {
         int value = ((SpinnerNumberModel) e.getSource()).getNumber().intValue();
         int new_max = mOrigMax - value;
         mPairModel.setMaximum(new Integer(new_max));

         if ( mPairModel.getNumber().intValue() > new_max )
         {
            mPairModel.setValue(new Integer(new_max));
         }
      }

      private SpinnerNumberModel mPairModel;
      private int                mOrigMax;
   }
}
