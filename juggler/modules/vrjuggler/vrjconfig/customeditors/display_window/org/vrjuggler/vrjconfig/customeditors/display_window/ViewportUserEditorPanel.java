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

import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JPanel;
import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementListener;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;


public class ViewportUserEditorPanel extends JPanel
{
   public ViewportUserEditorPanel()
   {
      this(null);
   }

   public ViewportUserEditorPanel(ConfigElementListener externalListener)
   {
      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition vp_def = broker.getRepository().get("surface_viewport");
      ConfigElementFactory factory =
         new ConfigElementFactory(broker.getRepository().getAllLatest());
      mElt = factory.create("ViewportUserEditorPanel Junk", vp_def);

      if ( externalListener != null )
      {
         mElt.addConfigElementListener(externalListener);
      }

      mViewpointEditor =
         new PropertyEditorPanel(mElt.getProperty("view", 0),
                                 vp_def.getPropertyDefinition("view"),
                                 mElt, 0, Color.white);
      mUserEditor =
         new PropertyEditorPanel(mElt.getProperty("user", 0),
                                 vp_def.getPropertyDefinition("user"),
                                 mElt, 0, Color.white);

      double[][] layout_size = {{TableLayout.PREFERRED, 5,
                                 TableLayout.PREFERRED},
                                {TableLayout.PREFERRED, 5,
                                 TableLayout.PREFERRED}};
      mLayout = new TableLayout(layout_size);

      try
      {
         jbInit();
      }
      catch(Exception ex)
      {
         ex.printStackTrace();
      }
   }

   public Object getViewpoint()
   {
      return mElt.getProperty("view", 0);
   }

   public Object getUser()
   {
      return mElt.getProperty("user", 0);
   }

   private void jbInit() throws Exception
   {
      this.setLayout(mLayout);
      mViewpointLabel.setLabelFor(mViewpointEditor);
      mViewpointLabel.setText("Viewpoint");
      mUserLabel.setLabelFor(mUserEditor);
      mUserLabel.setText("User");
      this.add(mViewpointLabel, new TableLayoutConstraints(0, 0, 0, 0,
                                                           TableLayout.RIGHT,
                                                           TableLayout.FULL));
      this.add(mViewpointEditor, new TableLayoutConstraints(2, 0, 2, 0,
                                                            TableLayout.FULL,
                                                            TableLayout.FULL));
      this.add(mUserLabel, new TableLayoutConstraints(0, 2, 0, 2,
                                                      TableLayout.RIGHT,
                                                      TableLayout.FULL));
      this.add(mUserEditor, new TableLayoutConstraints(2, 2, 2, 2,
                                                       TableLayout.FULL,
                                                       TableLayout.FULL));
   }

   private ConfigElement mElt = null;

   private TableLayout mLayout = null;
   private JLabel mViewpointLabel = new JLabel();
   private PropertyEditorPanel mViewpointEditor = null;
   private JLabel mUserLabel = new JLabel();
   private PropertyEditorPanel mUserEditor = null;
}
