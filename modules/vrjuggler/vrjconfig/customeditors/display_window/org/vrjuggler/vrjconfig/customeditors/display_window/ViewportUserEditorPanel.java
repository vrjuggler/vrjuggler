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

import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;


public class ViewportUserEditorPanel extends JPanel
{
   public ViewportUserEditorPanel(ConfigContext ctx, ConfigElement elt)
   {
      mElt = elt;

      ConfigDefinition vp_def = elt.getDefinition();

      mViewpointEditor =
         new PropertyEditorPanel(ctx, mElt.getProperty("view", 0),
                                 vp_def.getPropertyDefinition("view"),
                                 mElt, 0, Color.white);
      mUserEditor =
         new PropertyEditorPanel(ctx, mElt.getProperty("user", 0),
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
