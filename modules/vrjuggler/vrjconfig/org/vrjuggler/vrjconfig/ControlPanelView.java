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

package org.vrjuggler.vrjconfig;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Frame;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesService;
import org.vrjuggler.tweek.services.GlobalPreferencesServiceProxy;
import org.vrjuggler.vrjconfig.ui.*;
import org.vrjuggler.jccl.editors.*;

/**
 * Provides a control panel view into a config chunk collection.
 */
public class ControlPanelView
   extends JPanel
{
   public ControlPanelView()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      try
      {
         GlobalPreferencesService prefs = new GlobalPreferencesServiceProxy();

         // Using the global user preferences from Tweek, set the start
         // directory for fileChooser.
         File f = new File(prefs.getChooserStartDir());
         fileChooser.setCurrentDirectory(f);
      }
      catch(Exception ex)
      {
         System.err.println("ControlPanelView(): WARNING: Failed to set file chooser start directory: " +
                            ex.getMessage());
      }

      // Add forward and back buttons to the toolbar
//      toolbar.addSeparator();
      toolbar.addToToolbar(Box.createHorizontalStrut(8));
      backBtn.setText("Back");
      backBtn.setEnabled(false);
      backBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            backClicked();
         }
      });
      forwardBtn.setText("Forward");
      forwardBtn.setEnabled(false);
      forwardBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            forwClicked();
         }
      });
      toolbar.addToToolbar(backBtn);
      toolbar.addToToolbar(forwardBtn);

      // Make sure the toolbar is using our context whenever we become active
      addComponentListener(new ComponentAdapter()
      {
         public void componentShown(ComponentEvent evt)
         {
            toolbar.setConfigContext(context);
         }
      });

//      SwingUtilities.invokeLater(new Runnable()
//      {
//         public void run()
//         {
//            showMainPanel();
//         }
//      });

//      // Init the watermark in the control panel
//      ClassLoader loader = BeanJarClassLoader.instance();
      showMainPanel();
//
//      control.addActionListener(new ActionListener()
//      {
//         public void actionPerformed(ActionEvent evt)
//         {
//            String item = control.getModel().getLabelAt(evt.getID());
//            if (item.equals("D e v i c e s"))
//            {
//               showDevicesPanel();
//            }
//            else if (item.equals("D i s p l a y"))
//            {
//               ControlPanelView.this.remove(control);
//               ControlPanelView.this.add(new DisplayEditor());
//            }
//            else if (item.equals("A n a l o g"))
//            {
//               ClassLoader loader = BeanJarClassLoader.instance();
//               showPanelWithChunks("simulated_analog_device",
//                                   new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/analog_devices64.png")),
//                                   "Choose an analog device");
//            }
//            else if (item.equals("D i g i t a l"))
//            {
//               ClassLoader loader = BeanJarClassLoader.instance();
//               showPanelWithChunks("simulated_digital_device",
//                                   new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/digital_devices64.png")),
//                                   "Choose a digital device");
//            }
//            else if (item.equals("P o s i t i o n a l"))
//            {
//               ClassLoader loader = BeanJarClassLoader.instance();
//               showPanelWithChunks("simulated_positional_device",
//                                   new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/positional_devices64.png")),
//                                   "Choose a positional device");
//
////               JDialog dlg = new JDialog(
////                     (Frame)SwingUtilities.getAncestorOfClass(Frame.class, ControlPanelView.this),
////                     "Simulator Positional Device Editor",
////                     true);
////               dlg.getContentPane().setLayout(new BorderLayout());
////               SimPosDeviceEditor editor = new SimPosDeviceEditor();
////
////               ConfigManagerService mgr = getConfigManager();
////               ConfigChunk device = mgr.getActiveConfig().get("SimHeadPos");
////               editor.setDevice(device);
////               dlg.getContentPane().add(editor, BorderLayout.CENTER);
////               dlg.pack();
////               dlg.setVisible(true);
//            }
//            else if (item.equals("C l u s t e r"))
//            {
//               JDialog dlg = new JDialog(
//                     (Frame)SwingUtilities.getAncestorOfClass(Frame.class, ControlPanelView.this),
//                     "Sim Device Editor",
//                     true);
//               dlg.getContentPane().setLayout(new BorderLayout());
//               SimAnalogDeviceEditor editor = new SimAnalogDeviceEditor();
//
//               ConfigManagerService mgr = getConfigManager();
//               ConfigChunk device = mgr.getActiveConfig().get("Analog Simulator");
//               editor.setDevice(device);
//               dlg.getContentPane().add(editor, BorderLayout.CENTER);
//               dlg.pack();
//               dlg.setVisible(true);
//            }
////            else if (item.equals("Simulator"))
////            {
////               showSimEditor();
////            }
//            else
//            {
//               JOptionPane.showMessageDialog(ControlPanelView.this, "Selected "+String.valueOf(value));
//            }
//         }
//      });
   }

