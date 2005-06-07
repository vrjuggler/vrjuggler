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

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Window;
import java.util.Iterator;
import java.util.List;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import org.vrjuggler.jccl.config.*;


public class PositionProxyEditor
   extends JPanel
   implements ProxyTypeEditor
            , EditorConstants
{
   public PositionProxyEditor()
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
      if ( ! elt.getDefinition().getToken().equals(POSITION_PROXY_TYPE) )
      {
         throw new IllegalArgumentException("Config element of type '" +
                                            elt.getDefinition().getToken() +
                                            "' is not allowed");
      }

      List filters = elt.getPropertyValues(POSITION_FILTERS_PROPERTY);

      if ( filters == null || filters.size() == 0 )
      {
         // Pop up a warning message saying that the config element is
         // missing information.
         Window parent =
            (Window) SwingUtilities.getAncestorOfClass(Window.class, this);
         JOptionPane.showMessageDialog(parent,
                                       "WARNING: Config element '" +
                                          elt.getName() +
                                          "' has no position filters!\n" +
                                          "Will create default position " +
                                          "transform filter ...",
                                       "Correcting Incomplete Configuration Element",
                                       JOptionPane.WARNING_MESSAGE);

         // Create the new position_transform_filter config element.
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition filter_def =
            broker.getRepository().get(POSITION_TRANSFORM_FILTER_TYPE);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());
         ConfigElement xform_filter = factory.create("Transform Filter 0",
                                                     filter_def);

         // Add the new config element to the position_filters property.
         elt.addProperty(POSITION_FILTERS_PROPERTY, xform_filter, ctx);
         mRotationPanel.setConfig(ctx, xform_filter);
         mTranslationPanel.setConfig(ctx, xform_filter);
      }
      else
      {
         String type = POSITION_TRANSFORM_FILTER_TYPE;

         // Pass off the embedded position_transform_filter config element.
         // For now, we assume that there will be at most one such element.
         for ( Iterator i = filters.iterator(); i.hasNext(); )
         {
            ConfigElement filter = (ConfigElement) i.next();

            if ( filter.getDefinition().getToken().equals(type) )
            {
               mRotationPanel.setConfig(ctx, filter);
               mTranslationPanel.setConfig(ctx, filter);
               break;
            }
         }
      }
   }

   private void jbInit()
      throws Exception
   {
      this.setMinimumSize(new Dimension(275, 150));
      this.setPreferredSize(new Dimension(500, 150));
      this.setLayout(gridLayout1);
      gridLayout1.setColumns(1);
      gridLayout1.setRows(0);
      mRotationPanel.setBorder(mRotationPanelTitle);
      mRotationPanelTitle.setTitle("Sensor Orientation");
      mTranslationPanel.setBorder(mTranslationPanelTitle);
      mTranslationPanelTitle.setTitle("Sensor Position");
      this.add(mRotationPanel);
      this.add(mTranslationPanel);
   }

   private CoordinateFrameEditor mRotationPanel =
      new CoordinateFrameEditor(CoordinateFrameEditor.SENSOR);
   private TransformTranslationEditor mTranslationPanel =
      new TransformTranslationEditor(TransformTranslationEditor.SENSOR);
   private GridLayout gridLayout1 = new GridLayout();
   private TitledBorder mRotationPanelTitle = new TitledBorder("");
   private TitledBorder mTranslationPanelTitle = new TitledBorder("");
}
