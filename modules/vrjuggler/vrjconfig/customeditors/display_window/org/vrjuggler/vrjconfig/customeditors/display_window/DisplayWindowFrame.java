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

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import org.vrjuggler.jccl.config.ConfigBrokerProxy;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.event.ConfigElementAdapter;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;

import org.vrjuggler.vrjconfig.commoneditors.placer.PlacerSelectionEvent;
import org.vrjuggler.vrjconfig.commoneditors.placer.PlacerSelectionListener;


public class DisplayWindowFrame
   extends JugglerWindowFrame
   implements PlacerSelectionListener
{
   private static Cursor mInverseCursor;

   private static final int LEFT_EYE;
   private static final int RIGHT_EYE;
   private static final int STEREO;

   /**
    * Constructs the inverse cursor that will be used when the mouse pointer
    * is over DisplayWindowFrame components that represent a display window
    * that hides the mouse cursor.
    */
   static
   {
      Toolkit tk = Toolkit.getDefaultToolkit();
      ClassLoader loader = DisplayWindowFrame.class.getClassLoader();
      Image cursor_img =
         new ImageIcon(loader.getResource(EditorConstants.imageBase + "/inverse-cursor.png")).getImage();
      mInverseCursor = tk.createCustomCursor(cursor_img, new Point(8, 6),
                                             "inverse");

      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition vp_def =
         broker.getRepository().get(EditorConstants.surfaceViewportType);
      java.util.Map views = vp_def.getPropertyDefinition("view").getEnums();

      LEFT_EYE  = ((Integer) views.get("Left Eye")).intValue();
      RIGHT_EYE = ((Integer) views.get("Right Eye")).intValue();
      STEREO    = ((Integer) views.get("Stereo")).intValue();
   }

   public DisplayWindowFrame(String title, Dimension resolution,
                             Dimension desktopSize, ConfigContext ctx,
                             ConfigElement elt)
   {
      super(title, resolution, desktopSize, ctx, elt);

      mElementListener =
         new DisplayWindowFrame_this_configElementAdapter(this);
      mElement.addConfigElementListener(mElementListener);

      try
      {
         mViewportEditor = new ViewportPlacer(this.getSize(), mContext,
                                              mElement);
         mViewportEditor.getPlacer().addPlacerSelectionListener(this);

         jbInit();

         placeMyself();

         mViewportEditor.getPlacer().addMouseListener(
            new DisplayWindowFrame_this_mouseInputAdapter(this)
         );

         mViewButtonGroup.add(mViewportLeftEyeItem);
         mViewButtonGroup.add(mViewportRightEyeItem);
         mViewButtonGroup.add(mViewportStereoItem);

         updateContextMenuItems();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   public void valueChanged(PlacerSelectionEvent e)
   {
      mSelectedViewport = ((ConfigElement) e.getValue());

      boolean enabled = mSelectedViewport != null;
      mViewportPropsItem.setEnabled(enabled);
      mViewportActiveItem.setEnabled(enabled);
      mViewportLeftEyeItem.setEnabled(enabled);
      mViewportRightEyeItem.setEnabled(enabled);
      mViewportStereoItem.setEnabled(enabled);
      mViewportRemoveItem.setEnabled(enabled);

      if ( enabled )
      {
         boolean active = mSelectedViewport.getProperty("active", 0).equals(Boolean.TRUE);
         mViewportActiveItem.setSelected(active);

         int view = ((Integer) mSelectedViewport.getProperty("view", 0)).intValue();

         if ( view == LEFT_EYE )
         {
            mViewButtonGroup.setSelected(mViewportLeftEyeItem.getModel(), true);
         }
         else if ( view == RIGHT_EYE )
         {
            mViewButtonGroup.setSelected(mViewportRightEyeItem.getModel(), true);
         }
         else
         {
            mViewButtonGroup.setSelected(mViewportStereoItem.getModel(), true);
         }
      }
      else
      {
         mViewportActiveItem.setSelected(false);
      }
   }

   public void addSurfaceViewport(ConfigElement surfaceVP)
   {
      mElement.addProperty("surface_viewports", surfaceVP);
   }

   public void addSimulatorViewport(ConfigElement simVP)
   {
      mElement.addProperty("simulator_viewports", simVP);
   }

   public void removeSurfaceViewport(ConfigElement surfaceVP)
   {
      mElement.removeProperty("surface_viewports", surfaceVP);
   }

   public void removeSimulatorViewport(ConfigElement simVP)
   {
      mElement.removeProperty("simulator_viewports", simVP);
   }

   private void jbInit() throws Exception
   {
      mWinPropsItem.setText("Display Window Properties ...");
      mWinPropsItem.addActionListener(new DisplayWindowFrame_mWinPropsItem_actionAdapter(this));
      mWinActiveItem.setText("Is window active?");
      mWinActiveItem.addActionListener(new DisplayWindowFrame_mActiveItem_actionAdapter(this));
      mBorderItem.setText("Window has border");
      mBorderItem.addActionListener(new DisplayWindowFrame_mBorderItem_actionAdapter(this));
      mPointerItem.setText("Hide mouse pointer");
      mPointerItem.addActionListener(new DisplayWindowFrame_mPointerItem_actionAdapter(this));
      mWinStereoItem.setText("Render in stereo");
      mWinStereoItem.addActionListener(new DisplayWindowFrame_mStereoItem_actionAdapter(this));
      mViewportPropsItem.setEnabled(false);
      mViewportPropsItem.setText("Viewport Properties ...");
      mViewportPropsItem.addActionListener(new DisplayWindowFrame_mViewportPropsItem_actionAdapter(this));
      mViewportActiveItem.setEnabled(false);
      mViewportActiveItem.setText("Is viewport active?");
      mViewportActiveItem.addActionListener(new DisplayWindowFrame_mViewportActiveItem_actionAdapter(this));
      mViewportLeftEyeItem.setEnabled(false);
      mViewportLeftEyeItem.setText("Left eye");
      mViewportLeftEyeItem.addActionListener(new DisplayWindowFrame_mViewportLeftEyeItem_actionAdapter(this));
      mViewportRightEyeItem.setEnabled(false);
      mViewportRightEyeItem.setText("Right eye");
      mViewportRightEyeItem.addActionListener(new DisplayWindowFrame_mViewportRightEyeItem_actionAdapter(this));
      mViewportStereoItem.setEnabled(false);
      mViewportStereoItem.setText("Stereo");
      mViewportStereoItem.addActionListener(new DisplayWindowFrame_mViewportStereoItem_actionAdapter(this));
      mViewportRemoveItem.setEnabled(false);
      mViewportRemoveItem.setText("Delete viewport");
      mViewportRemoveItem.addActionListener(new DisplayWindowFrame_mViewportRemoveItem_actionAdapter(this));
      this.addInternalFrameListener(new DisplayWindowFrame_this_internalFrameAdapter(this));
      mContextMenu.add(mWinActiveItem);
      mContextMenu.add(mBorderItem);
      mContextMenu.add(mPointerItem);
      mContextMenu.add(mWinStereoItem);
      mContextMenu.addSeparator();
      mContextMenu.add(mViewportActiveItem);
      mContextMenu.add(mViewportLeftEyeItem);
      mContextMenu.add(mViewportRightEyeItem);
      mContextMenu.add(mViewportStereoItem);
      mContextMenu.add(mViewportRemoveItem);
      mContextMenu.addSeparator();
      mContextMenu.add(mWinPropsItem);
      mContextMenu.add(mViewportPropsItem);
      this.getContentPane().add(mViewportEditor, BorderLayout.CENTER);
   }

   private void updateContextMenuItems()
   {
      boolean active = ((Boolean) mElement.getProperty("active", 0)).booleanValue();
      mWinActiveItem.setSelected(active);

      boolean border = ((Boolean) mElement.getProperty("border", 0)).booleanValue();
      mBorderItem.setSelected(border);

      boolean pointer = ((Boolean) mElement.getProperty("hide_mouse", 0)).booleanValue();
      mPointerItem.setSelected(pointer);

      boolean stereo = ((Boolean) mElement.getProperty("stereo", 0)).booleanValue();
      mWinStereoItem.setSelected(stereo);
   }

   /**
    * Positions the given Dialog object relative to this window frame.
    */
   private void positionDialog(Dialog dialog, Container parent)
   {
      Dimension dlg_size   = dialog.getPreferredSize();
      Dimension frame_size = parent.getSize();
      Point loc            = parent.getLocation();

      // Set the location of the dialog so that it is centered with respect
      // to this frame.
      dialog.setLocation((frame_size.width - dlg_size.width) / 2 + loc.x,
                         (frame_size.height - dlg_size.height) / 2 + loc.y);
   }

   private DisplayWindowFrame_this_configElementAdapter mElementListener = null;
   private boolean mHideMouse = false;
//   private boolean mMousePressed = false;
   private ViewportPlacer mViewportEditor = null;
   private ConfigElement mSelectedViewport = null;

   private JPopupMenu mContextMenu = new JPopupMenu();
   private JMenuItem mWinPropsItem = new JMenuItem();
   private JCheckBoxMenuItem mWinActiveItem = new JCheckBoxMenuItem();
   private JCheckBoxMenuItem mBorderItem = new JCheckBoxMenuItem();
   private JCheckBoxMenuItem mPointerItem = new JCheckBoxMenuItem();
   private JCheckBoxMenuItem mWinStereoItem = new JCheckBoxMenuItem();

   private JMenuItem mViewportPropsItem = new JMenuItem();
   private JCheckBoxMenuItem mViewportActiveItem = new JCheckBoxMenuItem();
   private JRadioButtonMenuItem mViewportRightEyeItem = new JRadioButtonMenuItem();
   private JRadioButtonMenuItem mViewportStereoItem = new JRadioButtonMenuItem();
   private JRadioButtonMenuItem mViewportLeftEyeItem = new JRadioButtonMenuItem();
   private JMenuItem mViewportRemoveItem = new JMenuItem();
   private ButtonGroup mViewButtonGroup = new ButtonGroup();

   void this_mouseClicked(MouseEvent e)
   {
      checkForPopUp(e);
   }

   void this_mousePressed(MouseEvent e)
   {
      checkForPopUp(e);
   }

   void this_mouseReleased(MouseEvent e)
   {
      checkForPopUp(e);
   }

   private void checkForPopUp(MouseEvent e)
   {
      if ( e.isPopupTrigger() )
      {
         mContextMenu.show(e.getComponent(), e.getX(), e.getY());
      }
   }

   void windowPropsEditSelected(ActionEvent e)
   {
      DisplayWindowStartDialog dlg =
         new DisplayWindowStartDialog(mElement, mDesktopSize);
      positionDialog(dlg, (Container) SwingUtilities.getRoot(this));
      dlg.setVisible(true);

      if ( dlg.getStatus() == DisplayWindowStartDialog.OK_OPTION )
      {
         Rectangle bounds  = dlg.getDisplayWindowBounds();
         mElement.setProperty("origin", 0, new Integer(bounds.x), mContext);
         mElement.setProperty("origin", 1, new Integer(bounds.y), mContext);
         mElement.setProperty("size", 0, new Integer(bounds.width), mContext);
         mElement.setProperty("size", 1, new Integer(bounds.height), mContext);

         ConfigElement fb_cfg =
            (ConfigElement) mElement.getProperty("frame_buffer_config", 0);
         fb_cfg.setProperty("visual_id", 0, dlg.getVisualID(), mContext);
         fb_cfg.setProperty("red_size", 0, dlg.getRedDepth(), mContext);
         fb_cfg.setProperty("green_size", 0, dlg.getGreenDepth(), mContext);
         fb_cfg.setProperty("blue_size", 0, dlg.getBlueDepth(), mContext);
         fb_cfg.setProperty("alpha_size", 0, dlg.getAlphaDepth(), mContext);
         fb_cfg.setProperty("depth_buffer_size", 0, dlg.getDepthBufferSize(),
                            mContext);
         fb_cfg.setProperty("fsaa_enable", 0, dlg.getFSAAEnable(), mContext);

         mElement.setProperty("stereo", 0, dlg.inStereo(), mContext);
         mElement.setProperty("border", 0, dlg.hasBorder(), mContext);
      }
   }

   void windowActiveItemActionPerformed(ActionEvent e)
   {
      mElement.setProperty("active", 0,
                           Boolean.valueOf(mWinActiveItem.isSelected()),
                           mContext);
   }

   void borderItemActionPerformed(ActionEvent e)
   {
      mElement.setProperty("border", 0,
                           Boolean.valueOf(mBorderItem.isSelected()),
                           mContext);
   }

   void pointerItemActionPerformed(ActionEvent e)
   {
      mElement.setProperty("hide_mouse", 0,
                           Boolean.valueOf(mPointerItem.isSelected()),
                           mContext);
      mHideMouse = mPointerItem.isSelected();
   }

   void windowStereoItemActionPerformed(ActionEvent e)
   {
      mElement.setProperty("stereo", 0,
                           Boolean.valueOf(mWinStereoItem.isSelected()),
                           mContext);
   }

   void viewportPropsSelected(ActionEvent e)
   {
      Container owner = (Container) SwingUtilities.getRoot(this);

      float old_origin_x = ((Number) mSelectedViewport.getProperty("origin", 0)).floatValue();
      float old_origin_y = ((Number) mSelectedViewport.getProperty("origin", 1)).floatValue();
      float old_width    = ((Number) mSelectedViewport.getProperty("size", 0)).floatValue();
      float old_height   = ((Number) mSelectedViewport.getProperty("size", 1)).floatValue();

      int status;
      float origin_x = 0.0f, origin_y = 0.0f, width = 0.0f, height = 0.0f;

      if ( mSelectedViewport.getDefinition().getToken().equals(EditorConstants.surfaceViewportType) )
      {
         SurfaceViewportCreateDialog dlg =
            new SurfaceViewportCreateDialog(mContext, mSelectedViewport);
         positionDialog(dlg, owner);
         dlg.setVisible(true);

         status = dlg.getStatus();
         if ( status == DisplayWindowStartDialog.OK_OPTION )
         {
            Rectangle bounds = dlg.getViewportBounds();
            origin_x = (float) bounds.x / 100.0f;
            origin_y = (float) bounds.y / 100.0f;
            width    = (float) bounds.width / 100.0f;
            height   = (float) bounds.height / 100.0f;

            mSelectedViewport.setProperty("origin", 0, new Float(origin_x),
                                          mContext);
            mSelectedViewport.setProperty("origin", 1, new Float(origin_y),
                                          mContext);
            mSelectedViewport.setProperty("size", 0, new Float(width),
                                          mContext);
            mSelectedViewport.setProperty("size", 1, new Float(height),
                                          mContext);
            mSelectedViewport.setProperty("view", 0, dlg.getViewpoint(),
                                          mContext);
            mSelectedViewport.setProperty("user", 0, dlg.getUser(), mContext);

            Point3D[] corners = dlg.getCorners();
            mSelectedViewport.setProperty("lower_left_corner", 0,
                                          new Float(corners[0].x), mContext);
            mSelectedViewport.setProperty("lower_left_corner", 1,
                                          new Float(corners[0].y), mContext);
            mSelectedViewport.setProperty("lower_left_corner", 2,
                                          new Float(corners[0].z), mContext);
            mSelectedViewport.setProperty("lower_right_corner", 0,
                                          new Float(corners[1].x), mContext);
            mSelectedViewport.setProperty("lower_right_corner", 1,
                                          new Float(corners[1].y), mContext);
            mSelectedViewport.setProperty("lower_right_corner", 2,
                                          new Float(corners[1].z), mContext);
            mSelectedViewport.setProperty("upper_right_corner", 0,
                                          new Float(corners[2].x), mContext);
            mSelectedViewport.setProperty("upper_right_corner", 1,
                                          new Float(corners[2].y), mContext);
            mSelectedViewport.setProperty("upper_right_corner", 2,
                                          new Float(corners[2].z), mContext);
            mSelectedViewport.setProperty("upper_left_corner", 0,
                                          new Float(corners[3].x), mContext);
            mSelectedViewport.setProperty("upper_left_corner", 1,
                                          new Float(corners[3].y), mContext);
            mSelectedViewport.setProperty("upper_left_corner", 2,
                                          new Float(corners[3].z), mContext);

            mSelectedViewport.setProperty("tracked", 0, dlg.isTracked(),
                                          mContext);

            if ( dlg.isTracked() == Boolean.TRUE )
            {
               mSelectedViewport.setProperty("tracker_proxy", 0,
                                             dlg.getTrackerProxy(), mContext);
            }
         }
      }
      else
      {
         SimulatorViewportCreateDialog dlg =
            new SimulatorViewportCreateDialog(mContext, mSelectedViewport);
         positionDialog(dlg, owner);
         dlg.setVisible(true);

         status = dlg.getStatus();
         if ( status == DisplayWindowStartDialog.OK_OPTION )
         {
            Rectangle bounds = dlg.getViewportBounds();
            origin_x = (float) bounds.x / 100.0f;
            origin_y = (float) bounds.y / 100.0f;
            width    = (float) bounds.width / 100.0f;
            height   = (float) bounds.height / 100.0f;

            mSelectedViewport.setProperty("origin", 0, new Float(origin_x),
                                          mContext);
            mSelectedViewport.setProperty("origin", 1, new Float(origin_y),
                                          mContext);
            mSelectedViewport.setProperty("size", 0, new Float(width),
                                          mContext);
            mSelectedViewport.setProperty("size", 1, new Float(height),
                                          mContext);
            mSelectedViewport.setProperty("view", 0, dlg.getViewpoint(),
                                          mContext);
            mSelectedViewport.setProperty("user", 0, dlg.getUser(),
                                          mContext);
            mSelectedViewport.setProperty("vertical_fov", 0,
                                          dlg.getVertialFOV(), mContext);

            ConfigElement sim_elt =
               (ConfigElement) mSelectedViewport.getProperty("simulator_plugin", 0);

            sim_elt.setProperty("camera_pos", 0, dlg.getCameraPosition(),
                                mContext);
            sim_elt.setProperty("wand_pos", 0, dlg.getWandPosition(),
                                mContext);
         }
      }

      // If the user changed the bounds of the viewport, the placer needs to
      // be repainted ASAP to reflect that change.
      if ( status == DisplayWindowStartDialog.OK_OPTION )
      {
         if ( old_origin_x != origin_x || old_origin_y != origin_y ||
              old_width != width || old_height != height )
         {
            // Having an optimal repaint call here would be nice, but computing
            // the correct bounding box is not entirely straightforward.
            mViewportEditor.getPlacer().repaint();
         }
      }
   }

   void viewportActiveItemSelected(ActionEvent e)
   {
      mSelectedViewport.setProperty("active", 0,
                                    Boolean.valueOf(mViewportActiveItem.isSelected()),
                                    mContext);
   }

   void viewportLeftEyeItemSelected(ActionEvent e)
   {
      mSelectedViewport.setProperty("view", 0, new Integer(LEFT_EYE),
                                    mContext);
   }

   void viewportRightEyeItemSelected(ActionEvent e)
   {
      mSelectedViewport.setProperty("view", 0, new Integer(RIGHT_EYE),
                                    mContext);
   }

   void viewportStereoItemSelected(ActionEvent e)
   {
      mSelectedViewport.setProperty("view", 0, new Integer(STEREO),
                                    mContext);

      // Since a viewport will use the stereo view, we need to enable
      // stereoscopoic rendering for the whole display window.
      mElement.setProperty("stereo", 0, Boolean.TRUE, mContext);
   }

   void viewportRemoveItemSelected(ActionEvent e)
   {
      String prop;
      if ( mSelectedViewport.getDefinition().getToken().equals(EditorConstants.surfaceViewportType) )
      {
         prop = "surface_viewports";
      }
      else
      {
         prop = "simulator_viewports";
      }

      mElement.removeProperty(prop, mSelectedViewport);
   }

   void frameActivated(InternalFrameEvent e)
   {
      // XXX: Why do we have to do this?
      e.getInternalFrame().getGlassPane().setVisible(true);
   }

   void displayNameChanged(ConfigElementEvent e)
   {
      this.setTitle(mElement.getName());
   }

   void displayPropertyChanged(ConfigElementEvent e)
   {
      updateContextMenuItems();
   }

   protected void updateSize(Rectangle newBounds)
   {
      super.updateSize(newBounds);
      mViewportEditor.setDesktopSize(this.getSize());
   }
}

class DisplayWindowFrame_mActiveItem_actionAdapter implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mActiveItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.windowActiveItemActionPerformed(e);
   }
}

