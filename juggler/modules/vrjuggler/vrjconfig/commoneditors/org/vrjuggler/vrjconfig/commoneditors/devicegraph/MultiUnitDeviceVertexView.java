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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.*;
import javax.swing.*;

import org.jgraph.JGraph;
import org.jgraph.graph.*;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;


/**
 * A custom vertex view for graph cells representing device config elements
 * that have multiple input sources (which are in turn represented with
 * JGraph ports).  This view makes use of heavy-weight renderers so that users
 * may interact with components in the vertex view.
 */
public class MultiUnitDeviceVertexView
   extends VertexView
{
   private transient DeviceVertexRenderer renderer =
      new DeviceVertexRenderer();

   public MultiUnitDeviceVertexView()
   {
      super();
   }

   public MultiUnitDeviceVertexView(Object cell)
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
   {
      private static final int PORT_START_COLUMN    = 1;
      private static final int PORT_END_COLUMN      = 1;
      private static final int LABEL_START_COLUMN   = 3;
      private static final int LABEL_END_COLUMN     = 3;
      private static final int BUTTON0_START_COLUMN = 5;
      private static final int BUTTON0_END_COLUMN   = 5;

      private static final int SPAN_START_COLUMN = PORT_START_COLUMN;
      private static final int SPAN_END_COLUMN   = BUTTON0_END_COLUMN;

      private static final int BUTTON_MIN_WIDTH   = 30;
      private static final int BUTTON_MIN_HEIGHT  = 16;
      private static final int BUTTON_PREF_WIDTH  = 30;
      private static final int BUTTON_PREF_HEIGHT = 16;

      private static final int NAME_ROW       = 1;
      private static final int ADD_BUTTON_ROW = 2;

      private static String IMG_ROOT = EditorConstants.COMMON_IMG_ROOT;

      public DeviceVertexRenderer()
      {
         super();

         double[][] sizes =
            {
               {5, TableLayout.MINIMUM, 5, TableLayout.FILL, 5,
                TableLayout.MINIMUM, 5},
               {5, TableLayout.PREFERRED, TableLayout.MINIMUM}
            };
         mMainLayout = new TableLayout(sizes);
         this.setLayout(mMainLayout);

         nameLabel.setText(" ");

         mAddUnitButton.setEnabled(false);
         mAddUnitButton.setToolTipText(
            "Add a new input source for this device"
         );
         mAddUnitButton.setText("Create Input Source");
         mAddUnitButton.setMargin(new Insets(3, 0, 0, 0));
         mAddUnitButton.setBorderPainted(false);
         mAddUnitButton.setFont(new Font("Dialog", Font.PLAIN, 10));
         mAddUnitButton.setContentAreaFilled(false);

         try
         {
            ClassLoader loader = getClass().getClassLoader();
            mAddUnitButton.setIcon(
               new ImageIcon(loader.getResource(IMG_ROOT + "/New16.gif"))
            );
         }
         catch (Exception ex)
         {
         }

         mAddUnitButton.addActionListener(
            new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  // This adds a new property value to our config element
                  // which results in propertyValueAdded() being invoked.
                  // It is there that we add the new row to the layout.
                  addUnit();
               }
            }
         );

         this.add(nameLabel,
                  new TableLayoutConstraints(SPAN_START_COLUMN, NAME_ROW,
                                             SPAN_END_COLUMN, NAME_ROW,
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

         Map attributes = view.getAllAttributes();
         installAttributes(graph, attributes);

         if ( mDeviceInfo == null )
         {
            try
            {
               DefaultGraphCell cell = (DefaultGraphCell) view.getCell();
               mDeviceInfo = (DeviceInfo) cell.getUserObject();

               mVariableUnits = mDeviceInfo.hasVariableUnitCount();

               if ( mVariableUnits )
               {
                  mUnitStartRow = ADD_BUTTON_ROW + 1;
                  mAddUnitButton.setEnabled(true);
                  this.add(mAddUnitButton,
                           new TableLayoutConstraints(SPAN_START_COLUMN,
                                                      ADD_BUTTON_ROW,
                                                      SPAN_END_COLUMN,
                                                      ADD_BUTTON_ROW,
                                                      TableLayoutConstraints.CENTER,
                                                      TableLayoutConstraints.CENTER));
               }
               else
               {
                  mUnitStartRow = ADD_BUTTON_ROW;
               }

               Dimension label_size = nameLabel.getPreferredSize();
               Dimension add_size   = mAddUnitButton.getPreferredSize();

               int min_width = Math.max(label_size.width, add_size.width);

               // The extra 10 units of height is to allow some vertical
               // breathing room.
               int min_height = label_size.height + add_size.height + 10;

               setMinimumSize(new Dimension(min_width, min_height));
               setPreferredSize(new Dimension(min_width, min_height));

               for ( Iterator c = cell.getChildren().iterator(); c.hasNext(); )
               {
                  Object child = c.next();

                  if ( child instanceof DefaultPort )
                  {
                     addUnitRow((DefaultPort) child, false);
                  }
               }

               // At this point, we only revalidate the layout.  There is no
               // need to change the cell view size because it is still being
               // set up by JGraph at this stage of execution.
               this.revalidate();

               mDeviceInfo.getElement().addConfigElementListener(this);
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

      public void propertyValueAdded(ConfigElementEvent evt)
      {
         PropertyDefinition prop_def = mDeviceInfo.getUnitPropertyDefinition();

         // If the device has a variable unit count and the value added was
         // for the device unit property, then we need to add another row to
         // the renderer to repersent the new unit.
         // mVariableUnits == true iff prop_def != null
         if ( mVariableUnits && prop_def.getToken().equals(evt.getProperty()) )
         {
            ConfigElement elt = mDeviceInfo.getElement();
            int count = elt.getPropertyValueCount(prop_def.getToken()) - 1;
            DefaultPort port = new DefaultPort(new Integer(count));
            ((DefaultGraphCell) mView.getCell()).add(port);
            addUnitRow(port, true);
         }
      }

      public void propertyValueChanged(ConfigElementEvent evt)
      {
      }

      public void propertyValueRemoved(ConfigElementEvent evt)
      {
/*
         PropertyDefinition prop_def = mDeviceInfo.getUnitPropertyDefinition();
         if ( mVariableUnits && prop_def.getToken().equals(evt.getProperty) )
         {
            removeUnitRow(evt.getIndex());
         }
*/
      }

      /**
       * Adds a new row for the given port that represents the input source
       * (device unit) for the input device represented by our vertex.
       * If the caller chooses not to resize the cell view after adding the
       * new row to this renderer, it is the responsibility of the caller to
       * perform the layout validation and the cell view resizing.  The
       * attributes of the port will be changed so that its bounds place it
       * within this row of the renderer layout.
       *
       * @param unitPort        the port for the input source represented by
       *                        the newly added row
       * @param resizeCellView  a flag indicating whether or not to revalidate
       *                        the renderer layout and to resize the cell
       *                        view associated with this renderer
       *
       * @see #revalidate()
       * @see GraphHelpers#autoSizeCellView(JGraph,Dimension,CellView)
       */
      private void addUnitRow(DefaultPort unitPort, boolean resizeCellView)
      {
         Object user_obj = unitPort.getUserObject();

         final PortComponent port_widget = new PortComponent(this.graph,
                                                             unitPort);
         port_widget.setMinimumSize(new Dimension(5, 5));
         port_widget.setPreferredSize(new Dimension(5, 5));
         port_widget.setForeground(nameLabel.getForeground());

         final JLabel name_field = new JLabel();

         PropertyDefinition unit_def = mDeviceInfo.getUnitPropertyDefinition();
         if ( unit_def != null && unit_def.getType() == ConfigElement.class )
         {
            Integer unit_num = (Integer) user_obj;
            ConfigElement dev_elt = mDeviceInfo.getElement();
            ConfigElement unit_elt =
               (ConfigElement) dev_elt.getProperty(unit_def.getToken(),
                                                   unit_num.intValue());
            name_field.setText(unit_elt.getName());
         }
         else
         {
            name_field.setText("Unit: " + user_obj.toString());
         }

         name_field.setFont(new Font("Dialog", Font.ITALIC,
                                     nameLabel.getFont().getSize()));
         name_field.setBorder(BorderFactory.createLineBorder(Color.black));
         name_field.setBackground(
            gradientColor != null ? gradientColor.brighter().brighter()
                                  : this.getBackground().brighter().brighter()
         );

         final int row = mMainLayout.getNumRow();
         int label_start_col, label_end_col;

         if ( mVariableUnits )
         {
            label_start_col = LABEL_START_COLUMN;
            label_end_col   = LABEL_END_COLUMN;
         }
         else
         {
            label_start_col = LABEL_START_COLUMN;
            label_end_col   = BUTTON0_END_COLUMN;
         }

         mMainLayout.insertRow(row, TableLayoutConstraints.PREFERRED);
         this.add(port_widget,
                  new TableLayoutConstraints(PORT_START_COLUMN, row,
                                             PORT_END_COLUMN, row,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.CENTER));
         this.add(name_field,
                  new TableLayoutConstraints(label_start_col, row,
                                             label_end_col, row,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.CENTER));

         int button_width = 0;

         if ( mVariableUnits )
         {
            final JButton remove_btn = new JButton();
            remove_btn.setEnabled(true);
            remove_btn.setToolTipText("Delete this input source");

            try
            {
               ClassLoader loader = getClass().getClassLoader();

               remove_btn.setIcon(
                  new ImageIcon(loader.getResource(IMG_ROOT + "/Delete16.gif"))
               );
               remove_btn.setMargin(new Insets(0, 0, 0, 0));
               remove_btn.setBorderPainted(false);
               remove_btn.setFocusPainted(false);
               remove_btn.setContentAreaFilled(false);
            }
            catch (Exception ex)
            {
               remove_btn.setMinimumSize(new Dimension(BUTTON_MIN_WIDTH,
                                                       BUTTON_MIN_HEIGHT));
               remove_btn.setPreferredSize(new Dimension(BUTTON_PREF_WIDTH,
                                                         BUTTON_PREF_HEIGHT));
               remove_btn.setFont(new Font("Dialog", Font.PLAIN, 9));
               remove_btn.setText("Delete");
            }

            remove_btn.addActionListener(
               new ActionListener()
               {
                  public void actionPerformed(ActionEvent evt)
                  {
                     JComponent[] components = new JComponent[]{port_widget,
                                                                name_field,
                                                                remove_btn};

                     int row = removeUnitRow(components);
                     removeUnit(row - mUnitStartRow);
                  }
               }
            );

            this.add(remove_btn,
                     new TableLayoutConstraints(BUTTON0_START_COLUMN, row,
                                                BUTTON0_END_COLUMN, row,
                                                TableLayoutConstraints.CENTER,
                                                TableLayoutConstraints.CENTER));

            button_width = remove_btn.getPreferredSize().width;
         }

         // Update the preferred size of the renderer based on the
         // dimensions of the newly added row.
         int width  = port_widget.getPreferredSize().width +
                         name_field.getPreferredSize().width + button_width;
         int height = name_field.getPreferredSize().height;

         Dimension pref_size = this.getPreferredSize();
         int new_width  = Math.max(width, pref_size.width);
         int new_height = height + pref_size.height;
         pref_size = new Dimension(new_width, new_height);
         this.setPreferredSize(pref_size);

         // Only resize the cell view associated with this renderer if we are
         // told to do so.  This is a bit of an optimization.
         if ( resizeCellView )
         {
            this.revalidate();
            GraphHelpers.autoSizeCellView(this.graph, pref_size, mView);
         }
      }

      /**
       * Handles the addition of a new unit to the device by the user.  This
       * can only happen by clicking the button for adding a new unit,
       * which implies that this device has a variable unit count.
       */
      private void addUnit()
      {
         ConfigElement elt = mDeviceInfo.getElement();
         PropertyDefinition unit_prop_def =
            mDeviceInfo.getUnitPropertyDefinition();
         PropertyValueDefinition value_def =
            unit_prop_def.getPropertyValueDefinition(0);
         String token = unit_prop_def.getToken();

         Object default_value = value_def.getDefaultValue();

         if ( default_value == null )
         {
            if ( unit_prop_def.getType() == ConfigElement.class )
            {
               ConfigBroker broker = new ConfigBrokerProxy();
               ConfigDefinitionRepository repos = broker.getRepository();
               ConfigElementFactory factory =
                  new ConfigElementFactory(repos.getAllLatest());

               // XXX: How do we deal with this?  The flexibility of allowed
               // types makes this tricky.
               int count = elt.getPropertyValueCount(token);
               default_value =
                  factory.create(value_def.getLabel() + " " + count,
                                 repos.get(unit_prop_def.getAllowedType(0)));
            }
            else
            {
               System.out.println("Don't know what to do for type " +
                                  unit_prop_def.getType());
            }
         }

         System.out.println("[DeviceVertexRenderer.addUnit()] " +
                            "default_value == " + default_value + " (type: " +
                            unit_prop_def.getType() + ")");
         elt.addProperty(token, default_value, mDeviceInfo.getContext());
      }

      /**
       *
       * @return the index of the row that was removed
       */
      private int removeUnitRow(JComponent[] components)
      {
         // Get the preferred size of the renderer so that we can modify it
         // after the row is removed.
         Dimension pref_size = this.getPreferredSize();

         // Set up the new preferred width and height based on the row that
         // is being removed.
         int width = pref_size.width;
         int height = pref_size.height;
         int max_height = 0;

         // Find the row containing the components being removed.  We expect
         // that all the components in the given array are in the same row.
         // XXX: Is there a more robust way to do this?
         TableLayoutConstraints tlc = mMainLayout.getConstraints(components[0]);
         int row = tlc.row1;

         DefaultPort port = null;
         for ( int i = 0; i < components.length; ++i )
         {
            if ( port == null && components[i] instanceof PortComponent )
            {
               port = ((PortComponent) components[i]).getPort();
            }

            if ( max_height < components[i].getPreferredSize().height )
            {
               max_height = components[i].getPreferredSize().height;
            }

            this.remove(components[i]);
         }

         // The height of the renderer will be decreased by the component
         // with the largest height in the given array of components.
         height -= max_height;

         mMainLayout.deleteRow(row);
         List removed_edges = new ArrayList();

         Frame parent = (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
                                                                  this);

         // For each of the edges associated with port (port will always be a
         // target), update the proxy on the other end of te edge (the source).
         // The proxy will be updated to refer to no device since the unit it
         // was expecting has gone away.
         for ( Iterator e = this.graph.getModel().edges(port); e.hasNext(); )
         {
            Edge edge = (Edge) e.next();
            removed_edges.add(edge);

            DefaultPort proxy_port = (DefaultPort) edge.getSource();
            DefaultGraphCell proxy_cell =
               (DefaultGraphCell) proxy_port.getParent();
            ProxyInfo proxy_info = (ProxyInfo) proxy_cell.getUserObject();
            ConfigElement proxy_elt = proxy_info.getElement();
            proxy_elt.setProperty(EditorConstants.DEVICE_PROPERTY, 0, "",
                                  proxy_info.getContext());

            int option =
               JOptionPane.showConfirmDialog(
                  parent,
                  "Remove proxy " + proxy_elt.getName() +
                     " and all its aliases?",
                  "Unit-less Proxy Removal", JOptionPane.YES_NO_OPTION,
                  JOptionPane.QUESTION_MESSAGE
               );

            if ( option == JOptionPane.YES_OPTION )
            {
               ConfigBroker broker = new ConfigBrokerProxy();
               ConfigContext ctx   = proxy_info.getContext();
               List resources      = ctx.getResources();

               for ( Iterator a = proxy_info.getAliases().iterator();
                     a.hasNext(); )
               {
                  ConfigElement alias_elt = (ConfigElement) a.next();
                  broker.remove(ctx, alias_elt);
               }

               broker.remove(ctx, proxy_elt);
               this.graph.getModel().remove(new Object[]{proxy_cell});
            }

            proxy_cell.setUserObject(null);
         }

         // Remove all the connections to port that have now been broken.
         this.graph.getModel().remove(removed_edges.toArray());

         // Remove port as a child of our cell.
         DefaultGraphCell device_cell = (DefaultGraphCell) mView.getCell();
         device_cell.remove(port);

         // Now, we need to update all the other ports that represent units
         // that are (sequentially) after the port that has been removed.
         // The ports need to have their contained Integer decremented, and
         // all proxies referring to each of those ports needs to have its
         // UNIT_PROPERTY updated to the new unit value.
         Integer unit_val = (Integer) port.getUserObject();

         for ( Enumeration children = device_cell.children();
               children.hasMoreElements(); )
         {
            Object child = children.nextElement();

            if ( child instanceof DefaultPort )
            {
               DefaultPort child_port = (DefaultPort) child;

               // If unit_value is less than child_port's Integer, then we
               // know that child_port's Integer comes after unit_value
               // sequentially.  Therefore, it needs to be updated to have its
               // Integer value decremented by one.
               if ( unit_val.compareTo(child_port.getUserObject()) < 0 )
               {
                  Integer old_value = (Integer) child_port.getUserObject();
                  Integer new_value = new Integer(old_value.intValue() - 1);
                  child_port.setUserObject(new_value);

                  // Update any proxies referring to the unit that has had its
                  // index decremented.
                  for ( Iterator i = this.graph.getModel().edges(child_port);
                        i.hasNext(); )
                  {
                     Edge edge = (Edge) i.next();
                     DefaultPort proxy_port = (DefaultPort) edge.getSource();
                     DefaultGraphCell proxy_cell =
                        (DefaultGraphCell) proxy_port.getParent();
                     ProxyInfo proxy_info =
                        (ProxyInfo) proxy_cell.getUserObject();
                     proxy_info.getElement().setProperty(
                        EditorConstants.UNIT_PROPERTY, 0, new_value,
                        proxy_info.getContext()
                     );
                  }
               }
            }
         }

         // Change the preferred size of the renderer to account for the row
         // removal.
         pref_size = new Dimension(width, height);
         this.setPreferredSize(pref_size);

         this.revalidate();

         // Resize the cell view now that the renderer has changed.
         GraphHelpers.autoSizeCellView(this.graph, pref_size, mView);

         return row;
      }

      private void removeUnit(int unitNumber)
      {
         ConfigElement elt = mDeviceInfo.getElement();
         ConfigContext ctx = mDeviceInfo.getContext();
         PropertyDefinition unit_prop_def =
            mDeviceInfo.getUnitPropertyDefinition();
         elt.removeProperty(unit_prop_def.getToken(), unitNumber, ctx);
      }

      private transient CellView   mView       = null;
      private transient DeviceInfo mDeviceInfo = null;

      private int     mUnitStartRow  = 0;
      private boolean mVariableUnits = false;

      private TableLayout mMainLayout    = null;
      private JButton     mAddUnitButton = new JButton();
   }
}
