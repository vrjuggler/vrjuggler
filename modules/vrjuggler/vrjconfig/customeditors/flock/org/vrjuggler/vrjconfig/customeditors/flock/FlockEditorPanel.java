/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

package org.vrjuggler.vrjconfig.customeditors.flock;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.vrjconfig.commoneditors.SerialPortChooser;
import org.vrjuggler.vrjconfig.commoneditors.TransmitterTransformPanel;

public class FlockEditorPanel
   extends JPanel
{
   public FlockEditorPanel()
   {
/*
      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
*/

      mBaudEditor.addItem(new Integer(4800));
      mBaudEditor.addItem(new Integer(9600));
      mBaudEditor.addItem(new Integer(19200));
      mBaudEditor.addItem(new Integer(38400));
      mBaudEditor.addItem(new Integer(57600));
      mBaudEditor.addItem(new Integer(76800));
      mBaudEditor.addItem(new Integer(115200));
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      mContext = ctx;
      mElement = elt;

      mPortEditor.setPort((String) elt.getProperty("port", 0));
      mBaudEditor.setSelectedItem(elt.getProperty("baud", 0));

      ConfigDefinition flock_def = elt.getDefinition();
      mHemisphereEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("hemisphere", 0),
                                 flock_def.getPropertyDefinition("hemisphere"),
                                 elt, 0, Color.white);
      mMasterAddrEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("master_address", 0),
                                 flock_def.getPropertyDefinition("master_address"),
                                 elt, 0, Color.white);
      mAddrModeEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("addressing_mode", 0),
                                 flock_def.getPropertyDefinition("addressing_mode"),
                                 elt, 0, Color.white);
      mFilterEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("filter", 0),
                                 flock_def.getPropertyDefinition("filter"),
                                 elt, 0, Color.white);

      java.util.List filters = elt.getPropertyValues("position_filters");

      if ( filters == null || filters.size() == 0 )
      {
         // Pop up a warning message saying that the config element is
         // missing information.
         Frame parent =
            (Frame) SwingUtilities.getAncestorOfClass(Frame.class, this);
         JOptionPane.showMessageDialog(parent,
                                       "WARNING: Config element '" +
                                          elt.getName() +
                                          "' has no position filters!\n" +
                                          "Will create default position " +
                                          "transform filter ...",
                                       "Correcting Incomplete Configuration Element",
                                       JOptionPane.WARNING_MESSAGE);

         // Create the new position_transform_filter config element.
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition filter_def =
            broker.getRepository().get("position_transform_filter");
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());
         ConfigElement xform_filter = factory.create("Transform Filter 0",
                                                     filter_def);

         // Add the new config element to the position_filters property.
         elt.addProperty("position_filters", xform_filter);

         // Go ahead and set up the position transform filter editor panel
         // using the newly created config element.
         mPosXformFilterPanel.setConfig(ctx, xform_filter);
      }
      else
      {
         // Pass off the embedded position_transform_filter config element.
         // For now, we assume that there will be at most one such element.
         for ( java.util.Iterator i = filters.iterator(); i.hasNext(); )
         {
            ConfigElement filter = (ConfigElement) i.next();

            if ( filter.getDefinition().getToken().equals("position_transform_filter") )
            {
               mPosXformFilterPanel.setConfig(ctx, filter);
               break;
            }
         }
      }

      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
   }

   private void jbInit()
      throws Exception
   {
      this.setLayout(mMainLayout);
      mHardwarePanel.setBorder(mHardwarePanelTitle);
      mHardwarePanel.setLayout(nHardwarePanelLayout);
      mHardwarePanelTitle.setTitle("Hardware Settings");
      mPortLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mPortLabel.setLabelFor(mPortEditor);
      mPortLabel.setText("Serial Port:");
      mBaudLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mBaudLabel.setLabelFor(mBaudEditor);
      mBaudLabel.setText("Baud:");
      mHemisphereLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mHemisphereLabel.setLabelFor(mHemisphereEditor);
      mHemisphereLabel.setText("Hemisphere:");
      mMasterAddrLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mMasterAddrLabel.setLabelFor(mMasterAddrEditor);
      mMasterAddrLabel.setText("Master Address:");
      mAddrModeLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mAddrModeLabel.setText("Addressing Mode:");
      mPortEditor.addActionListener(new
                                    FlockEditorPanel_mPortEditor_actionAdapter(this));
      mFilterLabel.setLabelFor(mFilterEditor);
      mFilterLabel.setText("Filter:");
      mBaudEditor.addActionListener(new
                                    FlockEditorPanel_mBaudEditor_actionAdapter(this));
      mHardwarePanel.add(mPortLabel,
                         new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                                GridBagConstraints.EAST,
                                                GridBagConstraints.NONE,
                                                new Insets(0, 3, 0, 3), 0, 0));
      mHardwarePanel.add(mPortEditor,
                         new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0,
                                                GridBagConstraints.WEST,
                                                GridBagConstraints.HORIZONTAL,
                                                new Insets(0, 0, 0, 0), 0, 0));
      mHardwarePanel.add(mBaudLabel,
                         new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0,
                                                GridBagConstraints.EAST,
                                                GridBagConstraints.NONE,
                                                new Insets(0, 3, 0, 3), 0, 0));
      mHardwarePanel.add(mBaudEditor,
                         new GridBagConstraints(1, 1, 1, 1, 1.0, 1.0,
                                                GridBagConstraints.WEST,
                                                GridBagConstraints.BOTH,
                                                new Insets(0, 0, 0, 0), 0, 0));
      mHardwarePanel.add(mHemisphereEditor,
                         new GridBagConstraints(1, 2, 1, 1, 1.0, 1.0,
                                                GridBagConstraints.WEST,
                                                GridBagConstraints.BOTH,
                                                new Insets(0, 3, 0, 0), 0, 0));
      mHardwarePanel.add(mHemisphereLabel,
                         new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0,
                                                GridBagConstraints.EAST,
                                                GridBagConstraints.NONE,
                                                new Insets(0, 0, 0, 3), 0, 0));
      mHardwarePanel.add(mMasterAddrLabel,
                         new GridBagConstraints(0, 3, 1, 1, 0.0, 0.0,
                                                GridBagConstraints.EAST,
                                                GridBagConstraints.NONE,
                                                new Insets(0, 3, 0, 3), 0, 0));
      mHardwarePanel.add(mMasterAddrEditor,
                         new GridBagConstraints(1, 3, 1, 1, 1.0, 1.0,
                                                GridBagConstraints.WEST,
                                                GridBagConstraints.BOTH,
                                                new Insets(0, 0, 0, 0), 0, 0));
      mHardwarePanel.add(mAddrModeLabel,
                         new GridBagConstraints(0, 4, 1, 1, 0.0, 0.0,
                                                GridBagConstraints.EAST,
                                                GridBagConstraints.NONE,
                                                new Insets(0, 3, 0, 3), 0, 0));
      mHardwarePanel.add(mAddrModeEditor,
                         new GridBagConstraints(1, 4, 1, 1, 1.0, 1.0,
                                                GridBagConstraints.WEST,
                                                GridBagConstraints.BOTH,
                                                new Insets(0, 0, 0, 0), 0, 0));
      mHardwarePanel.add(mFilterLabel,
                         new GridBagConstraints(0, 5, 1, 1, 0.0, 0.0,
                                                GridBagConstraints.EAST,
                                                GridBagConstraints.NONE,
                                                new Insets(0, 3, 0, 3), 0, 0));
      mHardwarePanel.add(mFilterEditor,
                         new GridBagConstraints(1, 5, 1, 1, 1.0, 1.0,
                                                GridBagConstraints.WEST,
                                                GridBagConstraints.BOTH,
                                                new Insets(0, 0, 0, 0), 0, 0));
      this.add(mHardwarePanel,
               new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.WEST,
                                      GridBagConstraints.BOTH,
                                      new Insets(0, 0, 2, 0), 20, 0));
      this.add(mPosXformFilterPanel,
               new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.WEST,
                                      GridBagConstraints.BOTH,
                                      new Insets(0, 0, 2, 0), 20, 0));
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;

   private TransmitterTransformPanel mPosXformFilterPanel =
      new TransmitterTransformPanel();
   private JPanel mHardwarePanel = new JPanel();
   private TitledBorder mHardwarePanelTitle = new TitledBorder("");
   private JLabel mPortLabel = new JLabel();
   private SerialPortChooser mPortEditor = new SerialPortChooser();
   private JLabel mBaudLabel = new JLabel();
   private JComboBox mBaudEditor = new JComboBox();
   private JLabel mHemisphereLabel = new JLabel();
   private PropertyEditorPanel mHemisphereEditor = null;
   private JLabel mMasterAddrLabel = new JLabel();
   private PropertyEditorPanel mMasterAddrEditor = null;
   private JLabel mAddrModeLabel = new JLabel();
   private PropertyEditorPanel mAddrModeEditor = null;
   private JLabel mFilterLabel = new JLabel();
   private PropertyEditorPanel mFilterEditor = null;
   private GridBagLayout nHardwarePanelLayout = new GridBagLayout();
   private GridBagLayout mMainLayout = new GridBagLayout();

   void mPortEditor_actionPerformed(ActionEvent actionEvent)
   {
      mElement.setProperty("port", 0, mPortEditor.getPort());
   }

   void mBaudEditor_actionPerformed(ActionEvent actionEvent)
   {
      mElement.setProperty("baud", 0, mBaudEditor.getSelectedItem());
   }
}

class FlockEditorPanel_mBaudEditor_actionAdapter
   implements ActionListener
{
   private FlockEditorPanel adaptee;
   FlockEditorPanel_mBaudEditor_actionAdapter(FlockEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mBaudEditor_actionPerformed(actionEvent);
   }
}

class FlockEditorPanel_mPortEditor_actionAdapter
   implements ActionListener
{
   private FlockEditorPanel adaptee;
   FlockEditorPanel_mPortEditor_actionAdapter(FlockEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mPortEditor_actionPerformed(actionEvent);
   }
}
