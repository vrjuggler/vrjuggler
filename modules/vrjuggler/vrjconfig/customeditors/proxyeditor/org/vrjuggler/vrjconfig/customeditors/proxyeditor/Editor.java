/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

package org.vrjuggler.vrjconfig.customeditors.proxyeditor;

import java.util.Iterator;
import java.util.List;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.CustomEditor;
import org.vrjuggler.jccl.editors.CustomEditorRegistry;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.commoneditors.ProxyEditorUI;


public class Editor
   implements CustomEditor
{
   public Editor()
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository repos = broker.getRepository();
      List all_defs = repos.getAllLatest();

      List proxy_defs =
         ConfigUtilities.getDefinitionsOfType(all_defs,
                                              EditorConstants.PROXY_TYPE);

      for ( Iterator d = proxy_defs.iterator(); d.hasNext(); )
      {
         ConfigDefinition def = (ConfigDefinition) d.next();

         if ( ! def.isAbstract() )
         {
            CustomEditorRegistry.registerEditor(def.getToken(), getClass());
         }
      }
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      mProxyEditor.setConfig(ctx, elt);
   }

   public java.awt.Container getPanel()
   {
      return mProxyEditor;
   }

   public String getTitle()
   {
      return "Proxy Editor";
   }

   public java.awt.event.ActionListener getHelpActionListener()
   {
      return null;
   }

   /**
    * Clean up after ourself.  In particular, this is intended to break
    * circular object references.
    */
   public void editorClosing()
   {
      mProxyEditor.editorClosing();
      mProxyEditor = null;
   }

   private ProxyEditorUI mProxyEditor = new ProxyEditorUI();
}
