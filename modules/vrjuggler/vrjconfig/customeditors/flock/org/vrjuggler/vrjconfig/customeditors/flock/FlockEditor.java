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

package org.vrjuggler.vrjconfig.customeditors.flock;

import java.awt.Container;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.editors.CustomEditor;
import org.vrjuggler.jccl.editors.CustomEditorRegistry;


public class FlockEditor
   implements CustomEditor
{
   public FlockEditor()
   {
      CustomEditorRegistry.registerEditor("flock", getClass());
   }

   public String getTitle()
   {
      return "Flock of Birds Editor";
   }

   public Container getPanel()
   {
      return mMainEditorPanel;
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      mContext = ctx;
      mElement = elt;
      mMainEditorPanel.setConfig(ctx, elt);
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;
   private MainEditorPanel mMainEditorPanel = new MainEditorPanel();
}
