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

import java.util.ArrayList;
import java.util.List;
import javax.swing.*;

import org.jgraph.event.GraphSelectionEvent;
import org.jgraph.event.GraphSelectionListener;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.commoneditors.ProxyEditorUI;
import org.vrjuggler.vrjconfig.commoneditors.KeyboardEditorPanel;


public class SimAnalogDeviceEditor
   extends JSplitPane
   implements SimDeviceEditor
            , GraphSelectionListener
            , EditorConstants
{
   public SimAnalogDeviceEditor()
   {
      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
   }

   public void setKeyboardEditorPanel(KeyboardEditorPanel panel)
   {
      if ( panel == null && mKeyboardEditor != null )
      {
         // Unregister our listener for events from mKeyboardEditor ...
      }

      mKeyboardEditor = panel;

      if ( mKeyboardEditor != null )
      {
         // Register our listener for events from mKeyboardEditor ...
      }
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      if ( ! elt.getDefinition().getToken().equals(SIM_ANALOG_DEVICE_TYPE) )
      {
         throw new IllegalArgumentException("Invalid config element type '" +
                                            elt.getDefinition().getToken() +
                                            "'!  Expected " +
                                            SIM_ANALOG_DEVICE_TYPE);
      }

      mContext = ctx;

      if ( mProxyGraph != null )
      {
         this.remove(mProxyGraph);
         mProxyGraph.getDeviceProxyEditor().getGraph().
            removeGraphSelectionListener(this);
         mProxyGraph.editorClosing();
         mProxyGraph = null;
      }

      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository repos = broker.getRepository();

      List allowed_types = new ArrayList(2);
      allowed_types.add(0, repos.get(ANALOG_PROXY_TYPE));
      allowed_types.add(1, repos.get(SIM_ANALOG_DEVICE_TYPE));

      mProxyGraph = new ProxyEditorUI(allowed_types);
      mProxyGraph.getDeviceProxyEditor().getGraph().
         addGraphSelectionListener(this);
      mProxyGraph.setConfig(ctx, elt);
      this.add(mProxyGraph, JSplitPane.LEFT);
   }

   public JComponent getEditor()
   {
      return this;
   }

   public void valueChanged(GraphSelectionEvent e)
   {
   }

   private void jbInit() throws Exception
   {
      this.setOneTouchExpandable(false);
      this.setOrientation(VERTICAL_SPLIT);
      this.setResizeWeight(0.5);
      this.add(mKeyBindingPanel, JSplitPane.RIGHT);
   }

   private ConfigContext mContext = null;

   private KeyboardEditorPanel mKeyboardEditor = null;

   private ProxyEditorUI mProxyGraph = null;
   private JPanel mKeyBindingPanel = new JPanel();
}
