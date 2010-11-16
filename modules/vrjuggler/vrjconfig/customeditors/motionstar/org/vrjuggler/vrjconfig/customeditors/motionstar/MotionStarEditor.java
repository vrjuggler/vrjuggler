/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.motionstar;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ActionListener;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.editors.CustomEditor;
import org.vrjuggler.jccl.editors.CustomEditorRegistry;
import org.vrjuggler.vrjconfig.commoneditors.DeviceEditorPanel;


public class MotionStarEditor
   implements CustomEditor
{
   public MotionStarEditor()
   {
      java.net.URL help_url =
         getClass().getClassLoader().getResource("org/vrjuggler/vrjconfig/customeditors/motionstar/data/help.html");

      mMainEditorPanel = new DeviceEditorPanel(help_url,
                                               new MotionStarEditorPanel(),
                                               new Dimension(0, 0),
                                               new Dimension(225, 680),
                                               new Dimension(600, 680),
                                               new Dimension(735, 680));
      CustomEditorRegistry.registerEditor("motion_star", getClass());
   }

   public String getTitle()
   {
      return "MotionStar Wireless Editor";
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

   public ActionListener getHelpActionListener()
   {
      return null;
   }

   public void editorClosing()
   {
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;
   private DeviceEditorPanel mMainEditorPanel = null;
}
