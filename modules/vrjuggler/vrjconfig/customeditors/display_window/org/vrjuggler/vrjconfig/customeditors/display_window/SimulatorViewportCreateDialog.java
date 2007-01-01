/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
import javax.swing.border.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;


public class SimulatorViewportCreateDialog
   extends ViewportCreateDialog
   implements EditorConstants
{
   public SimulatorViewportCreateDialog(Container parent, ConfigContext ctx)
   {
      this(parent, ctx, null);
   }

   public SimulatorViewportCreateDialog(Container parent, ConfigContext ctx,
                                        ConfigElement viewportElt)
   {
      super("Basic Simulator Viewport Parameters", ctx, viewportElt,
            SIMULATOR_VIEWPORT_TYPE);

      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition sim_def =
         broker.getRepository().get(DEFAULT_SIMULATOR_TYPE);

      if ( mViewportElement.getPropertyValues(SIMULATOR_PLUGIN_PROPERTY).isEmpty() )
      {
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());
         mSimElt = factory.create("Default Simulator Plug-in", sim_def);
         mViewportElement.addProperty(SIMULATOR_PLUGIN_PROPERTY, mSimElt, ctx);
      }
      else
      {
         mSimElt =
            (ConfigElement) mViewportElement.getProperty(SIMULATOR_PLUGIN_PROPERTY, 0);
      }

      mSimElt.addConfigElementListener(this);

      mCameraPosEditor =
         new PropertyEditorPanel(
            ctx, mSimElt.getProperty(CAMERA_POS_PROPERTY, 0),
            sim_def.getPropertyDefinition(CAMERA_POS_PROPERTY), mSimElt, 0,
            Color.white
         );
      mWandPosEditor =
         new PropertyEditorPanel(
            ctx, mSimElt.getProperty(WAND_POS_PROPERTY, 0),
            sim_def.getPropertyDefinition(WAND_POS_PROPERTY), mSimElt, 0,
            Color.white
         );

      initUI();
      validateUserInput();
      this.pack();
      this.setLocationRelativeTo(parent);
   }

   public Float getVertialFOV()
   {
      return (Float) mVertFOVField.getValue();
   }

   public Object getCameraPosition()
   {
      return mSimElt.getProperty(CAMERA_POS_PROPERTY, 0);
   }

   public Object getWandPosition()
   {
      return mSimElt.getProperty(WAND_POS_PROPERTY, 0);
   }

   protected boolean validateCustomInput()
   {
      boolean valid_fov, camera_set, wand_set;

      valid_fov  = ((Float) mVertFOVField.getValue()).doubleValue() > 0.0;
      camera_set = (((ConfigElementPointer) getCameraPosition()).getTarget() != null);
      wand_set   = (((ConfigElementPointer) getWandPosition()).getTarget() != null);

      return valid_fov && camera_set && wand_set;
   }

   protected void initUI()
   {
      super.initUI();

      double[][] main_size =
         {
            {TableLayout.PREFERRED},
            {TableLayout.PREFERRED, TableLayout.PREFERRED,
             TableLayout.PREFERRED}
         };
      mMainPanelLayout = new TableLayout(main_size);

      double[][] fov_size =
         {
            {TableLayout.PREFERRED, 10, TableLayout.PREFERRED},
            {TableLayout.PREFERRED, TableLayout.PREFERRED,
             TableLayout.PREFERRED}
         };
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
      mVertFOVField.setValue(new Float(80.0));
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

   private ConfigElement mSimElt = null;

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