//   protected void showSimEditor()
//   {
//      JDialog dlg = new JDialog(
//            (Frame)SwingUtilities.getAncestorOfClass(Frame.class, this),
//            "Simulator Editor",
//            true);
//      dlg.getContentPane().setLayout(new BorderLayout());
//      dlg.getContentPane().add(new SimEditor(), BorderLayout.CENTER);
//      dlg.pack();
//      dlg.setVisible(true);
//   }

   /**
    * Helper class for getting the config manager serivce.
    */
   private ConfigBroker getConfigBroker()
   {
      return new ConfigBrokerProxy();
   }

   private void showMainPanel()
   {
      ClassLoader loader = BeanJarClassLoader.instance();
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      model.add("D i s p l a y",       new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/display64.png")));
      model.add("S i m u l a t o r",   new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/simulator64.png")));
      model.add("C l u s t e r",       new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/cluster64.png")));
      model.add("D e v i c e s",       new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/devices64.png")));
      model.add("A u d i o",           new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/audio64.png")));
      ControlPanel new_control = new ControlPanel();
      new_control.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            ControlPanel control = (ControlPanel)evt.getSource();
            String item = control.getModel().getLabelAt(evt.getID());
            if (item.equals("D e v i c e s"))
            {
               showDevicesPanel();
            }
            else if (item.equals("D i s p l a y"))
            {
               //CustomEditor editor = CustomEditorRegistry.findEditor("display_window");

               //editor.setContext(context);
               //pushCurrentBack(editor.getPanel());
            }
            else if (item.equals("S i m u l a t o r"))
            {
               SimDisplayEditor display_editor = new SimDisplayEditor();
               //display_editor.setContext(context);
               pushCurrentBack(display_editor);
            }
            else if (item.equals("C l u s t e r"))
            {
               JDialog dlg = new JDialog(
                     (Frame)SwingUtilities.getAncestorOfClass(Frame.class, ControlPanelView.this),
                     "Sim Device Editor",
                     true);
               dlg.getContentPane().setLayout(new BorderLayout());
               SimAnalogDeviceEditor editor = new SimAnalogDeviceEditor(context);

               List chunks = getConfigBroker().getElements(context);
               ConfigElement device = (ConfigElement)ConfigUtilities.getElementsWithName(chunks,
                                                            "Analog Simulator").get(0);
               editor.setDevice(device);
               dlg.getContentPane().add(editor, BorderLayout.CENTER);
               dlg.pack();
               dlg.setVisible(true);
            }
         }
      });
      new_control.setModel(model);
      new_control.setTitle("Choose a category");
      new_control.setWatermark(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/watermark_logo.png")));
      pushCurrentBack(new_control);
   }

   private void showDevicesPanel()
   {
      ClassLoader loader = BeanJarClassLoader.instance();
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      model.add("A n a l o g",            new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/analog_devices64.png")));
      model.add("D i g i t a l",          new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/digital_devices64.png")));
      model.add("P o s i t i o n a l",    new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/positional_devices64.png")));
      model.add("G l o v e",              new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/glove_devices64.png")));
      ControlPanel new_control = new ControlPanel();
      new_control.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            ControlPanel control = (ControlPanel)evt.getSource();
            String item = control.getModel().getLabelAt(evt.getID());
            if (item.equals("A n a l o g"))
            {
               ClassLoader loader = BeanJarClassLoader.instance();
               ControlPanel ctl = createPanelWithChunks(
                                       "simulated_analog_device",
                                       new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/analog_devices64.png")));
               ctl.setTitle("Choose an analog device");
               ctl.addActionListener(new ActionListener()
               {
                  public void actionPerformed(ActionEvent evt)
                  {
                     JDialog dlg = new JDialog(
                           (Frame)SwingUtilities.getAncestorOfClass(Frame.class, ControlPanelView.this),
                           "Analog Device Editor",
                           true);
                     dlg.getContentPane().setLayout(new BorderLayout());
                     SimAnalogDeviceEditor editor = new SimAnalogDeviceEditor(context);

                     List chunks = getConfigBroker().getElements(context);
                     String device_name = ((ControlPanel)evt.getSource()).getModel().getLabelAt(evt.getID());
                     ConfigElement device = (ConfigElement)ConfigUtilities.getElementsWithName(chunks, device_name).get(0);
                     editor.setDevice(device);
                     dlg.getContentPane().add(editor, BorderLayout.CENTER);
                     dlg.pack();
                     dlg.setVisible(true);
                  }
               });
               pushCurrentBack(ctl);
            }
            else if (item.equals("D i g i t a l"))
            {
               ClassLoader loader = BeanJarClassLoader.instance();
               ControlPanel ctl = createPanelWithChunks(
                                       "simulated_digital_device",
                                       new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/digital_devices64.png")));
               ctl.setTitle("Choose a digital device");
               pushCurrentBack(ctl);
            }
            else if (item.equals("P o s i t i o n a l"))
            {
               /*
               List list = CustomEditorRegistry.findEditors("intersense");
               CustomEditor editor = (CustomEditor)list.get(0);

               //editor.setContext(context);
               pushCurrentBack(editor.getPanel());
               
               */
               ClassLoader loader = BeanJarClassLoader.instance();
               ControlPanel ctl = createPanelWithElementsOfType("positional_device");
               ctl.setTitle("Choose a positional device");
               ctl.addActionListener(new ActionListener()
               {
                  public void actionPerformed(ActionEvent evt)
                  {
                     JDialog dlg = new JDialog(
                           (Frame)SwingUtilities.getAncestorOfClass(Frame.class, ControlPanelView.this),
                           "Positional Device Editor",
                           true);
                     /*
                     dlg.getContentPane().setLayout(new BorderLayout());
                     SimPosDeviceEditor editor = new SimPosDeviceEditor(context);

                     List chunks = getConfigBroker().getElements(context);
                     System.out.println("Action Cmd: "+evt.getActionCommand());

                     String device_name = ((ControlPanel)evt.getSource()).getModel().getLabelAt(evt.getID());
                     ConfigElement device = (ConfigElement)ConfigUtilities.getElementsWithName(chunks, device_name).get(0);
                     editor.setDevice(device);
                     */
                     
                     Object object = ((ControlPanel)evt.getSource()).getModel().getUserObjectAt(evt.getID());
                     if(null != object && object instanceof ConfigElement)
                     {
                        ConfigElement element = (ConfigElement)object;
                        String token = element.getDefinition().getToken();
                        
                        List list = CustomEditorRegistry.findEditors(token);
                        
                        //XXX:This will be used after making findEditors -> findEditor
                        //if(null != editor)
                        if(null != list && list.size() > 0)
                        {
                           CustomEditor editor = (CustomEditor)list.get(0);
                           dlg.getContentPane().add(editor.getPanel(), BorderLayout.CENTER);
                           
                           // Make sure to set the context first.
                           editor.setContext(context);
                           editor.setConfigElement(element);
                           dlg.setTitle(editor.getTitle());
                           dlg.pack();
                           dlg.setVisible(true);
                        }
                     }


                  }
               });
               pushCurrentBack(ctl);
            }
            else if (item.equals("G l o v e"))
            {
               ClassLoader loader = BeanJarClassLoader.instance();
               ControlPanel ctl = createPanelWithChunks(
                                       "SimGlove",
                                       new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/glove_devices64.png")));
               ctl.setTitle("Choose a glove device");
               pushCurrentBack(ctl);
            }
         }
      });
      new_control.setModel(model);
      new_control.setTitle("Choose a device type");
      new_control.setWatermark(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/watermark_logo.png")));
      pushCurrentBack(new_control);
   }

   /**
    * Displays a panel containing all of the chunks that have the given token.
    */
   private ControlPanel createPanelWithChunks(String token, Icon icon)
   {
      ClassLoader loader = BeanJarClassLoader.instance();
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      /*
      List devices = ConfigUtilities.getElementsWithDefinition(
                           getConfigBroker().getElements(context), token);
      */
      List devices = getElementsWithDefinitionAndDerived(token);
      for (Iterator itr = devices.iterator(); itr.hasNext(); )
      {
         ConfigElement element = (ConfigElement)itr.next();
         String icon_location = element.getDefinition().getIconLocation();
         icon_location = (new EnvironmentServiceProxy()).expandEnvVars(icon_location);
         System.out.println("icon_location:" + icon_location);
         try
         {
            java.net.URL url = new java.net.URL(icon_location);
            Icon device_icon = new ImageIcon(url);
            model.add(element.getName(), device_icon);
         }
         catch(Exception ex)
         {
            ex.printStackTrace();
         }
      }
      ControlPanel new_control = new ControlPanel();
      new_control.setModel(model);
      new_control.setWatermark(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/watermark_logo.png")));

      return new_control;
   }

   /**
    * Displays a panel containing all of the chunks that have the given token.
    */
   private ControlPanel createPanelWithElementsOfType(String token)
   {
      ClassLoader loader = BeanJarClassLoader.instance();
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      /*
      List devices = ConfigUtilities.getElementsWithDefinition(
                           getConfigBroker().getElements(context), token);
      */
      List devices = getElementsWithDefinitionAndDerived(token);
      for (Iterator itr = devices.iterator(); itr.hasNext(); )
      {
         ConfigElement element = (ConfigElement)itr.next();
         String icon_location = element.getDefinition().getIconLocation();
         icon_location = (new EnvironmentServiceProxy()).expandEnvVars(icon_location);
         System.out.println("icon_location:" + icon_location);
         try
         {
            java.net.URL url = new java.net.URL(icon_location);
            Icon device_icon = new ImageIcon(url);
            model.add(element.getName(), device_icon, element);
         }
         catch(Exception ex)
         {
            ex.printStackTrace();
         }
      }
      ControlPanel new_control = new ControlPanel();
      new_control.setModel(model);
      new_control.setWatermark(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/watermark_logo.png")));

      return new_control;
   }

   private List getElementsWithDefinitionAndDerived(String token)
   {
      List result = new ArrayList();
      List elts = getConfigBroker().getElements(context);
      ConfigDefinitionRepository repos = getConfigBroker().getRepository();
      List sub_defs = repos.getSubDefinitions(token); 
      for(Iterator itr = sub_defs.iterator() ; itr.hasNext() ; )
      {
         List elements = ConfigUtilities.getElementsWithDefinition(elts, (String)itr.next());
         result.addAll(elements);
      }
      return result;
   }

   /**
    * Handles the case when the backward navigation button is clicked.
    */
   private void backClicked()
   {
      if (forwStack.empty())
      {
         forwardBtn.setEnabled(true);
      }

      // Change the current component to the one at the top of the back stack.
      swapCurComponent(backStack, forwStack);

      if (backStack.empty())
      {
         backBtn.setEnabled(false);
      }
   }

   /**
    * Handles the case when the forward navigation button is clicked.
    */
   private void forwClicked()
   {
      if (backStack.empty())
      {
         backBtn.setEnabled(true);
      }

      // Change the current component to the next panel in the forward stack.
      swapCurComponent(forwStack, backStack);

      if (forwStack.empty())
      {
         forwardBtn.setEnabled(false);
      }
   }

   /**
    * Moves the current component to the top of toStack and sets the current
    * component to the top of fromStack.
    */
   private void swapCurComponent(Stack fromStack, Stack toStack)
   {
      toStack.push(currentView);

      this.remove(currentView);
      currentView = (Component)fromStack.pop();
      this.add(currentView, BorderLayout.CENTER);
      this.repaint();
   }

   /**
    * Pushes the current component to the back stack and makes the given
    * component the new current component.
    */
   private void pushCurrentBack(Component newComp)
   {
      if (backStack.empty())
      {
         backBtn.setEnabled(true);
      }

      backStack.push(currentView);

      // Clear out the forward navigation stack because what we are now viewing
      // is what will go into the forward stack if the back button is pressed.
      forwStack.clear();
      forwardBtn.setEnabled(false);

      this.remove(currentView);
      currentView = newComp;
      this.add(currentView, BorderLayout.CENTER);
      updateUI();
      newComp.invalidate();
   }

   /**
    * Responding to notification that from the toolbar that a configuration has
    * been opened or a new configuration has been created, this method creates
    * a new main panel to hold that configuration.
    */
   private void toolbarContextChanged()
   {
      this.context = toolbar.getConfigContext();
      showMainPanel();
   }

   /**
    * Autogenerated code for the JBuilder GUI.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(baseLayout);
//      control.setModel(model);
      toolbar.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            String cmd = evt.getActionCommand();
            if (cmd.equals("New") || cmd.equals("Open"))
            {
               toolbarContextChanged();
            }
         }
      });
      this.add(toolbar,  BorderLayout.NORTH);
//      this.add(control,  BorderLayout.CENTER);
   }

   //--- JBuilder GUI variables ---//
   private BorderLayout baseLayout = new BorderLayout();
   private ConfigToolbar toolbar = new ConfigToolbar();
//   private ControlPanel control = new ControlPanel();
   private JFileChooser fileChooser = new JFileChooser();

   /**
    * The data model for the control panel.
    */
//   private DefaultControlPanelModel model = new DefaultControlPanelModel();

   /**
    * The context providing a view into the current configuration.
    */
   private ConfigContext context = new ConfigContext();

   /**
    * The currently viewed component.
    */
   private Component currentView = new ControlPanel();

   private JButton backBtn = new JButton();
   private JButton forwardBtn = new JButton();

   private Stack backStack = new Stack();
   private Stack forwStack = new Stack();
}
