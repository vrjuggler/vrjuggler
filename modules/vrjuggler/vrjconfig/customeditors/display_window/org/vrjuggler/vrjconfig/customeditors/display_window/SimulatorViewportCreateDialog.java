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

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.*;
import javax.swing.*;
import javax.swing.border.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;


public class SimulatorViewportCreateDialog
   extends ViewportCreateDialog
{
   public SimulatorViewportCreateDialog()
   {
      this(null);
   }

   public SimulatorViewportCreateDialog(ConfigElement viewportElt)
   {
      super("Basic Simulator Viewport Parameters", viewportElt);

      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition vp_def = broker.getRepository().get("default_simulator");

      ConfigElement sim_elt;

      if ( viewportElt == null )
      {
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());
         sim_elt = factory.create("Junk", vp_def);
      }
      else
      {
         sim_elt =
            (ConfigElement) viewportElt.getProperty("simulator_plugin", 0);
      }

      sim_elt.addConfigElementListener(this);

      mCameraPosEditor =
         new PropertyEditorPanel(sim_elt.getProperty("camera_pos", 0),
                                 vp_def.getPropertyDefinition("camera_pos"),
                                 sim_elt, 0, Color.white);
      mWandPosEditor =
         new PropertyEditorPanel(sim_elt.getProperty("wand_pos", 0),
                                 vp_def.getPropertyDefinition("wand_pos"),
                                 sim_elt, 0, Color.white);

      initUI();
      this.pack();
   }

   public void propertyValueChanged(ConfigElementEvent e)
   {
      super.propertyValueChanged(e);

      if ( e.getProperty().equals("camera_pos") )
      {
         mCameraPos = e.getValue();
         validateUserInput();
      }
      else if ( e.getProperty().equals("wand_pos") )
      {
         mWandPos = e.getValue();
         validateUserInput();
      }
   }

   public Double getVertialFOV()
   {
      return (Double) mVertFOVField.getValue();
   }

   public Object getCameraPosition()
   {
      return mCameraPos;
   }

   public Object getWandPosition()
   {
      return mWandPos;
   }

   protected boolean validateCustomInput()
   {
      boolean valid_fov, camera_set, wand_set;

      valid_fov  = ((Double) mVertFOVField.getValue()).doubleValue() > 0.0;
      camera_set = mCameraPos != null;
      wand_set   = mWandPos != null;

      return valid_fov && camera_set && wand_set;
   }

   protected void initUI()
   {
      super.initUI();

      double main_size[][] = {{TableLayout.PREFERRED},
                              {TableLayout.PREFERRED, TableLayout.PREFERRED,
                               TableLayout.PREFERRED}};
      mMainPanelLayout = new TableLayout(main_size);

      double fov_size[][] = {{TableLayout.PREFERRED, 10, TableLayout.PREFERRED},
                             {TableLayout.PREFERRED, TableLayout.PREFERRED,
                              TableLayout.PREFERRED}};
      mMiscPanelLayout = new TableLayout(fov_size);

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   private void jbInit() throws Exception
   {
      mBoundsPanelBorder = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"Viewport Bounds");
      mUserPanelBorder = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"User Settings");
      mMiscPanelBorder = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"Simulator Scene Settings");
      mBoundsPanel.setBorder(mBoundsPanelBorder);
      mMainPanel.setLayout(mMainPanelLayout);
      mUserPanel.setBorder(mUserPanelBorder);
      mMiscPanel.setBorder(mMiscPanelBorder);
      mMiscPanel.setLayout(mMiscPanelLayout);
      mVertFOVLabel.setLabelFor(mVertFOVField);
      mVertFOVLabel.setText("Vertical Field of View");
      mVertFOVField.setMinimumSize(new Dimension(50, 19));
      mVertFOVField.setPreferredSize(new Dimension(50, 19));
      mVertFOVField.setValue(new Double(80.0));
      mVertFOVField.setHorizontalAlignment(SwingConstants.TRAILING);
      mVertFOVField.setFocusLostBehavior(JFormattedTextField.COMMIT_OR_REVERT);
      mCameraPosLabel.setLabelFor(mCameraPosEditor);
      mCameraPosLabel.setText("Camera Position Proxy");
      mWandPosLabel.setLabelFor(mWandPosEditor);
      mWandPosLabel.setText("Wand Position Proxy");
      this.getContentPane().add(mMainPanel, BorderLayout.CENTER);
      mMainPanel.add(mBoundsPanel, new TableLayoutConstraints(0, 0, 0, 0,
                                                              TableLayout.FULL,
                                                              TableLayout.FULL));
      mMainPanel.add(mUserPanel, new TableLayoutConstraints(0, 1, 0, 1,
                                                            TableLayout.FULL,
                                                            TableLayout.FULL));
      mMainPanel.add(mMiscPanel, new TableLayoutConstraints(0, 2, 0, 2,
                                                            TableLayout.FULL,
                                                            TableLayout.FULL));
      mMiscPanel.add(mVertFOVLabel,
                     new TableLayoutConstraints(0, 0, 0, 0, TableLayout.RIGHT,
                                                TableLayout.FULL));
      mMiscPanel.add(mVertFOVField,
                     new TableLayoutConstraints(2, 0, 2, 0, TableLayout.LEFT,
                                                TableLayout.FULL));
      mMiscPanel.add(mCameraPosLabel,
                     new TableLayoutConstraints(0, 1, 0, 1, TableLayout.RIGHT,
                                                TableLayout.FULL));
      mMiscPanel.add(mCameraPosEditor,
                     new TableLayoutConstraints(2, 1, 2, 1, TableLayout.LEFT,
                                                TableLayout.FULL));
      mMiscPanel.add(mWandPosLabel,
                     new TableLayoutConstraints(0, 2, 0, 2, TableLayout.RIGHT,
                                                TableLayout.FULL));
      mMiscPanel.add(mWandPosEditor,
                     new TableLayoutConstraints(2, 2, 2, 2, TableLayout.LEFT,
                                                TableLayout.FULL));
   }

   private Object mCameraPos = null;
   private Object mWandPos   = null;

   private TitledBorder mBoundsPanelBorder;
   private TitledBorder mUserPanelBorder;
   private JPanel mMiscPanel = new JPanel();
   private TitledBorder mMiscPanelBorder;
   private TableLayout mMiscPanelLayout = null;
   private JLabel mVertFOVLabel = new JLabel();
   private JFormattedTextField mVertFOVField = new JFormattedTextField();
   private JLabel mCameraPosLabel = new JLabel();
   private PropertyEditorPanel mCameraPosEditor = null;
   private JLabel mWandPosLabel = new JLabel();
   private PropertyEditorPanel mWandPosEditor = null;
}