class DisplayWindowFrame_mBorderItem_actionAdapter implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mBorderItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.borderItemActionPerformed(e);
   }
}

class DisplayWindowFrame_mPointerItem_actionAdapter implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mPointerItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.pointerItemActionPerformed(e);
   }
}

class DisplayWindowFrame_mStereoItem_actionAdapter implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mStereoItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.windowStereoItemActionPerformed(e);
   }
}

class DisplayWindowFrame_this_mouseInputAdapter
   extends MouseInputAdapter
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_this_mouseInputAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void mouseClicked(MouseEvent e)
   {
      adaptee.this_mouseClicked(e);
   }
   public void mousePressed(MouseEvent e)
   {
      adaptee.this_mousePressed(e);
   }
   public void mouseReleased(MouseEvent e)
   {
      adaptee.this_mouseReleased(e);
   }
}

class DisplayWindowFrame_mViewportActiveItem_actionAdapter
   implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mViewportActiveItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.viewportActiveItemSelected(e);
   }
}

class DisplayWindowFrame_mViewportLeftEyeItem_actionAdapter
   implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mViewportLeftEyeItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.viewportLeftEyeItemSelected(e);
   }
}

class DisplayWindowFrame_mViewportRightEyeItem_actionAdapter
   implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mViewportRightEyeItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.viewportRightEyeItemSelected(e);
   }
}

class DisplayWindowFrame_mViewportStereoItem_actionAdapter
   implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mViewportStereoItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.viewportStereoItemSelected(e);
   }
}

class DisplayWindowFrame_mViewportRemoveItem_actionAdapter
   implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mViewportRemoveItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.viewportRemoveItemSelected(e);
   }
}

class DisplayWindowFrame_this_internalFrameAdapter extends javax.swing.event.InternalFrameAdapter
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_this_internalFrameAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void internalFrameActivated(InternalFrameEvent e)
   {
      adaptee.frameActivated(e);
   }
}

class DisplayWindowFrame_mWinPropsItem_actionAdapter implements java.awt.event.ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mWinPropsItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.windowPropsEditSelected(e);
   }
}

class DisplayWindowFrame_mViewportPropsItem_actionAdapter implements java.awt.event.ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mViewportPropsItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.viewportPropsSelected(e);
   }
}

class DisplayWindowFrame_this_configElementAdapter extends ConfigElementAdapter
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_this_configElementAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void nameChanged(ConfigElementEvent e)
   {
      adaptee.displayNameChanged(e);
   }
   public void propertyValueChanged(ConfigElementEvent e)
   {
      adaptee.displayPropertyChanged(e);
   }
}
