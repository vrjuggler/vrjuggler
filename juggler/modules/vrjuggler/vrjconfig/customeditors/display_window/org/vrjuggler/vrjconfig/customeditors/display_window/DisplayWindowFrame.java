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
import java.awt.event.*;
import javax.swing.*;
import org.vrjuggler.jccl.config.ConfigBrokerProxy;
import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.event.ConfigElementAdapter;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;

import org.vrjuggler.vrjconfig.customeditors.display_window.placer.PlacerSelectionEvent;
import org.vrjuggler.vrjconfig.customeditors.display_window.placer.PlacerSelectionListener;
import javax.swing.event.*;


public class DisplayWindowFrame
   extends JInternalFrame
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
                             Dimension desktopSize, ConfigElement elt)
   {
      super(title, true, true, true, false);

      mElement = elt;
      mResolution = resolution;
      mDesktopSize = desktopSize;

      mElement.addConfigElementListener(new DisplayWindowFrame_this_configElementAdapter(this));

      try
      {
         mViewportEditor = new ViewportPlacer(this.getSize(), mElement);
         mViewportEditor.getPlacer().addPlacerSelectionListener(this);
         jbInit();
         placeMyself();

         this.getGlassPane().addMouseListener(new DisplayWindowFrame_this_mouseInputAdapter(this));
         this.getGlassPane().addMouseMotionListener(new DisplayWindowFrame_this_mouseInputAdapter(this));
         this.getGlassPane().setVisible(true);

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

   /**
    * Changes the location of this internal frame based on the new resolution.
    * @param resolution Dimension
    */
   public void updateResolution(Dimension resolution)
   {
      mResolution = resolution;
      placeMyself();
   }

   public void updateDesktopSize(Dimension size)
   {
      mDesktopSize = size;
      placeMyself();
   }

   public ConfigElement getConfiguration()
   {
      return mElement;
   }

   public void addSurfaceViewport(ConfigElement surfaceVP)
   {
      mElement.addProperty("surface_viewports", surfaceVP);
   }

   public void addSimulatorViewport(ConfigElement simVP)
   {
      // When we have a simulator viewport, we *have* to act as an event
      // source.
      mElement.setProperty("act_as_event_source", 0, Boolean.TRUE);
      mEventSrcItem.setEnabled(false);

      mElement.addProperty("simulator_viewports", simVP);
   }

   public void removeSurfaceViewport(ConfigElement surfaceVP)
   {
      mElement.removeProperty("surface_viewports", surfaceVP);
   }

   public void removeSimulatorViewport(ConfigElement simVP)
   {
      mElement.removeProperty("simulator_viewports", simVP);

      // If we have reached a point where we no longer have any simulator
      // viewports, we can allow the user to change the event source status.
      if ( mElement.getPropertyValueCount("simulator_viewports") == 0 )
      {
         mEventSrcItem.setEnabled(true);
      }
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
      mEventSrcItem.setText("Acts as an event source");
      mEventSrcItem.addActionListener(new DisplayWindowFrame_mEventSrcItem_actionAdapter(this));
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
      this.addComponentListener(new DisplayWindowFrame_this_componentAdapter(this));
      mViewportRemoveItem.setEnabled(false);
      mViewportRemoveItem.setText("Delete viewport");
      mViewportRemoveItem.addActionListener(new DisplayWindowFrame_mViewportRemoveItem_actionAdapter(this));
      this.addInternalFrameListener(new DisplayWindowFrame_this_internalFrameAdapter(this));
      mContextMenu.add(mWinActiveItem);
      mContextMenu.add(mBorderItem);
      mContextMenu.add(mPointerItem);
      mContextMenu.add(mWinStereoItem);
      mContextMenu.add(mEventSrcItem);
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

   private Point desktopToResolution(Point desktopPoint)
   {
      float width_conv  = (float) mResolution.width / (float) mDesktopSize.width;
      float height_conv = (float) mResolution.height / (float) mDesktopSize.height;
//      System.out.println("To-resolution width conversion: " + width_conv);
//      System.out.println("To-resolution height conversion: " + height_conv);
      return new Point((int) ((float) desktopPoint.x * width_conv),
                       (int) ((float) desktopPoint.y * height_conv));
   }

   private Point resolutionToDesktop(Point resolutionPoint)
   {
      float width_conv  = (float) mDesktopSize.width / (float) mResolution.width;
      float height_conv = (float) mDesktopSize.height / (float) mResolution.height;
//      System.out.println("To-desktop width conversion: " + width_conv);
//      System.out.println("To-desktop height conversion: " + height_conv);
      return new Point((int) ((float) resolutionPoint.x * width_conv),
                       (int) ((float) resolutionPoint.y * height_conv));
   }

   private void placeMyself()
   {
      int cfg_size_x = ((Integer) mElement.getProperty("size", 0)).intValue();
      int cfg_size_y = ((Integer) mElement.getProperty("size", 1)).intValue();
      Point size = resolutionToDesktop(new Point(cfg_size_x, cfg_size_y));
      System.out.println("Setting my size to " + size);
      this.setSize(size.x, size.y);

      int cfg_origin_x = ((Integer) mElement.getProperty("origin", 0)).intValue();
      int cfg_origin_y =
         mResolution.height -
            (((Integer) mElement.getProperty("origin", 1)).intValue() +
            cfg_size_y);
      Point origin = resolutionToDesktop(new Point(cfg_origin_x,
                                                   cfg_origin_y));
      System.out.println("Setting my origin to " + origin);
      this.setLocation(origin.x, origin.y);
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

      boolean event_src = ((Boolean) mElement.getProperty("act_as_event_source", 0)).booleanValue();
      mEventSrcItem.setSelected(event_src);
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

   private Dimension mResolution = null;
   private Dimension mDesktopSize = null;
   private ConfigElement mElement = null;
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
   private JCheckBoxMenuItem mEventSrcItem = new JCheckBoxMenuItem();

   private JMenuItem mViewportPropsItem = new JMenuItem();
   private JCheckBoxMenuItem mViewportActiveItem = new JCheckBoxMenuItem();
   private JRadioButtonMenuItem mViewportRightEyeItem = new JRadioButtonMenuItem();
   private JRadioButtonMenuItem mViewportStereoItem = new JRadioButtonMenuItem();
   private JRadioButtonMenuItem mViewportLeftEyeItem = new JRadioButtonMenuItem();
   private JMenuItem mViewportRemoveItem = new JMenuItem();
   private ButtonGroup mViewButtonGroup = new ButtonGroup();

   private void redispatchMouseEvent(MouseEvent e)
   {
      Point glass_pane_point = e.getPoint();
      Container container = this.getContentPane();
      Point container_point = SwingUtilities.convertPoint(this.getGlassPane(),
                                                          glass_pane_point,
                                                          container);

      if ( container_point.y < 0 )
      {
         // XXX: What to do here?
      }
      else
      {
         // The mouse event is probably over the content pane.
         // Find out exactly which component it's over.
         Component component =
            SwingUtilities.getDeepestComponentAt(container, container_point.x,
                                                 container_point.y);

         if ( component != null )
         {
            Point component_point =
               SwingUtilities.convertPoint(this.getGlassPane(),
                                           glass_pane_point, component);

            // Forward the mouse event on to the viewport editor panel.
            component.dispatchEvent(new MouseEvent(component, e.getID(),
                                                   e.getWhen(),
                                                   e.getModifiers(),
                                                   component_point.x,
                                                   component_point.y,
                                                   e.getClickCount(),
                                                   e.isPopupTrigger()));
         }
      }
   }

   void this_mouseEntered(MouseEvent e)
   {
      if ( mHideMouse )
      {
         this.setCursor(mInverseCursor);
      }

      redispatchMouseEvent(e);
   }

   void this_mouseExited(MouseEvent e)
   {
      if ( mHideMouse )
      {
         this.setCursor(Cursor.getDefaultCursor());
      }

      redispatchMouseEvent(e);
   }

   void this_mouseClicked(MouseEvent e)
   {
      if ( ! checkForPopUp(e) )
      {
         redispatchMouseEvent(e);
      }
   }

   void this_mousePressed(MouseEvent e)
   {
      if ( ! checkForPopUp(e) )
      {
/*
         if ( e.getButton() == MouseEvent.BUTTON1 )
         {
            System.out.println("Mouse pressed");
            mMousePressed = true;
         }
*/
         redispatchMouseEvent(e);
      }
   }

   void this_mouseReleased(MouseEvent e)
   {
      if ( ! checkForPopUp(e) )
      {
/*
         if ( e.getButton() == MouseEvent.BUTTON1 )
         {
            System.out.println("Mouse released");
            mMousePressed = false;
         }
*/
         redispatchMouseEvent(e);
      }
   }

   private boolean checkForPopUp(MouseEvent e)
   {
      if ( e.isPopupTrigger() )
      {
         mContextMenu.show(e.getComponent(), e.getX(), e.getY());
         return true;
      }
      else
      {
         return false;
      }
   }

   void windowPropsEditSelected(ActionEvent e)
   {
      DisplayWindowStartDialog dlg =
         new DisplayWindowStartDialog(mElement, mDesktopSize);
      positionDialog(dlg, (Container) SwingUtilities.getRoot(this));
      dlg.show();

      if ( dlg.getStatus() == DisplayWindowStartDialog.OK_OPTION )
      {

         Rectangle bounds  = dlg.getDisplayWindowBounds();
         mElement.setProperty("origin", 0, new Integer(bounds.x));
         mElement.setProperty("origin", 1, new Integer(bounds.y));
         mElement.setProperty("size", 0, new Integer(bounds.width));
         mElement.setProperty("size", 1, new Integer(bounds.height));

         ConfigElement fb_cfg =
            (ConfigElement) mElement.getProperty("frame_buffer_config", 0);
         fb_cfg.setProperty("visual_id", 0, dlg.getVisualID());
         fb_cfg.setProperty("red_size", 0, dlg.getRedDepth());
         fb_cfg.setProperty("green_size", 0, dlg.getGreenDepth());
         fb_cfg.setProperty("blue_size", 0, dlg.getBlueDepth());
         fb_cfg.setProperty("alpha_size", 0, dlg.getAlphaDepth());
         fb_cfg.setProperty("depth_buffer_size", 0, dlg.getDepthBufferSize());
         fb_cfg.setProperty("fsaa_enable", 0, dlg.getFSAAEnable());

         mElement.setProperty("stereo", 0, dlg.inStereo());
         mElement.setProperty("border", 0, dlg.hasBorder());
         mElement.setProperty("act_as_event_source", 0, dlg.isEventSource());

         updateContextMenuItems();
      }
   }

   void windowActiveItemActionPerformed(ActionEvent e)
   {
      mElement.setProperty("active", 0,
                           Boolean.valueOf(mWinActiveItem.isSelected()));
   }

   void borderItemActionPerformed(ActionEvent e)
   {
      mElement.setProperty("border", 0,
                           Boolean.valueOf(mBorderItem.isSelected()));
   }

   void pointerItemActionPerformed(ActionEvent e)
   {
      mElement.setProperty("hide_mouse", 0,
                           Boolean.valueOf(mPointerItem.isSelected()));
      mHideMouse = mPointerItem.isSelected();
   }

   void windowStereoItemActionPerformed(ActionEvent e)
   {
      mElement.setProperty("stereo", 0,
                           Boolean.valueOf(mWinStereoItem.isSelected()));
   }

   void eventSrcItemActionPerformed(ActionEvent e)
   {
      mElement.setProperty("act_as_event_source", 0,
                           Boolean.valueOf(mEventSrcItem.isSelected()));
   }

   void viewportPropsSelected(ActionEvent e)
   {
      Container owner = (Container) SwingUtilities.getRoot(this);

      if ( mSelectedViewport.getDefinition().getToken().equals(EditorConstants.surfaceViewportType) )
      {
         SurfaceViewportCreateDialog dlg =
            new SurfaceViewportCreateDialog(mSelectedViewport);
         positionDialog(dlg, owner);
         dlg.show();

         if ( dlg.getStatus() == DisplayWindowStartDialog.OK_OPTION )
         {
            Rectangle bounds = dlg.getViewportBounds();
            float origin_x = (float) bounds.x / 100.0f;
            float origin_y = (float) bounds.y / 100.0f;
            float width    = (float) bounds.width / 100.0f;
            float height   = (float) bounds.height / 100.0f;

            mSelectedViewport.setProperty("origin", 0, new Float(origin_x));
            mSelectedViewport.setProperty("origin", 1, new Float(origin_y));
            mSelectedViewport.setProperty("size", 0, new Float(width));
            mSelectedViewport.setProperty("size", 1, new Float(height));
            mSelectedViewport.setProperty("view", 0, dlg.getViewpoint());
            mSelectedViewport.setProperty("user", 0, dlg.getUser());

            Point3D[] corners = dlg.getCorners();
            mSelectedViewport.setProperty("lower_left_corner", 0,
                                          new Float(corners[0].x));
            mSelectedViewport.setProperty("lower_left_corner", 1,
                                          new Float(corners[0].y));
            mSelectedViewport.setProperty("lower_left_corner", 2,
                                          new Float(corners[0].z));
            mSelectedViewport.setProperty("lower_right_corner", 0,
                                          new Float(corners[1].x));
            mSelectedViewport.setProperty("lower_right_corner", 1,
                                          new Float(corners[1].y));
            mSelectedViewport.setProperty("lower_right_corner", 2,
                                          new Float(corners[1].z));
            mSelectedViewport.setProperty("upper_right_corner", 0,
                                          new Float(corners[2].x));
            mSelectedViewport.setProperty("upper_right_corner", 1,
                                          new Float(corners[2].y));
            mSelectedViewport.setProperty("upper_right_corner", 2,
                                          new Float(corners[2].z));
            mSelectedViewport.setProperty("upper_left_corner", 0,
                                          new Float(corners[3].x));
            mSelectedViewport.setProperty("upper_left_corner", 1,
                                          new Float(corners[3].y));
            mSelectedViewport.setProperty("upper_left_corner", 2,
                                          new Float(corners[3].z));

            mSelectedViewport.setProperty("tracked", 0, dlg.isTracked());

            if ( dlg.isTracked() == Boolean.TRUE )
            {
               mSelectedViewport.setProperty("tracker_proxy", 0,
                                             dlg.getTrackerProxy());
            }
         }
      }
      else
      {
         SimulatorViewportCreateDialog dlg =
            new SimulatorViewportCreateDialog(mSelectedViewport);
         positionDialog(dlg, owner);
         dlg.show();

         if ( dlg.getStatus() == DisplayWindowStartDialog.OK_OPTION )
         {
            Rectangle bounds = dlg.getViewportBounds();
            float origin_x = (float) bounds.x / 100.0f;
            float origin_y = (float) bounds.y / 100.0f;
            float width    = (float) bounds.width / 100.0f;
            float height   = (float) bounds.height / 100.0f;

            mSelectedViewport.setProperty("origin", 0, new Float(origin_x));
            mSelectedViewport.setProperty("origin", 1, new Float(origin_y));
            mSelectedViewport.setProperty("size", 0, new Float(width));
            mSelectedViewport.setProperty("size", 1, new Float(height));
            mSelectedViewport.setProperty("view", 0, dlg.getViewpoint());
            mSelectedViewport.setProperty("user", 0, dlg.getUser());
            mSelectedViewport.setProperty("vertical_fov", 0,
                                          dlg.getVertialFOV());

            ConfigElement sim_elt =
               (ConfigElement) mSelectedViewport.getProperty("simulator_plugin", 0);

            sim_elt.setProperty("camera_pos", 0, dlg.getCameraPosition());
            sim_elt.setProperty("wand_pos", 0, dlg.getWandPosition());
         }
      }
   }

   void viewportActiveItemSelected(ActionEvent e)
   {
      mSelectedViewport.setProperty("active", 0,
                                    Boolean.valueOf(mViewportActiveItem.isSelected()));
   }

   void viewportLeftEyeItemSelected(ActionEvent e)
   {
      mSelectedViewport.setProperty("view", 0, new Integer(LEFT_EYE));
   }

   void viewportRightEyeItemSelected(ActionEvent e)
   {
      mSelectedViewport.setProperty("view", 0, new Integer(RIGHT_EYE));
   }

   void viewportStereoItemSelected(ActionEvent e)
   {
      mSelectedViewport.setProperty("view", 0, new Integer(STEREO));
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

   void this_mouseDragged(MouseEvent e)
   {
   }

   void this_mouseMoved(MouseEvent e)
   {
      if ( mHideMouse )
      {
         this.setCursor(mInverseCursor);
      }

      redispatchMouseEvent(e);
   }

   void this_componentMoved(ComponentEvent e)
   {
//      if ( ! mMousePressed )
//      {
         updatePosition(e.getComponent().getBounds());
//      }
   }

   void this_componentResized(ComponentEvent e)
   {
//      if ( ! mMousePressed )
//      {
         Rectangle bounds = e.getComponent().getBounds();
         updateSize(bounds);
         updatePosition(bounds);
//      }
   }

   private void updatePosition(Rectangle newBounds)
   {
      int y_pos = mDesktopSize.height - (newBounds.y + newBounds.height);
      Point origin = desktopToResolution(new Point(newBounds.x, y_pos));
      mElement.setProperty("origin", 0, new Integer(origin.x));
      mElement.setProperty("origin", 1, new Integer(origin.y));
   }

   private void updateSize(Rectangle newBounds)
   {
      Point size = desktopToResolution(new Point(newBounds.width,
                                                 newBounds.height));
      mElement.setProperty("size", 0, new Integer(size.x));
      mElement.setProperty("size", 1, new Integer(size.y));
      mViewportEditor.setDesktopSize(this.getSize());
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

class DisplayWindowFrame_mEventSrcItem_actionAdapter implements ActionListener
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_mEventSrcItem_actionAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.eventSrcItemActionPerformed(e);
   }
}

class DisplayWindowFrame_this_mouseInputAdapter
   extends javax.swing.event.MouseInputAdapter
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_this_mouseInputAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void mouseEntered(MouseEvent e)
   {
      adaptee.this_mouseEntered(e);
   }
   public void mouseExited(MouseEvent e)
   {
      adaptee.this_mouseExited(e);
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
   public void mouseDragged(MouseEvent e)
   {
      adaptee.this_mouseDragged(e);
   }
   public void mouseMoved(MouseEvent e)
   {
      adaptee.this_mouseMoved(e);
   }
}

class DisplayWindowFrame_this_componentAdapter extends ComponentAdapter
{
   private DisplayWindowFrame adaptee;

   DisplayWindowFrame_this_componentAdapter(DisplayWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void componentMoved(ComponentEvent e)
   {
      adaptee.this_componentMoved(e);
   }
   public void componentResized(ComponentEvent e)
   {
      adaptee.this_componentResized(e);
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
