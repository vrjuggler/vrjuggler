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
package org.vrjuggler.jccl.editors;

import java.awt.*;
import javax.swing.*;
import org.vrjuggler.jccl.config.ConfigContext;

/**
 * A generic editor for a configuration. It contains editing components for both
 * the configuration descriptions and the configuration elements themselves.
 */
public class GenericConfigEditor
   extends JPanel
{
   public GenericConfigEditor()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Set ourselves up with a default context
      setConfigContext(new ConfigContext());
   }

   /**
    * Set the ConfigContext to use for this ConfigEditor.
    */
   public void setConfigContext(ConfigContext context)
   {
      mContextEditor.setContext(context);
   }

   /**
    * Get a handle to the ConfigContext used in this ConfigEditor.
    */
   public ConfigContext getConfigContext()
   {
      return mContextEditor.getContext();
   }

   /**
    * Get a handle to the ConfigContextEditor for this ConfigEditor.
    */
   public ConfigContextEditor getContextEditor()
   {
      return mContextEditor;
   }

   private void jbInit()
      throws Exception
   {
      this.setLayout(mBaseLayout);
      this.add(mContextEditor, BorderLayout.CENTER);
   }

   private BorderLayout mBaseLayout = new BorderLayout();
   private ConfigContextEditor mContextEditor = new ConfigContextEditor();
}
