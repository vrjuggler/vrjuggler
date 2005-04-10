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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph.extras;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.util.Iterator;
import javax.swing.*;

import org.jgraph.JGraph;
import org.jgraph.graph.CellView;
import org.jgraph.graph.CellViewRenderer;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.VertexView;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.commoneditors.devicegraph.*;


public class SimRelativePosVertexView
   extends VertexView
{
   private transient DeviceVertexRenderer renderer =
      new DeviceVertexRenderer();

   public SimRelativePosVertexView()
   {
      super();
   }

   public SimRelativePosVertexView(Object cell)
   {
      super(cell);
   }

   public CellViewRenderer getRenderer()
   {
      return renderer;
   }

   private static class DeviceVertexRenderer
      extends AbstractCustomVertexRenderer
      implements ConfigElementListener
               , EditorConstants
   {
      private static final int UNIT_PORT_COLUMN  = 1;
      private static final int LABEL_COLUMN      = 3;
      private static final int PROXY_PORT_COLUMN = 5;

      private static final int SPAN_START_COLUMN = UNIT_PORT_COLUMN;
      private static final int SPAN_END_COLUMN   = PROXY_PORT_COLUMN;

      private static final int NAME_ROW       = 1;
      private static final int UNIT_ROW       = 2;
      private static final int BASE_FRAME_ROW = 3;
      private static final int RELATIVE_ROW   = 4;

      public DeviceVertexRenderer()
      {
         super();

         double[][] sizes =
            {
               {5, TableLayout.MINIMUM, 5, TableLayout.FILL, 5,
                TableLayout.MINIMUM, 5},
               {5, TableLayout.PREFERRED, TableLayout.PREFERRED,
                TableLayout.PREFERRED, TableLayout.PREFERRED, 5}
            };
         this.setLayout(new TableLayout(sizes));

         nameLabel.setText(" ");

         String font_name = "Dialog";
         int font_size = nameLabel.getFont().getSize();

         mPosUnitLabel.setFont(new Font(font_name, Font.PLAIN, font_size));
         mPosUnitLabel.setBorder(BorderFactory.createLineBorder(Color.black));

         Font proxy_font = new Font(font_name, Font.PLAIN, font_size);

         JLabel base_label = new JLabel("Base Frame Proxy");
         mBaseLabel.setFont(proxy_font);
         mBaseLabel.setBorder(BorderFactory.createLineBorder(Color.black));

         JLabel mRelativeLabel = new JLabel("Relative Proxy");
         mRelativeLabel.setFont(proxy_font);
         mRelativeLabel.setBorder(BorderFactory.createLineBorder(Color.black));

         this.add(nameLabel,
                  new TableLayoutConstraints(SPAN_START_COLUMN, NAME_ROW,
                                             SPAN_END_COLUMN, NAME_ROW,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.CENTER));
         this.add(mPosUnitLabel,
                  new TableLayoutConstraints(LABEL_COLUMN, UNIT_ROW,
                                             LABEL_COLUMN, UNIT_ROW,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.CENTER));
         this.add(mBaseLabel,
                  new TableLayoutConstraints(LABEL_COLUMN, BASE_FRAME_ROW,
                                             LABEL_COLUMN, BASE_FRAME_ROW,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.CENTER));
         this.add(mRelativeLabel,
                  new TableLayoutConstraints(LABEL_COLUMN, RELATIVE_ROW,
                                             LABEL_COLUMN, RELATIVE_ROW,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.CENTER));
      }

      public Component getRendererComponent(JGraph graph, CellView view,
                                            boolean selected, boolean hasFocus,
                                            boolean preview)
      {
         mView = view;

         this.graph    = graph;
         this.selected = selected;
         this.preview  = preview;
         this.hasFocus = hasFocus;

         installAttributes(graph, view.getAllAttributes());

         if ( mDeviceInfo == null )
         {
            try
            {
               DefaultGraphCell cell = (DefaultGraphCell) view.getCell();
               mDeviceInfo = (RelativeDeviceInfo) cell.getUserObject();

               DefaultPort unit_port = null, base_port = null,
                           relative_port = null;

               for ( Iterator c = cell.getChildren().iterator(); c.hasNext(); )
               {
                  Object child = c.next();

                  if ( child instanceof DefaultPort )
                  {
                     DefaultPort port = (DefaultPort) child;

                     if ( port.getUserObject() instanceof UnitInfo )
                     {
                        unit_port = port;
                     }
                     else if ( port.getUserObject() != null )
                     {
                        try
                        {
                           ProxyPointerInfo ptr_info =
                              (ProxyPointerInfo) port.getUserObject();
                           String prop_type =
                              ptr_info.getPointerPropertyDefinition().getToken();

                           if ( prop_type.equals(BASE_FRAME_PROXY_PROPERTY) )
                           {
                              base_port = port;
                           }
                           else if ( prop_type.equals(RELATIVE_PROXY_PROPERTY) )
                           {
                              relative_port = port;
                           }
                        }
                        catch (ClassCastException ex)
                        {
                        }
                     }
                  }
               }

//               if ( unit_port == null || base_port == null ||
//                    relative_port == null )
//               {
//                  throw new 
//               }

               Color label_bg =
                  gradientColor != null ? gradientColor.brighter().brighter()
                                        : this.getBackground().brighter().brighter();
               mPosUnitLabel.setBackground(label_bg);
               mBaseLabel.setBackground(label_bg);
               mRelativeLabel.setBackground(label_bg);

               PortComponent pos_unit_port_widget =
                  makePortComponent(unit_port);
               PortComponent base_port_widget = makePortComponent(base_port);
               PortComponent relative_port_widget =
                  makePortComponent(relative_port);

               this.add(
                  pos_unit_port_widget,
                  new TableLayoutConstraints(UNIT_PORT_COLUMN, UNIT_ROW,
                                             UNIT_PORT_COLUMN, UNIT_ROW,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.CENTER)
               );
               this.add(
                  base_port_widget,
                  new TableLayoutConstraints(PROXY_PORT_COLUMN, BASE_FRAME_ROW,
                                             PROXY_PORT_COLUMN, BASE_FRAME_ROW,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.CENTER)
               );
               this.add(
                  relative_port_widget,
                  new TableLayoutConstraints(PROXY_PORT_COLUMN, RELATIVE_ROW,
                                             PROXY_PORT_COLUMN, RELATIVE_ROW,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.CENTER)
               );

               Dimension name_size     = this.nameLabel.getPreferredSize();
               Dimension unit_size     = mPosUnitLabel.getPreferredSize();
               Dimension port_size     = base_port_widget.getPreferredSize();
               Dimension base_size     = mBaseLabel.getPreferredSize();
               Dimension relative_size = mRelativeLabel.getPreferredSize();

               int row_width = Math.max(unit_size.width,
                                        Math.max(base_size.width,
                                                 relative_size.width));
               row_width += port_size.width;

               int min_width  = Math.max(name_size.width, row_width);
               int min_height = name_size.height + unit_size.height +
                                   port_size.height + base_size.height +
                                   relative_size.height + 10;

               setMinimumSize(new Dimension(min_width, min_height));
               // XXX: Doubling the minimum width is a hack to deal with the
               // default preferred alias row sizes not being wide enough.
               this.setPreferredSize(new Dimension(min_width * 2, min_height));

               this.revalidate();

               // Avoid adding ourselves as a listener multiple times.
               // XXX: How can this happen?  The test for mDeviceInfo being
               // null above should be sufficient to guarantee that this code
               // only gets executed once per instance of this type...
               if ( ! preview )
               {
                  mDeviceInfo.getElement().addConfigElementListener(this);
               }
            }
            catch (Exception ex)
            {
               ex.printStackTrace();
            }
         }

         if ( graph.getEditingCell() != view.getCell() )
         {
            nameLabel.setText(mDeviceInfo.getElement().getName());
         }
         else
         {
            nameLabel.setText(" ");
         }

         return this;
      }

      public void nameChanged(ConfigElementEvent evt)
      {
         nameLabel.setText(mDeviceInfo.getElement().getName());
      }

      public void propertyValueAdded(ConfigElementEvent e)
      {
         /* Nothing to do. */ ;
      }

      public void propertyValueChanged(ConfigElementEvent e)
      {
         /* Nothing to do. */ ;
      }

      public void propertyValueOrderChanged(ConfigElementEvent e)
      {
         /* Nothing to do. */ ;
      }

      public void propertyValueRemoved(ConfigElementEvent e)
      {
         /* Nothing to do. */ ;
      }

      private PortComponent makePortComponent(DefaultPort port)
      {
         PortComponent pc = new PortComponent(this.graph, port);
         pc.setMinimumSize(new Dimension(5, 5));
         pc.setPreferredSize(new Dimension(5, 5));
         pc.setForeground(nameLabel.getForeground());

         return pc;
      }

      private transient CellView           mView       = null;
      private transient RelativeDeviceInfo mDeviceInfo = null;

      private JLabel mPosUnitLabel  = new JLabel("Position Unit");
      private JLabel mBaseLabel     = new JLabel("Base Frame Proxy");
      private JLabel mRelativeLabel = new JLabel("Relative Proxy");
   }
}
