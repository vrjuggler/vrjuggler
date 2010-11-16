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

package org.vrjuggler.vrjconfig.customeditors.intersense;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import java.util.*;

import org.vrjuggler.jccl.config.*;

/**
 * Editor used to manipulate information about an Intersense Station.
 */
public class StationEditor extends JPanel
{
   /** The name of this Intersense Station. */
   private JTextField   mStationName    = null;

   /** The name of this Intersense Station. */
   private JLabel   mStationLabel    = null;

   /** Whether analog input is enabled for this station. */
   private JCheckBox mAnalogBox = new JCheckBox();

   /** Whether digital input is enabled for this station. */
   private JCheckBox mDigitalBox = new JCheckBox();

   /** Label used to display a picture of the device connected to this station. */
   private JLabel       mIconLabel      = null;

   /** A box to contain the checkboxes. */

   /** The model used to store the data about this station. */
   private StationModel mStationModel   = null;

   /** Panel that contains all information about this station. */
   private JPanel       mInfoPanel      = null;

   private ImageIcon    mHeadIcon       = null;
   private ImageIcon    mWandIcon       = null;

   private GridBagLayout gridBagLayout1 = new GridBagLayout();

   /**
    * Constructor
    */
   public StationEditor()
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

   /**
    * Initialize the GUI.
    */
   private void jbInit() throws Exception
   {
      // Create controls.
      mStationName = new JTextField();
      mStationLabel = new JLabel("Station Name:");
      mIconLabel = new JLabel();
      mInfoPanel = new JPanel();

      // Set control properties.
      mAnalogBox = new JCheckBox();
      mDigitalBox = new JCheckBox();
      mStationName.setBorder(BorderFactory.createLineBorder(Color.black));
      mStationName.setText("Station Name");
      
      // Load the wand and head images.
      ClassLoader loader = getClass().getClassLoader();
      //mWandIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/wired_wand.jpg"));
      mWandIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/wireless_wand.jpg"));
      mHeadIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/head.jpg"));
      
      mIconLabel.setIcon(mWandIcon);

      // Add Controls to Panel.
      this.setLayout(new BorderLayout());
      mInfoPanel.setLayout(gridBagLayout1);
      mAnalogBox.setText("Enable Analog");
      mAnalogBox.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  mStationModel.setAnalogEnabled(mAnalogBox.isSelected());
                  if(mAnalogBox.isSelected() || mDigitalBox.isSelected())
                  {
                     mIconLabel.setIcon(mWandIcon);
                  }
                  else
                  {
                     mIconLabel.setIcon(mHeadIcon);
                  }
               }
            });
      mDigitalBox.setText("Enable Digital");
      mDigitalBox.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  mStationModel.setDigitalEnabled(mDigitalBox.isSelected());
                  if(mAnalogBox.isSelected() || mDigitalBox.isSelected())
                  {
                     mIconLabel.setIcon(mWandIcon);
                  }
                  else
                  {
                     mIconLabel.setIcon(mHeadIcon);
                  }
               }
            });

      this.add(mIconLabel, BorderLayout.WEST);
      this.add(mInfoPanel, BorderLayout.CENTER);
      mInfoPanel.add(mStationLabel,     new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
               ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 5, 0, 5), 0, 0));
      mInfoPanel.add(mStationName,  new GridBagConstraints(0, 1, 2, 1, 0.0, 0.0
               ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 5, 0, 5), 0, 0));

      mInfoPanel.add(mDigitalBox,  new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
               ,GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
      mInfoPanel.add(mAnalogBox,  new GridBagConstraints(0, 3, 1, 1, 0.0, 0.0
               ,GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));

      // Add listeners to catch changes.


      mStationName.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               mStationModel.setName(mStationName.getText()) ;
            }
         });

      mStationName.addFocusListener(new FocusAdapter()
         {
            public void focusLost(FocusEvent evt)
            {
               mStationModel.setName(mStationName.getText()) ;
            }
         });


   }

   /**
    * Set the model for the station to be edited.
    */
   public void setModel(StationModel model)
   {
      mStationModel = model;
      mStationName.setText(mStationModel.getName());
      mDigitalBox.setSelected(mStationModel.isDigitalEnabled());
      mAnalogBox.setSelected(mStationModel.isAnalogEnabled());
      if(mAnalogBox.isSelected() || mDigitalBox.isSelected())
      {
         mIconLabel.setIcon(mWandIcon);
      }
      else
      {
         mIconLabel.setIcon(mHeadIcon);
      }
   }

   /**
    * Set the background of all controls and this panel.
    */
   public void setBackground(java.awt.Color clr)
   {
      // Note: Have to check for null because for some reason a superclass
      //       constructor calls this method before our constructor.
      super.setBackground(clr);
      if(null != mAnalogBox &&
         null != mDigitalBox &&
         null != mInfoPanel &&
         null != mStationLabel)
      {
         mAnalogBox.setBackground(clr);
         mDigitalBox.setBackground(clr);
         mInfoPanel.setBackground(clr);
         mStationLabel.setBackground(clr);
      }
   }

   /**
    * Set the foreground of all controls and this panel.
    */
   public void setForeground(java.awt.Color clr)
   {
      // Note: Have to check for null because for some reason a superclass
      //       constructor calls this method before our constructor.
      super.setForeground(clr);
      if(null != mAnalogBox &&
         null != mDigitalBox &&
         null != mInfoPanel &&
         null != mStationLabel)
      {
         mAnalogBox.setForeground(clr);
         mDigitalBox.setForeground(clr);
         mInfoPanel.setForeground(clr);
         mInfoPanel.setForeground(clr);
         mStationLabel.setForeground(clr);
      }
   }

   /**
    * Get the value of this cell editor.
    */
   public Object getCellEditorValue()
   {
      return mStationModel;
   }

    /**
     * Asks the editor if it can start editing using <code>anEvent</code>.
     * <code>anEvent</code> is in the invoking component coordinate system.
     * The editor can not assume the Component returned by
     * <code>getCellEditorComponent</code> is installed.  This method
     * is intended for the use of client to avoid the cost of setting up
     * and installing the editor component if editing is not possible.
     * If editing can be started this method returns true.
     *
     * @param	anEvent		the event the editor should use to consider
     *				whether to begin editing or not
     * @return	true if editing can be started
     * @see #shouldSelectCell
     */
    public boolean isCellEditable(EventObject anEvent)
    { return true; }

    /**
     * Returns true if the editing cell should be selected, false otherwise.
     * Typically, the return value is true, because is most cases the editing
     * cell should be selected.  However, it is useful to return false to
     * keep the selection from changing for some types of edits.
     * eg. A table that contains a column of check boxes, the user might
     * want to be able to change those checkboxes without altering the
     * selection.  (See Netscape Communicator for just such an example)
     * Of course, it is up to the client of the editor to use the return
     * value, but it doesn't need to if it doesn't want to.
     *
     * @param	anEvent		the event the editor should use to start
     *				editing
     * @return	true if the editor would like the editing cell to be selected;
     *    otherwise returns false
     * @see #isCellEditable
     */
    public boolean shouldSelectCell(EventObject anEvent)
    { return true; }

   /**
    * Tells the editor to stop editing and accept any partially edited
    * value as the value of the editor.  The editor returns false if
    * editing was not stopped; this is useful for editors that validate
    * and can not accept invalid entries.
    *
    * @return	true if editing was stopped; false otherwise
    */
   public boolean stopCellEditing()
   {
      if(mAnalogBox.isSelected() != mStationModel.isAnalogEnabled())
      {
         mStationModel.setAnalogEnabled(mAnalogBox.isSelected());
      }
      if(mDigitalBox.isSelected() != mStationModel.isDigitalEnabled())
      {
         mStationModel.setDigitalEnabled(mDigitalBox.isSelected());
      }
      if(!mStationName.getText().equals(mStationModel.getName()))
      {
         mStationModel.setName(mStationName.getText()) ;
      }
      return true;
    }

    /**
     * Tells the editor to cancel editing and not accept any partially
     * edited value.
     */
    public void cancelCellEditing()
    {;}

    /**
     * Adds a listener to the list that's notified when the editor
     * stops, or cancels editing.
     *
     * @param	l		the CellEditorListener
     */
    public void addCellEditorListener(CellEditorListener l)
    {;}

    /**
     * Removes a listener from the list that's notified
     *
     * @param	l		the CellEditorListener
     */
    public void removeCellEditorListener(CellEditorListener l)
    {;}
  void mAnalogBox_actionPerformed(ActionEvent e) {

  }
  void mDigitalBox_actionPerformed(ActionEvent e) {

  }
}
