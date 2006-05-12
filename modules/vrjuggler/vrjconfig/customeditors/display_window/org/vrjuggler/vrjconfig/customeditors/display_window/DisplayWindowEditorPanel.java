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
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;
import java.beans.*;


public class DisplayWindowEditorPanel
   extends JPanel
   implements CustomEditor
            , InternalFrameListener
            , EditorConstants
{
   /**
    * For testing purposes only.
    *
    * @param args String[] Ignored arguments.
    */
   public static void main(String[] args)
   {
      JFrame frame = new JFrame();
      frame.getContentPane().add(new DisplayWindowEditorPanel());
      frame.pack();
      frame.setVisible(true);
   }

   public DisplayWindowEditorPanel()
   {
      // Register ourselves with the CustomEditorRegistry.
      CustomEditorRegistry.registerEditor(DISPLAY_WINDOW_TYPE,
                                          DisplayWindowEditorPanel.class);

      try
      {
         jbInit();
         mWidthField.setValue(new Integer(mCurrentResolution.width));
         mHeightField.setValue(new Integer(mCurrentResolution.height));
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      ClassLoader loader = getClass().getClassLoader();
      String root_path = IMAGE_BASE;

      try
      {
         ImageIcon window_icon =
            new ImageIcon(loader.getResource(root_path + "/vrjuggler-display-window-icon.png"));
         mDisplayCreateButton.setIcon(window_icon);
         ImageIcon surface_viewport_icon =
            new ImageIcon(loader.getResource(root_path + "/vrjuggler-surface-viewport-icon.png"));
         mSurfaceViewportButton.setIcon(surface_viewport_icon);
         ImageIcon sim_viewport_icon =
            new ImageIcon(loader.getResource(root_path + "/vrjuggler-sim-viewport-icon.png"));
         mSimViewportButton.setIcon(sim_viewport_icon);
         ImageIcon kbd_icon =
            new ImageIcon(loader.getResource(root_path + "/vrjuggler-input-window-add-icon.png"));
         mInputWinCreateButton.setIcon(kbd_icon);

         int width, height;
         int buffer = 6;
         width  = window_icon.getIconWidth() + buffer;
         height = window_icon.getIconHeight() + buffer;
         mDisplayCreateButton.setMinimumSize(new Dimension(width, height));
         mDisplayCreateButton.setPreferredSize(new Dimension(width, height));

         width  = surface_viewport_icon.getIconWidth() + buffer;
         height = surface_viewport_icon.getIconHeight() + buffer;
         mSurfaceViewportButton.setMinimumSize(new Dimension(width, height));
         mSurfaceViewportButton.setPreferredSize(new Dimension(width, height));

         width  = sim_viewport_icon.getIconWidth() + buffer;
         height = sim_viewport_icon.getIconHeight() + buffer;
         mSimViewportButton.setMinimumSize(new Dimension(width, height));
         mSimViewportButton.setPreferredSize(new Dimension(width, height));

         width  = kbd_icon.getIconWidth() + buffer;
         height = kbd_icon.getIconHeight() + buffer;
         mInputWinCreateButton.setMinimumSize(new Dimension(width, height));
         mInputWinCreateButton.setPreferredSize(new Dimension(width, height));
      }
      // If any icon fails to load, we fall back on plain old text for the
      // button labels.
      catch (NullPointerException ex)
      {
         System.err.println("WARNING: Failed to load icons for display window editor");
         if ( ex.getMessage() != null )
         {
            System.err.println(ex.getMessage());
         }

         mDisplayCreateButton.setIcon(null);
         mSurfaceViewportButton.setIcon(null);
         mSimViewportButton.setIcon(null);
         mInputWinCreateButton.setIcon(null);

         mDisplayCreateButton.setText("Add Display Window ...");
         mSurfaceViewportButton.setText("Add Surface Viewport");
         mSimViewportButton.setText("Add Simulator Viewport");
         mInputWinCreateButton.setText("Add Input Window ...");
      }

      // Set a background image for the JDesktopPane that will display the
      // representations of display windows.
      try
      {
         mDesktopBackground.setImageFile(loader.getResource(root_path + "/empty-desktop.png"));
         mDesktopBackground.setOpaque(true);
         mDesktopBackground.setBounds(0, 0, 800, 600);
         mDesktopEditor.add(mDesktopBackground, JLayeredPane.FRAME_CONTENT_LAYER);
      }
      catch (NullPointerException ex)
      {
         System.err.println("WARNING: Failed to set up desktop background image");
         if ( ex.getMessage() != null )
         {
            System.err.println(ex.getMessage());
         }
      }
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      mContext = ctx;

      // If elt is null, then we are acting as a multi-element editor.
      // In that case, we want to act as a full-blown desktop simulator
      // editor.
      if ( null == elt )
      {
         mSimEditor = true;

         // We will not be using mElement in this mode.
         mElement = null;

         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         java.util.List all_elts = broker.getElements(mContext);

         for ( Iterator i = all_elts.iterator(); i.hasNext(); )
         {
            ConfigElement cur_elt = (ConfigElement) i.next();
            String def_type = cur_elt.getDefinition().getToken();

            if ( def_type.equals(DISPLAY_WINDOW_TYPE) )
            {
               addDisplayWindow(cur_elt);
            }
            else if ( def_type.equals(INPUT_WINDOW_TYPE) )
            {
               addInputWindow(cur_elt);
            }
         }
      }
      // If elt is not null, then we are acting as a single-element editor.
      // In that case, we only want to allow editing of the display_window
      // element that we were given.
      else
      {
         mSimEditor = false;
         mElement   = elt;

         // Disable all user interface components that result in the creation
         // of new top-level (i.e., non-embedded) config elements.
         mDisplayCreateButton.setEnabled(false);
         mInputWinCreateButton.setEnabled(false);
         mAddDisplayItem.setEnabled(false);
         mAddInputWinItem.setEnabled(false);

         addDisplayWindow(mElement);
      }
   }

   public Container getPanel()
   {
      return this;
   }

   public String getTitle()
   {
      return "Display Window Editor";
   }

   public ActionListener getHelpActionListener()
   {
      return null;
   }

   public void editorClosing()
   {
   }

   public void internalFrameActivated(InternalFrameEvent e)
   {
      setActiveJugglerWindow((JugglerWindowFrame) e.getInternalFrame());
   }

   public void internalFrameClosed(InternalFrameEvent e)
   {
      JugglerWindowFrame win = (JugglerWindowFrame) e.getInternalFrame();

      // Only change the active window if the internal frame being closed is
      // the same as the active window.
      if ( mActiveWindow == win )
      {
         setActiveJugglerWindow(null);
      }

      ConfigElement elt = win.getConfiguration();

      ConfigBroker broker = new ConfigBrokerProxy();
      for ( Iterator i = broker.getResourceNames().iterator(); i.hasNext(); )
      {
         DataSource ds = broker.get((String) i.next());

         if ( ds.contains(elt) )
         {
            broker.remove(mContext, elt, ds);
            break;
         }
      }

      mAllWindows.remove(win);
   }

   public void internalFrameClosing(InternalFrameEvent e)
   {
      /* Nothing to do for this event. */ ;
   }

   public void internalFrameDeactivated(InternalFrameEvent e)
   {
      setActiveJugglerWindow(null);
   }

   public void internalFrameDeiconified(InternalFrameEvent e)
   {
      /* Nothing to do for this event. */ ;
   }

   public void internalFrameIconified(InternalFrameEvent e)
   {
      /* Nothing to do for this event. */ ;
   }

   public void internalFrameOpened(InternalFrameEvent e)
   {
      // This may not be necessary if internalFrameActivated() is called if a
      // newly opened window is automatically selected.
      if ( e.getInternalFrame().isSelected() )
      {
         if ( e.getInternalFrame() == mActiveWindow )
         {
            System.err.println("WARNING: Opened internal frame is already active");
         }

         setActiveJugglerWindow((JugglerWindowFrame) e.getInternalFrame());
      }
   }

   private void jbInit() throws Exception
   {
      double[][] button_size =
         {{5, TableLayout.PREFERRED, 5},
          {TableLayout.FILL, TableLayout.PREFERRED, TableLayout.PREFERRED, 10,
           TableLayout.PREFERRED, TableLayout.PREFERRED, 10,
           TableLayout.PREFERRED, TableLayout.PREFERRED, 10,
           TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.FILL}};
      mButtonPanelLayout = new TableLayout(button_size);

      this.setLayout(mMainLayout);
      mMainEditorPanel.setLayout(mMainEditorLayout);
      mButtonPanel.setLayout(mButtonPanelLayout);
      mDisplayCreateButton.setToolTipText("Add a new display window to the desktop");
      mDisplayCreateButton.addActionListener(new DisplayWindowEditorPanel_mDisplayCreateButton_actionAdapter(this));
      mSurfaceViewportButton.setEnabled(false);
      mSurfaceViewportButton.setToolTipText("Add a new Surface Viewport to the display window ");
      mSurfaceViewportButton.addActionListener(new DisplayWindowEditorPanel_mSurfaceViewportButton_actionAdapter(this));
      mSimViewportButton.setEnabled(false);
      mSimViewportButton.setToolTipText("Add a new Simulator Viewport to the display window");
      mSimViewportButton.addActionListener(new DisplayWindowEditorPanel_mSimViewportButton_actionAdapter(this));
      mInputWinCreateButton.setToolTipText("Add a new input window to the desktop");
      mInputWinCreateButton.addActionListener(new DisplayWindowEditorPanel_mInputWinCreateButton_actionAdapter(this));
      mResolutionLabel.setLabelFor(mWidthField);
      mResolutionLabel.setText("Screen Resolution:");
      mWidthField.setMinimumSize(new Dimension(50, 21));
      mWidthField.setPreferredSize(new Dimension(50, 21));
      mWidthField.setToolTipText("Screen resolution width");
      mWidthField.setHorizontalAlignment(SwingConstants.TRAILING);
      mWidthField.addPropertyChangeListener(new DisplayWindowEditorPanel_mWidthField_propertyChangeAdapter(this));
      mWidthField.addFocusListener(new DisplayWindowEditorPanel_Resolution_focusAdapter(this));
      mResolutionXLabel.setRequestFocusEnabled(true);
      mResolutionXLabel.setText("\u00D7");
      mHeightField.setMinimumSize(new Dimension(50, 21));
      mHeightField.setPreferredSize(new Dimension(50, 21));
      mHeightField.setToolTipText("Screen resolution height");
      mHeightField.addFocusListener(new DisplayWindowEditorPanel_Resolution_focusAdapter(this));
      mDesktopEditor.setBorder(BorderFactory.createLoweredBevelBorder());
      mDesktopEditor.setMinimumSize(new Dimension(800, 600));
      mDesktopEditor.setPreferredSize(new Dimension(800, 600));
      mDesktopEditor.setToolTipText("Desktop editor: add a new display to begin");
      mDesktopEditor.addMouseListener(new DisplayWindowEditorPanel_mDesktopEditor_mouseAdapter(this));
      mDesktopEditor.addComponentListener(new DisplayWindowEditorPanel_mDesktopEditor_componentAdapter(this));
      mButtonPanel.setBackground(Color.darkGray);
      mButtonPanel.setBorder(BorderFactory.createEtchedBorder());
      mEditorPanel.setLayout(mEditorPanelLayout);
      mDisplayCreateLabel.setFont(new java.awt.Font("Dialog", 0, 11));
      mDisplayCreateLabel.setForeground(Color.white);
      mDisplayCreateLabel.setMaximumSize(new Dimension(64, 28));
      mDisplayCreateLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mDisplayCreateLabel.setLabelFor(mDisplayCreateButton);
      mDisplayCreateLabel.setText("Display");
      mSurfaceViewportLabel.setFont(new java.awt.Font("Dialog", 0, 11));
      mSurfaceViewportLabel.setForeground(Color.white);
      mSurfaceViewportLabel.setMaximumSize(new Dimension(64, 28));
      mSurfaceViewportLabel.setMinimumSize(new Dimension(64, 14));
      mSurfaceViewportLabel.setPreferredSize(new Dimension(64, 28));
      mSurfaceViewportLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mSurfaceViewportLabel.setLabelFor(mSurfaceViewportButton);
      mSurfaceViewportLabel.setText("<html>Surface<br>Viewport</html>");
      mSimViewportLabel.setFont(new java.awt.Font("Dialog", 0, 11));
      mSimViewportLabel.setForeground(Color.white);
      mSimViewportLabel.setMaximumSize(new Dimension(64, 28));
      mSimViewportLabel.setMinimumSize(new Dimension(64, 14));
      mSimViewportLabel.setPreferredSize(new Dimension(64, 28));
      mSimViewportLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mSimViewportLabel.setLabelFor(mSimViewportButton);
      mSimViewportLabel.setText("<html>Simulator<br>Viewport</html>");
      mInputWinCreateLabel.setFont(new java.awt.Font("Dialog", 0, 11));
      mInputWinCreateLabel.setForeground(Color.white);
      mInputWinCreateLabel.setMaximumSize(new Dimension(64, 28));
      mInputWinCreateLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mInputWinCreateLabel.setLabelFor(mInputWinCreateButton);
      mInputWinCreateLabel.setText("Input Window");
      mMainPanel.setLayout(mMainPanelLayout);
      mAddDisplayItem.setToolTipText("Add a new display window to the desktop");
      mAddDisplayItem.setText("Add Display Window ...");
      mAddDisplayItem.addActionListener(new DisplayWindowEditorPanel_mAddDisplayItem_actionAdapter(this));
      mAddInputWinItem.setToolTipText("Add a new input window to the desktop");
      mAddInputWinItem.setText("Add Input Window ...");
      mAddInputWinItem.addActionListener(new DisplayWindowEditorPanel_mAddInputWinItem_actionAdapter(this));
      mResolutionPanel.setToolTipText("Set the size of the managed area (display screen resolution)");
      this.add(mMainPanel, BorderLayout.CENTER);
      mMainPanel.add(mMainEditorPanel, BorderLayout.CENTER);
      mResolutionPanel.add(mResolutionLabel, null);
      mResolutionPanel.add(mWidthField, null);
      mResolutionPanel.add(mResolutionXLabel, null);
      mResolutionPanel.add(mHeightField, null);
      mMainEditorPanel.add(mEditorPanel, BorderLayout.CENTER);
      mEditorPanel.add(mButtonPanel, BorderLayout.WEST);
      mEditorPanel.add(mDesktopEditor, BorderLayout.CENTER);
      mMainEditorPanel.add(mResolutionPanel, BorderLayout.NORTH);
      mMainPanel.add(mExtraEditorPanel, BorderLayout.SOUTH);
      mButtonPanel.add(mDisplayCreateButton,
                       new TableLayoutConstraints(1, 1, 1, 1,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mDisplayCreateLabel,
                       new TableLayoutConstraints(1, 2, 1, 2,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mSurfaceViewportButton,
                       new TableLayoutConstraints(1, 4, 1, 4,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mSurfaceViewportLabel,
                       new TableLayoutConstraints(1, 5, 1, 5,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mSimViewportButton,
                       new TableLayoutConstraints(1, 7, 1, 7,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mSimViewportLabel,
                       new TableLayoutConstraints(1, 8, 1, 8,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mInputWinCreateButton,
                       new TableLayoutConstraints(1, 10, 1, 10,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mInputWinCreateLabel,
                       new TableLayoutConstraints(1, 11, 1, 11,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mContextMenu.add(mAddDisplayItem);
      mContextMenu.add(mAddInputWinItem);
   }

   private void setActiveJugglerWindow(JugglerWindowFrame w)
   {
      mActiveWindow = w;
      boolean enabled = (w != null && w instanceof DisplayWindowFrame);
      mSimViewportButton.setEnabled(enabled);
      mSurfaceViewportButton.setEnabled(enabled);
   }

   private Dimension getResolution()
   {
      return mCurrentResolution;
   }

   private Container getOwner()
   {
      return (Container) SwingUtilities.getAncestorOfClass(Container.class,
                                                           this);
   }

   void createDisplayWindowClicked(ActionEvent e)
   {
      DisplayWindowStartDialog dlg =
         new DisplayWindowStartDialog(getOwner(), mContext,
                                      mCurrentResolution);

      if ( dlg.showDialog() == DisplayWindowStartDialog.OK_OPTION )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition window_def =
            broker.getRepository().get(DISPLAY_WINDOW_TYPE);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());

         // Validate the name chosen by the user to ensure that we do not
         // create conflicting config elements.
         String title = checkElementName(dlg.getDisplayWindowTitle());

         // Create a new config element based on the initial information we
         // have.
         ConfigElement elt = factory.create(title, window_def);

         // We do not use the undoable form of ConfigElement.setProperty()
         // here because elt is a freshly created config element.  Hence, the
         // act of creating the config element is what needs to be undone.
         Rectangle bounds  = dlg.getDisplayWindowBounds();
         elt.setProperty(ORIGIN_PROPERTY, 0, new Integer(bounds.x));
         elt.setProperty(ORIGIN_PROPERTY, 1, new Integer(bounds.y));
         elt.setProperty(SIZE_PROPERTY, 0, new Integer(bounds.width));
         elt.setProperty(SIZE_PROPERTY, 1, new Integer(bounds.height));

         ConfigDefinition fb_def =
            broker.getRepository().get(OPENGL_FRAME_BUFFER_TYPE);
         ConfigElement fb_cfg =
            factory.create("OpenGL Frame Buffer Configuration", fb_def);
         fb_cfg.setProperty(VISUAL_ID_PROPERTY, 0, dlg.getVisualID());
         fb_cfg.setProperty(COLOR_RED_SIZE_PROPERTY, 0,
                            dlg.getColorRedDepth());
         fb_cfg.setProperty(COLOR_GREEN_SIZE_PROPERTY, 0,
                            dlg.getColorGreenDepth());
         fb_cfg.setProperty(COLOR_BLUE_SIZE_PROPERTY, 0,
                            dlg.getColorBlueDepth());
         fb_cfg.setProperty(COLOR_ALPHA_SIZE_PROPERTY, 0,
                            dlg.getColorAlphaDepth());
         fb_cfg.setProperty(AUX_BUFFER_COUNT_PROPERTY, 0,
                            dlg.getAuxiliaryBufferCount());
         fb_cfg.setProperty(DEPTH_BUFFER_SIZE_PROPERTY, 0,
                            dlg.getDepthBufferSize());
         fb_cfg.setProperty(STENCIL_BUFFER_SIZE_PROPERTY, 0,
                            dlg.getStencilBufferSize());
         fb_cfg.setProperty(ACCUM_RED_SIZE_PROPERTY, 0,
                            dlg.getAccumRedDepth());
         fb_cfg.setProperty(ACCUM_GREEN_SIZE_PROPERTY, 0,
                            dlg.getAccumGreenDepth());
         fb_cfg.setProperty(ACCUM_BLUE_SIZE_PROPERTY, 0,
                            dlg.getAccumBlueDepth());
         fb_cfg.setProperty(ACCUM_ALPHA_SIZE_PROPERTY, 0,
                            dlg.getAccumAlphaDepth());
         fb_cfg.setProperty(FSAA_ENABLE_PROPERTY, 0, dlg.getFSAAEnable());
         elt.setProperty(FRAME_BUFFER_PROPERTY, 0, fb_cfg);

         elt.setProperty(STEREO_PROPERTY, 0, dlg.inStereo());
         elt.setProperty(BORDER_PROPERTY, 0, dlg.hasBorder());

         elt.setProperty(KEYBOARD_MOUSE_PTR_PROPERTY, 0,
                         dlg.getKeyboardMousePointer());
         elt.setProperty(LOCK_KEY_PROPERTY, 0, dlg.getLockKey());
         elt.setProperty(START_LOCKED_PROPERTY, 0, dlg.shouldStartLocked());
         elt.setProperty(SLEEP_TIME_PROPERTY, 0, dlg.getSleepTime());

         broker.add(mContext, elt, dlg.getResource());
         addDisplayWindow(elt);
      }
   }

   private void addDisplayWindow(ConfigElement elt)
   {
      Dimension desktop_size = mDesktopEditor.getSize();

      if ( desktop_size.width == 0 || desktop_size.height == 0 )
      {
         desktop_size = mDesktopEditor.getPreferredSize();
      }

      JInternalFrame new_frame = new DisplayWindowFrame(elt.getName(),
                                                        getResolution(),
                                                        desktop_size, mContext,
                                                        elt, mSimEditor);

      // Attempt to set an icon for the newly created frame.
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         ImageIcon icon =
            new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/display_window/images/vrjuggler.gif"));

         new_frame.setFrameIcon(icon);
      }
      catch(NullPointerException ex)
      {
         /* Ignore the exception. */ ;
      }

      // XXX: How do we handle inactive windows?  Perhaps that could be left to
      // the rendering process for each indvidual window.
      new_frame.setVisible(true);
      new_frame.addInternalFrameListener(this);
      mDesktopEditor.add(new_frame);
      mAllWindows.add(new_frame);
   }

   void createInputWindowClicked(ActionEvent e)
   {
      InputWindowStartDialog dlg =
         new InputWindowStartDialog(getOwner(), mContext, mCurrentResolution);

      if ( dlg.showDialog() == InputWindowStartDialog.OK_OPTION )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition window_def =
            broker.getRepository().get(INPUT_WINDOW_TYPE);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());

         // Validate the name chosen by the user to ensure that we do not
         // create conflicting config elements.
         String title = checkElementName(dlg.getInputWindowTitle());

         // Create a new config element based on the initial information we
         // have.
         ConfigElement elt = factory.create(title, window_def);

         // We do not use the undoable form of ConfigElement.setProperty()
         // here because elt is a freshly created config element.  Hence, the
         // act of creating the config element is what needs to be undone.
         Rectangle bounds = dlg.getInputWindowBounds();
         elt.setProperty(ORIGIN_PROPERTY, 0, new Integer(bounds.x));
         elt.setProperty(ORIGIN_PROPERTY, 1, new Integer(bounds.y));
         elt.setProperty(SIZE_PROPERTY, 0, new Integer(bounds.width));
         elt.setProperty(SIZE_PROPERTY, 1, new Integer(bounds.height));
         elt.setProperty(KEYBOARD_MOUSE_PTR_PROPERTY, 0,
                         dlg.getKeyboardMousePointer());
         elt.setProperty(LOCK_KEY_PROPERTY, 0, dlg.getLockKey());
         elt.setProperty(START_LOCKED_PROPERTY, 0, dlg.shouldStartLocked());
         elt.setProperty(SLEEP_TIME_PROPERTY, 0, dlg.getSleepTime());

         broker.add(mContext, elt, dlg.getResource());
         addInputWindow(elt);
      }
   }

   private void addInputWindow(ConfigElement elt)
   {
      Dimension desktop_size = mDesktopEditor.getSize();

      if ( desktop_size.width == 0 || desktop_size.height == 0 )
      {
         desktop_size = mDesktopEditor.getPreferredSize();
      }

      JInternalFrame new_frame = new InputWindowFrame(elt.getName(),
                                                      getResolution(),
                                                      desktop_size, mContext,
                                                      elt, mSimEditor);

      // Attempt to set an icon for the newly created frame.
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         ImageIcon icon =
            new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/display_window/images/vrjuggler.gif"));

         new_frame.setFrameIcon(icon);
      }
      catch(NullPointerException ex)
      {
         /* Ignore the exception. */ ;
      }

      new_frame.setVisible(true);
      new_frame.addInternalFrameListener(this);
      mDesktopEditor.add(new_frame);
      mAllWindows.add(new_frame);
   }

   private String checkElementName(String name)
   {
      if ( mContext.containsElementNamed(name) )
      {
         Frame parent =
            (Frame) SwingUtilities.getAncestorOfClass(Frame.class, this);
         String new_name =
            (String) JOptionPane.showInputDialog(
               parent,
               "A config element named '" + name +
                  "' already exists in this context\nPlease enter a new name:",
               "Duplicated Element Name",
               JOptionPane.ERROR_MESSAGE
            );

         return checkElementName(new_name);
      }

      return name;
   }

   void addSurfaceViewportClicked(ActionEvent e)
   {
      SurfaceViewportCreateDialog dlg =
         new SurfaceViewportCreateDialog(getOwner(), mContext);

      if ( dlg.showDialog() == SurfaceViewportCreateDialog.OK_OPTION )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition vp_def =
            broker.getRepository().get(SURFACE_VIEWPORT_TYPE);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());

         ConfigElement display_elt = mActiveWindow.getConfiguration();

         // Set the title for the new viewport based on the number of
         // simulator viewports that already exist in the window's
         // configuration.
         int sim_vps =
            display_elt.getPropertyValueCount(SURFACE_VIEWPORTS_PROPERTY);
         String title = "Surface Viewport " + sim_vps;

         // Create a new config element based on the initial information we
         // have.
         ConfigElement elt = factory.create(title, vp_def);

         Rectangle bounds = dlg.getViewportBounds();
         float origin_x = (float) bounds.x / 100.0f;
         float origin_y = (float) bounds.y / 100.0f;
         float width    = (float) bounds.width / 100.0f;
         float height   = (float) bounds.height / 100.0f;

         // We do not use the undoable form of ConfigElement.setProperty()
         // here because elt is a freshly created config element.  Hence, the
         // act of adding the config element as a property value to display_elt
         // is what needs to be undone.
         elt.setProperty(ORIGIN_PROPERTY, 0, new Float(origin_x));
         elt.setProperty(ORIGIN_PROPERTY, 1, new Float(origin_y));
         elt.setProperty(SIZE_PROPERTY, 0, new Float(width));
         elt.setProperty(SIZE_PROPERTY, 1, new Float(height));
         elt.setProperty(VIEW_PROPERTY, 0, dlg.getViewpoint());
         elt.setProperty(USER_PROPERTY, 0, dlg.getUser());

         Point3D[] corners = dlg.getCorners();
         elt.setProperty(LOWER_LEFT_CORNER_PROPERTY, 0,
                         new Double(corners[0].x));
         elt.setProperty(LOWER_LEFT_CORNER_PROPERTY, 1,
                         new Double(corners[0].y));
         elt.setProperty(LOWER_LEFT_CORNER_PROPERTY, 2,
                         new Double(corners[0].z));
         elt.setProperty(LOWER_RIGHT_CORNER_PROPERTY, 0,
                         new Double(corners[1].x));
         elt.setProperty(LOWER_RIGHT_CORNER_PROPERTY, 1,
                         new Double(corners[1].y));
         elt.setProperty(LOWER_RIGHT_CORNER_PROPERTY, 2,
                         new Double(corners[1].z));
         elt.setProperty(UPPER_RIGHT_CORNER_PROPERTY, 0,
                         new Double(corners[2].x));
         elt.setProperty(UPPER_RIGHT_CORNER_PROPERTY, 1,
                         new Double(corners[2].y));
         elt.setProperty(UPPER_RIGHT_CORNER_PROPERTY, 2,
                         new Double(corners[2].z));
         elt.setProperty(UPPER_LEFT_CORNER_PROPERTY, 0,
                         new Double(corners[3].x));
         elt.setProperty(UPPER_LEFT_CORNER_PROPERTY, 1,
                         new Double(corners[3].y));
         elt.setProperty(UPPER_LEFT_CORNER_PROPERTY, 2,
                         new Double(corners[3].z));

         elt.setProperty(TRACKED_PROPERTY, 0, dlg.isTracked());

         if ( dlg.isTracked() == Boolean.TRUE )
         {
            elt.setProperty(TRACKER_PROXY_PROPERTY, 0, dlg.getTrackerProxy());
         }

         display_elt.addProperty(SURFACE_VIEWPORTS_PROPERTY, elt, mContext);
      }
   }

   void addSimulatorViewportClicked(ActionEvent e)
   {
      SimulatorViewportCreateDialog dlg =
         new SimulatorViewportCreateDialog(getOwner(), mContext);

      if ( dlg.showDialog() == SimulatorViewportCreateDialog.OK_OPTION )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition vp_def =
            broker.getRepository().get(SIMULATOR_VIEWPORT_TYPE);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());

         ConfigElement display_elt = mActiveWindow.getConfiguration();

         // Set the title for the new viewport based on the number of
         // simulator viewports that already exist in the window's
         // configuration.
         int sim_vps =
            display_elt.getPropertyValueCount(SIMULATOR_VIEWPORTS_PROPERTY);
         String title = "Simulator Viewport " + sim_vps;

         // Create a new config element based on the initial information we
         // have.
         ConfigElement elt = factory.create(title, vp_def);

         Rectangle bounds = dlg.getViewportBounds();
         float origin_x = (float) bounds.x / 100.0f;
         float origin_y = (float) bounds.y / 100.0f;
         float width    = (float) bounds.width / 100.0f;
         float height   = (float) bounds.height / 100.0f;

         // We do not use the undoable form of ConfigElement.setProperty()
         // here because elt is a freshly created config element.  Hence, the
         // act of adding the config element as a property value to display_elt
         // is what needs to be undone.
         elt.setProperty(ORIGIN_PROPERTY, 0, new Float(origin_x));
         elt.setProperty(ORIGIN_PROPERTY, 1, new Float(origin_y));
         elt.setProperty(SIZE_PROPERTY, 0, new Float(width));
         elt.setProperty(SIZE_PROPERTY, 1, new Float(height));
         elt.setProperty(VIEW_PROPERTY, 0, dlg.getViewpoint());
         elt.setProperty(USER_PROPERTY, 0, dlg.getUser());
         elt.setProperty(VERTICAL_FOV_PROPERTY, 0, dlg.getVertialFOV());

         ConfigElement sim_elt;
         if ( elt.getPropertyValues(SIMULATOR_PLUGIN_PROPERTY).isEmpty() )
         {
            ConfigDefinition sim_def =
               broker.getRepository().get(DEFAULT_SIMULATOR_TYPE);
            sim_elt = factory.create("Simulator Plug-in", sim_def);
            elt.addProperty(SIMULATOR_PLUGIN_PROPERTY, sim_elt);
         }
         else
         {
            sim_elt =
               (ConfigElement) elt.getProperty(SIMULATOR_PLUGIN_PROPERTY, 0);
         }

         sim_elt.setProperty(CAMERA_POS_PROPERTY, 0, dlg.getCameraPosition());
         sim_elt.setProperty(WAND_POS_PROPERTY, 0, dlg.getWandPosition());

         display_elt.addProperty(SIMULATOR_VIEWPORTS_PROPERTY, elt, mContext);
      }
   }

   private ConfigElement mElement   = null;
   private ConfigContext mContext   = null;
   private boolean       mSimEditor = false;

   private JDesktopPane mDesktopEditor = new JDesktopPane();
   private DesktopBackgroundImage mDesktopBackground = new DesktopBackgroundImage();
   private JugglerWindowFrame mActiveWindow = null;
   private java.util.List mAllWindows = new ArrayList();
   private Dimension mCurrentResolution = new Dimension(1280, 1024);

   private JPopupMenu mContextMenu = new JPopupMenu();

   private BorderLayout mMainLayout = new BorderLayout();
   private JPanel mMainPanel = new JPanel();
   private BorderLayout mMainPanelLayout = new BorderLayout();
   private JPanel mMainEditorPanel = new JPanel();
   private BorderLayout mMainEditorLayout = new BorderLayout();
   private JPanel mResolutionPanel = new JPanel();
   private JPanel mEditorPanel = new JPanel();
   private JPanel mButtonPanel = new JPanel();
   private TableLayout mButtonPanelLayout = null;
   private JButton mDisplayCreateButton = new JButton();
   private JLabel mDisplayCreateLabel = new JLabel();
   private JButton mSurfaceViewportButton = new JButton();
   private JLabel mSurfaceViewportLabel = new JLabel();
   private JButton mSimViewportButton = new JButton();
   private JLabel mSimViewportLabel = new JLabel();
   private JLabel mResolutionLabel = new JLabel();
   private JFormattedTextField mWidthField = new JFormattedTextField();
   private JLabel mResolutionXLabel = new JLabel();
   private JFormattedTextField mHeightField = new JFormattedTextField();
   private JPanel mExtraEditorPanel = new JPanel();
   private BorderLayout mEditorPanelLayout = new BorderLayout();
   private JMenuItem mAddDisplayItem = new JMenuItem();
   private JMenuItem mAddInputWinItem = new JMenuItem();
   private JButton mInputWinCreateButton = new JButton();
   private JLabel mInputWinCreateLabel = new JLabel();

   void resolutionChanged(FocusEvent e)
   {
      // XXX: The size and/or aspect ration of mDesktopEditor should probably
      // be updated too.

      int new_width  = ((Number) mWidthField.getValue()).intValue();
      int new_height = ((Number) mHeightField.getValue()).intValue();

      if ( new_width != mCurrentResolution.width ||
           new_height != mCurrentResolution.height )
      {
         mCurrentResolution.setSize(new_width, new_height);

         for ( Iterator i = mAllWindows.iterator(); i.hasNext(); )
         {
            JugglerWindowFrame frame = (JugglerWindowFrame) i.next();
            frame.updateResolution(mCurrentResolution);
         }
      }
   }

   void desktopEditorResized(ComponentEvent e)
   {
      for ( Iterator i = mAllWindows.iterator(); i.hasNext(); )
      {
         JugglerWindowFrame frame = (JugglerWindowFrame) i.next();
         frame.updateDesktopSize(mDesktopEditor.getSize());
      }
   }

   void mDesktopEditor_mousePressed(MouseEvent e)
   {
      maybePopUp(e);
   }

   void mDesktopEditor_mouseReleased(MouseEvent e)
   {
      maybePopUp(e);
   }

   private void maybePopUp(MouseEvent e)
   {
      if ( e.isPopupTrigger() )
      {
         mContextMenu.show(e.getComponent(), e.getX(), e.getY());
      }
   }

   void mAddDisplayItem_actionPerformed(ActionEvent e)
   {
      createDisplayWindowClicked(e);
   }

   void mAddInputWinItem_actionPerformed(ActionEvent e)
   {
      createInputWindowClicked(e);
   }

   void closeClicked(ActionEvent e)
   {
   }

   void resolutionWidthValueChanged(PropertyChangeEvent e)
   {
      if ( e.getPropertyName().equals("text") )
      {
         System.out.println("Old width text: " + e.getOldValue());
         System.out.println("New width text: " + e.getNewValue());
      }
   }
}

