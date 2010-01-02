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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors;

import javax.swing.JPanel;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import javax.swing.JLabel;
import java.awt.BorderLayout;
import javax.swing.*;

public class DefaultProxyTypeEditor
   extends JPanel
   implements ProxyTypeEditor
{
   public DefaultProxyTypeEditor()
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

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
   }

   private void jbInit()
      throws Exception
   {
      mInfoLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mInfoLabel.setText("No editable properties");
      this.setLayout(mMainLayout);
      this.add(mInfoLabel, java.awt.BorderLayout.NORTH);
   }

   private JLabel mInfoLabel = new JLabel();
   private BorderLayout mMainLayout = new BorderLayout();
}
