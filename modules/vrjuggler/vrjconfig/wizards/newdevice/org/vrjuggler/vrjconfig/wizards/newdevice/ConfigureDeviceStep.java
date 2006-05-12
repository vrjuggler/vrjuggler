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
package org.vrjuggler.vrjconfig.wizards.newdevice;

import java.util.*;
import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Color;

import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;

/**
 * A pane used to welcome the user to a wizard. This is generally the first
 * pane you wish to show in a wizard.
 */
public class ConfigureDeviceStep
   extends AbstractWizardStep
   implements WizardPane
{
   /**
   * Creates a new ConfigureDeviceStep with no text.
   */
   public ConfigureDeviceStep()
   {
      cwp = null;
   }

   public void onEntered()
   {
      super.onEntered();
   }

   public void onExited()
   {
      if (!mConfigContext.containsElement(mConfigElement))
      {
         ConfigBroker broker = new ConfigBrokerProxy();
         broker.add(mConfigContext, mConfigElement);
      }
   }

   /**
   * Gets the GUI component for this pane.
   */
   public JComponent getGUI()
   {
      if (cwp == null)
      {
         Object object = mWhiteBoard.get("device_definition");
         Object ctx_obj = mWhiteBoard.get("context");

         if (null != object && object instanceof ConfigDefinition &&
             null != ctx_obj && ctx_obj instanceof ConfigContext)
         {
            mConfigContext = (ConfigContext)ctx_obj;

            ConfigDefinition def = (ConfigDefinition)object;
            String token = def.getToken();

            // Create a temporary list of ConfigDefinitions to pass to factory.
            java.util.List def_list = new ArrayList();
            def_list.add(def);
           
            // Initialize a ConfigElementFactory with the needed 
            // ConfigDefinition. And create a new ConfigElement.
            ConfigElementFactory temp_factory = new ConfigElementFactory(def_list);
            mConfigElement = temp_factory.create("New " + token, def);
            
            List list = CustomEditorRegistry.findEditors(token);
           
            Color start_color = new Color(160, 160, 180);
   
            Object color = UIManager.get( "window" );
            if(null != color && color instanceof Color)
            {
               start_color = (Color)color;
            }
            else
            {
               System.out.println("Could not get the desktop color from the  UIManager.");
            }

            //XXX:This will be used after making findEditors -> findEditor
            //if(null != editor)
            if(null == list || list.size() == 0)
            {
               System.out.println("No CustomEditors registered for token: " + token);
               
               JScrollPane scroll_pane = new JScrollPane();
               PropertySheet element_prop_sheet =
                  PropertySheetFactory.instance().makeSheet(mConfigContext, mConfigElement, start_color);
               
               scroll_pane.getViewport().removeAll();
               scroll_pane.getViewport().add(element_prop_sheet, null);
               cwp = scroll_pane;
            }
            else if(null != list && list.size() > 0)
            {
               CustomEditor editor = (CustomEditor)list.get(0);
               cwp = (JComponent)editor.getPanel();
               editor.setConfig(mConfigContext, mConfigElement);
            }
         }
      }
      //cwp.init(mWhiteBoard);
      return cwp;
   }

   public List getRequiredClasses()
   {
      List classes = new ArrayList();
      return classes;
   }

   public List getRequiredResources()
   {
      List res = new ArrayList();
      return res;
   }

   /**
    * The JPanel that makes up this pane's UI.
    */
   private transient JComponent cwp = null;
   private transient ConfigElement mConfigElement = null;
   private transient ConfigContext mConfigContext = null;
}