class DisplayWindowEditorPanel_mDisplayCreateButton_actionAdapter
   implements java.awt.event.ActionListener
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mDisplayCreateButton_actionAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.createDisplayWindowClicked(e);
   }
}

class DisplayWindowEditorPanel_mInputWinCreateButton_actionAdapter
   implements java.awt.event.ActionListener
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mInputWinCreateButton_actionAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.createInputWindowClicked(e);
   }
}

class DisplayWindowEditorPanel_Resolution_focusAdapter extends java.awt.event.FocusAdapter
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_Resolution_focusAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void focusLost(FocusEvent e)
   {
      adaptee.resolutionChanged(e);
   }
}

class DisplayWindowEditorPanel_mDesktopEditor_componentAdapter extends java.awt.event.ComponentAdapter
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mDesktopEditor_componentAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void componentResized(ComponentEvent e)
   {
      adaptee.desktopEditorResized(e);
   }
}

class DisplayWindowEditorPanel_mDesktopEditor_mouseAdapter extends java.awt.event.MouseAdapter
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mDesktopEditor_mouseAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void mousePressed(MouseEvent e)
   {
      adaptee.mDesktopEditor_mousePressed(e);
   }
   public void mouseReleased(MouseEvent e)
   {
      adaptee.mDesktopEditor_mouseReleased(e);
   }
}

class DisplayWindowEditorPanel_mAddDisplayItem_actionAdapter implements java.awt.event.ActionListener
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mAddDisplayItem_actionAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.mAddDisplayItem_actionPerformed(e);
   }
}

class DisplayWindowEditorPanel_mAddInputWinItem_actionAdapter implements java.awt.event.ActionListener
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mAddInputWinItem_actionAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.mAddInputWinItem_actionPerformed(e);
   }
}

class DisplayWindowEditorPanel_mSurfaceViewportButton_actionAdapter implements java.awt.event.ActionListener
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mSurfaceViewportButton_actionAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.addSurfaceViewportClicked(e);
   }
}

class DisplayWindowEditorPanel_mSimViewportButton_actionAdapter implements java.awt.event.ActionListener
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mSimViewportButton_actionAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.addSimulatorViewportClicked(e);
   }
}

class DisplayWindowEditorPanel_mWidthField_propertyChangeAdapter implements java.beans.PropertyChangeListener
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mWidthField_propertyChangeAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void propertyChange(PropertyChangeEvent e)
   {
      adaptee.resolutionWidthValueChanged(e);
   }
}
