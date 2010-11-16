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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph.extras;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Insets;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.*;
import javax.swing.*;
import javax.swing.border.TitledBorder;

import org.jgraph.JGraph;
import org.jgraph.graph.*;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.commoneditors.devicegraph.*;


/**
 * A custom vertex view for graph cells representing device config elements
 * for the IS-900 and Intersense API drivers.  This view makes use of
 * heavy-weight renderers so that users may interact with components in the
 * vertex view.
 */
public class IntersenseVertexView
   extends VertexView
{
   private transient DeviceVertexRenderer renderer =
      new DeviceVertexRenderer();

   public IntersenseVertexView()
   {
      super();
   }

   public IntersenseVertexView(Object cell)
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
      private static final int STATION_START_COLUMN     = 1;
      private static final int STATION_END_COLUMN       = 1;
      private static final int DEL_STATION_START_COLUMN = 3;
      private static final int DEL_STATION_END_COLUMN   = 3;

      private static final int PORT_START_COLUMN     = 1;
      private static final int PORT_END_COLUMN       = 1;
      private static final int LABEL_START_COLUMN    = 3;
      private static final int LABEL_END_COLUMN      = 3;
      private static final int DEL_UNIT_START_COLUMN = 5;
      private static final int DEL_UNIT_END_COLUMN   = 5;

      private static final int SPAN_START_COLUMN = PORT_START_COLUMN;
      private static final int SPAN_END_COLUMN   = DEL_UNIT_END_COLUMN;

      private static final int BUTTON_MIN_WIDTH   = 30;
      private static final int BUTTON_MIN_HEIGHT  = 16;
      private static final int BUTTON_PREF_WIDTH  = 30;
      private static final int BUTTON_PREF_HEIGHT = 16;

      private static final int NAME_ROW          = 1;
      private static final int ADD_STATION_ROW   = 2;
      private static final int STATION_START_ROW = 3;

      private static String IMG_ROOT = EditorConstants.COMMON_IMG_ROOT;

      public DeviceVertexRenderer()
      {
         super();

         double[][] sizes =
            {
               {5, TableLayout.FILL, 5, TableLayout.MINIMUM, 5},
               {5, TableLayout.PREFERRED, TableLayout.PREFERRED}
            };
         mMainLayout = new TableLayout(sizes);
         this.setLayout(mMainLayout);

         int span_start_column = STATION_START_COLUMN,
             span_end_column = DEL_STATION_END_COLUMN;

         nameLabel.setText(" ");

         ImageIcon add_station_icon = null;
         try
         {
            ClassLoader loader = getClass().getClassLoader();
            add_station_icon =
               new ImageIcon(loader.getResource(IMG_ROOT + "/New16.gif"));
         }
         catch (Exception ex)
         {
         }

         mAddStationButton.setEnabled(false);
         mAddStationButton.setToolTipText("Add a new station for this device");

         mAddStationButton.setText("Add Station");
         mAddStationButton.setMargin(new Insets(3, 0, 0, 0));
         mAddStationButton.setBorderPainted(false);
         mAddStationButton.setFont(new Font("Dialog", Font.PLAIN, 10));
         mAddStationButton.setContentAreaFilled(false);
         mAddStationButton.setIcon(add_station_icon);

         mAddStationButton.addActionListener(
            new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  ConfigBroker broker = new ConfigBrokerProxy();
                  ConfigDefinitionRepository repos = broker.getRepository();
                  ConfigElementFactory factory =
                     new ConfigElementFactory(repos.getAllLatest());
                  ConfigElement dev_elt = mDeviceInfo.getElement();

                  ConfigDefinition station_def =
                     repos.get(INTERSENSE_STATION_TYPE);

                  int station_num =
                     dev_elt.getPropertyValueCount(STATIONS_PROPERTY);
                  ConfigElement station_elt =
                     factory.create(station_def.getName() + " " + station_num,
                                    station_def);
                  dev_elt.addProperty(STATIONS_PROPERTY, station_elt,
                                      mDeviceInfo.getContext());
               }
            }
         );

         this.add(nameLabel,
                  new TableLayoutConstraints(span_start_column, NAME_ROW,
                                             span_end_column, NAME_ROW,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.CENTER));
         this.add(mAddStationButton,
                  new TableLayoutConstraints(span_start_column,
                                             ADD_STATION_ROW, span_end_column,
                                             ADD_STATION_ROW,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.CENTER));

         Dimension label_dim  = nameLabel.getPreferredSize();
         Dimension button_dim = mAddStationButton.getPreferredSize();
         int height = label_dim.height + button_dim.height;
         int width  = this.getPreferredSize().width;
         this.setPreferredSize(new Dimension(width, height));
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

               mAddStationButton.setEnabled(true);

               Dimension label_size = nameLabel.getPreferredSize();

               int min_width = label_size.width;

               // The extra 10 units of height is to allow some vertical
               // breathing room.
               int min_height = label_size.height + 10;

               setMinimumSize(new Dimension(min_width, min_height));
               setPreferredSize(new Dimension(min_width, min_height));

               int extra_width = min_width, extra_height = 0;

               ConfigElement dev_elt = mDeviceInfo.getElement();
               int station_count =
                  dev_elt.getPropertyValueCount(STATIONS_PROPERTY);

               for ( int i = 0; i < station_count; ++i )
               {
                  addStationPanel(i, false);
               }

               Dimension pref_size = getPreferredSize();
               setPreferredSize(
                  new Dimension(Math.max(pref_size.width, extra_width),
                                pref_size.height + extra_height)
               );

               // At this point, we only revalidate the layout.  There is no
               // need to change the cell view size because it is still being
               // set up by JGraph at this stage of execution.
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

      public void propertyValueAdded(ConfigElementEvent evt)
      {
         ConfigElement src_elt = (ConfigElement) evt.getSource();
         String token = src_elt.getDefinition().getToken();

         if ( token.equals(INTERSENSE_TYPE) ||
              token.equals(INTERSENSE_API_TYPE) )
         {
            if ( evt.getProperty().equals(STATIONS_PROPERTY) )
            {
               UnitInfo pos_info = new UnitInfo(UnitConstants.POSITION,
                                                new Integer(evt.getIndex()));
               DefaultPort pos_port = GraphHelpers.createDevicePort(pos_info);
               ((DefaultGraphCell) mView.getCell()).add(pos_port);

               addStationPanel(evt.getIndex(), true);
            }
         }
      }

      public void propertyValueChanged(ConfigElementEvent evt)
      {
         ConfigElement src_elt = (ConfigElement) evt.getSource();
         String token = src_elt.getDefinition().getToken();

         if ( token.equals(INTERSENSE_STATION_TYPE) )
         {
            if ( evt.getProperty().equals(ANALOG_COUNT_PROPERTY) ||
                 evt.getProperty().equals(DIGITAL_COUNT_PROPERTY) )
            {
               ConfigElement dev_elt = mDeviceInfo.getElement();

               int station_num =
                  dev_elt.getPropertyValueIndex(STATIONS_PROPERTY, src_elt);
               StationGroup sg = null;

               if ( station_num != -1 )
               {
                  sg = (StationGroup) mStations.get(station_num);
               }

               Integer old_count = (Integer) evt.getValue();
               Integer cur_count =
                  (Integer) src_elt.getProperty(evt.getProperty(), 0);

               // Unit addition.
               if ( cur_count.compareTo(old_count) > 0 )
               {
                  Integer unit_type;
                  if ( evt.getProperty().equals(ANALOG_COUNT_PROPERTY) )
                  {
                     unit_type = UnitConstants.ANALOG;
                  }
                  else
                  {
                     unit_type = UnitConstants.DIGITAL;
                  }

                  // Determine how many units of unit_type are already defined.
                  // That value will be used as the unit number for the newly
                  // added unit.
                  int unit_count = old_count.intValue();
                  for ( int i = 0; i < station_num; ++i )
                  {
                     StationGroup cur_sg = (StationGroup) mStations.get(i);
                     unit_count += cur_sg.getUnitCount(unit_type);
                  }

                  UnitInfo unit_info = new UnitInfo(unit_type, unit_count);
                  DefaultPort port = GraphHelpers.createDevicePort(unit_info);
                  ((DefaultGraphCell) mView.getCell()).add(port);

                  addUnitRow(sg, port, true, true);
                  shiftUnitsUp(station_num, unit_info);

                  // Keep track of the rows that are added dynamically.
                  // These may need to be removed later as a result of
                  // an undo operation.
                  mAddedUnitRows.push(unit_info);
               }
               // Unit removal.
               else
               {
                  DefaultGraphCell cell = (DefaultGraphCell) mView.getCell();

                  UnitInfo search_info = null;

                  // This bit is needed for handling undo and redo correctly.
                  // mRemovedUnitInfo is only non-null when a row is removed
                  // as a result of the user clicking on the "Delete" button
                  // for a row.
                  if ( mRemovedUnitInfo == null )
                  {
                     if ( ! mAddedUnitRows.empty() )
                     {
                        search_info = (UnitInfo) mAddedUnitRows.pop();
                     }
                  }
                  else
                  {
                     search_info = mRemovedUnitInfo;
                     mAddedUnitRows.remove(mRemovedUnitInfo);
                  }

                  DefaultPort port = findPort(cell, search_info);

                  if ( port != null )
                  {
                     removeUnitRow(sg, port);
                     mRemovedUnitInfo = null;
                  }
                  else
                  {
                     System.err.println("WARNING: Could not find port " +
                                        "to remove containing an object " +
                                        "matching " +
                                        mRemovedUnitInfo);
                  }
               }
            }
         }
      }

      public void propertyValueOrderChanged(ConfigElementEvent evt)
      {
         System.err.println(
            "WARNING: IntersenseVertexView.propertyValueOrderChanged() " +
            "is not implemented!  Complain about this."
         );
      }

      public void propertyValueRemoved(ConfigElementEvent evt)
      {
         ConfigElement src_elt = (ConfigElement) evt.getSource();
         String token = src_elt.getDefinition().getToken();

         if ( token.equals(INTERSENSE_TYPE) ||
              token.equals(INTERSENSE_API_TYPE) )
         {
            if ( evt.getProperty().equals(STATIONS_PROPERTY) )
            {
               removeStationPanel(evt.getIndex());
            }
         }
      }

      private DefaultPort findPort(DefaultGraphCell parent,
                                   UnitInfo searchUnitInfo)
      {
         DefaultPort port = null;

         for ( Iterator c = parent.getChildren().iterator(); c.hasNext(); )
         {
            Object child = c.next();
            if ( child instanceof DefaultPort )
            {
               UnitInfo cur_unit_info =
                  (UnitInfo) ((DefaultPort) child).getUserObject();
               if ( searchUnitInfo.equals(cur_unit_info) )
               {
                  port = (DefaultPort) child;
                  break;
               }
            }
         }

         return port;
      }

      private void removeStationProperty(int stationNum)
      {
         ConfigElement dev_elt = mDeviceInfo.getElement();
         ConfigElement station_elt =
            (ConfigElement) dev_elt.getProperty(STATIONS_PROPERTY,
                                                stationNum);
         station_elt.removeConfigElementListener(this);
         dev_elt.removeProperty(STATIONS_PROPERTY, stationNum,
                                mDeviceInfo.getContext());
      }

      private void addStationPanel(int stationNum, boolean resizeCellView)
      {
         double[][] sizes =
            {
               {5, TableLayout.MINIMUM, 5, TableLayout.FILL, 5,
                TableLayout.MINIMUM, 5},
               {TableLayout.PREFERRED, TableLayout.PREFERRED,
                TableLayout.PREFERRED}
            };
         int pos_unit_row = 0, ana_btn_row = 1, dig_btn_row = 2;

         Color shadow_color = new Color(142, 142, 142);
         Insets button_insets = new Insets(3, 0, 0, 0);
         Font button_font = new Font("Dialog", Font.PLAIN, 10);
         Font unit_label_font = new Font("Dialog", Font.ITALIC,
                                         nameLabel.getFont().getSize());

         ConfigElement dev_elt = mDeviceInfo.getElement();
         ConfigElement station_elt =
            (ConfigElement) dev_elt.getProperty(STATIONS_PROPERTY, stationNum);

         // Hack around JGraph oddities.
         if ( ! this.preview )
         {
            station_elt.addConfigElementListener(this);
         }

         JPanel station_panel = new JPanel();
         TableLayout main_layout = new TableLayout(sizes);

         station_panel.setOpaque(false);
         station_panel.setLayout(main_layout);
         station_panel.setBorder(
            new TitledBorder(
               BorderFactory.createEtchedBorder(Color.white,
                                                shadow_color),
               "Station " + stationNum
            )
         );

         JButton add_ana_button = new JButton();
         JButton add_dig_button = new JButton();
         JButton remove_station_button = new JButton();

         DefaultPort pos_unit_port = null;

         DefaultGraphCell cell = (DefaultGraphCell) mView.getCell();
         Integer pos_unit_num = new Integer(stationNum);

         for ( Iterator c = cell.getChildren().iterator();
               c.hasNext(); )
         {
            Object child_obj = c.next();

            if ( child_obj instanceof DefaultPort )
            {
               UnitInfo unit_info =
                  (UnitInfo) ((DefaultPort) child_obj).getUserObject();
               if ( unit_info.getUnitType().equals(UnitConstants.POSITION) &&
                    unit_info.getUnitNumber().equals(pos_unit_num) )
               {
                  pos_unit_port = (DefaultPort) child_obj;
                  break;
               }
            }
         }

         if ( pos_unit_port == null )
         {
            throw new IllegalArgumentException(
               "Could not find port for position unit #" + stationNum
            );
         }

         PortComponent pos_port = createPortComponent(pos_unit_port);

         station_panel.add(
            pos_port,
            new TableLayoutConstraints(PORT_START_COLUMN,
                                       pos_unit_row, PORT_END_COLUMN,
                                       pos_unit_row,
                                       TableLayoutConstraints.CENTER,
                                       TableLayoutConstraints.CENTER)
         );

         JLabel pos_unit_label =
            createUnitLabel((UnitInfo) pos_unit_port.getUserObject());

         station_panel.add(
            pos_unit_label,
            new TableLayoutConstraints(LABEL_START_COLUMN,
                                       pos_unit_row, LABEL_END_COLUMN,
                                       pos_unit_row,
                                       TableLayoutConstraints.FULL,
                                       TableLayoutConstraints.CENTER)
         );

         ClassLoader loader = getClass().getClassLoader();
         ImageIcon add_icon = null, del_icon = null;

         try
         {
            add_icon =
               new ImageIcon(loader.getResource(IMG_ROOT + "/New16.gif"));
         }
         catch (Exception ex)
         {
         }

         try
         {
            del_icon =
               new ImageIcon(loader.getResource(IMG_ROOT +
                                                "/Delete16.gif"));
         }
         catch (Exception ex)
         {
         }

         add_ana_button.setEnabled(true);
         add_ana_button.setToolTipText(
            "Add a new analog input source for this device"
         );
         add_ana_button.setText("Create Analog Input Source");
         add_ana_button.setMargin(button_insets);
         add_ana_button.setBorderPainted(false);
         add_ana_button.setFont(button_font);
         add_ana_button.setContentAreaFilled(false);
         add_ana_button.setIcon(add_icon);

         add_dig_button.setEnabled(true);
         add_dig_button.setToolTipText(
            "Add a new digital input source for this device"
         );
         add_dig_button.setText("Create Digital Input Source");
         add_dig_button.setMargin(button_insets);
         add_dig_button.setBorderPainted(false);
         add_dig_button.setFont(button_font);
         add_dig_button.setContentAreaFilled(false);
         add_dig_button.setIcon(add_icon);

         final StationGroup sg = new StationGroup(station_panel, main_layout,
                                                  remove_station_button,
                                                  pos_unit_label,
                                                  add_ana_button,
                                                  add_dig_button);
         mStations.add(sg);

         // For some reason, we cannot use an action listener here.  Something
         // goes wrong when the MouseEvent is redispatched to the button.
         // Using a mouse listener works.
         add_ana_button.addMouseListener(
            new MouseAdapter()
            {
               public void mouseClicked(MouseEvent e)
               {
                  addUnit(sg, UnitConstants.ANALOG);
               }
            }
         );
         add_dig_button.addMouseListener(
            new MouseAdapter()
            {
               public void mouseClicked(MouseEvent e)
               {
                  addUnit(sg, UnitConstants.DIGITAL);
               }
            }
         );

         station_panel.add(
            add_ana_button,
            new TableLayoutConstraints(SPAN_START_COLUMN, ana_btn_row,
                                       SPAN_END_COLUMN, ana_btn_row,
                                       TableLayoutConstraints.FULL,
                                       TableLayoutConstraints.CENTER));
         station_panel.add(
            add_dig_button,
            new TableLayoutConstraints(SPAN_START_COLUMN, dig_btn_row,
                                       SPAN_END_COLUMN, dig_btn_row,
                                       TableLayoutConstraints.FULL,
                                       TableLayoutConstraints.CENTER));

         Integer ana_count =
            (Integer) station_elt.getProperty(ANALOG_COUNT_PROPERTY, 0);
         Integer dig_count =
            (Integer) station_elt.getProperty(DIGITAL_COUNT_PROPERTY, 0);

         for ( int j = 0; j < ana_count.intValue(); ++j )
         {
            Integer ana_unit_num = new Integer(j);

            for ( Iterator c = cell.getChildren().iterator();
                  c.hasNext(); )
            {
               Object child = c.next();

               if ( child instanceof DefaultPort )
               {
                  DefaultPort port = (DefaultPort) child;
                  UnitInfo unit_info = (UnitInfo) port.getUserObject();
                  if ( unit_info.getUnitType().equals(UnitConstants.ANALOG) &&
                       unit_info.getUnitNumber().equals(ana_unit_num) )
                  {
                     addUnitRow(sg, (DefaultPort) child, false,
                                resizeCellView);
                  }
               }
            }
         }

         for ( int j = 0; j < dig_count.intValue(); ++j )
         {
            Integer dig_unit_num = new Integer(j);

            for ( Iterator c = cell.getChildren().iterator();
                  c.hasNext(); )
            {
               Object child = c.next();

               if ( child instanceof DefaultPort )
               {
                  DefaultPort port = (DefaultPort) child;
                  UnitInfo unit_info = (UnitInfo) port.getUserObject();
                  if ( unit_info.getUnitType().equals(UnitConstants.DIGITAL) &&
                       unit_info.getUnitNumber().equals(dig_unit_num) )
                  {
                     addUnitRow(sg, (DefaultPort) child, false,
                                resizeCellView);
                  }
               }
            }
         }

         int station_row = STATION_START_ROW + stationNum;
         mMainLayout.insertRow(station_row, TableLayout.PREFERRED);

         remove_station_button.setEnabled(true);
         remove_station_button.setToolTipText(
            "Remove this station from the device configuration"
         );
         remove_station_button.setMargin(button_insets);
         remove_station_button.setBorderPainted(false);
         remove_station_button.setFont(button_font);
         remove_station_button.setContentAreaFilled(false);

         if ( del_icon != null )
         {
            remove_station_button.setIcon(del_icon);
         }
         else
         {
            remove_station_button.setText("Remove Station");
         }

         remove_station_button.addActionListener(
            new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  JButton button = (JButton) evt.getSource();
                  TableLayoutConstraints tlc =
                     mMainLayout.getConstraints(button);
                  int station_num = tlc.row1 - STATION_START_ROW;
                  removeStationProperty(station_num);
               }
            }
         );

         this.add(station_panel,
                  new TableLayoutConstraints(STATION_START_COLUMN,
                                             station_row,
                                             STATION_END_COLUMN,
                                             station_row,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.FULL));
         this.add(remove_station_button,
                  new TableLayoutConstraints(DEL_STATION_START_COLUMN,
                                             station_row,
                                             DEL_STATION_END_COLUMN,
                                             station_row,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.TOP));

         Dimension renderer_size = this.getPreferredSize();
         Dimension station_size = station_panel.getPreferredSize();
         int new_width  = Math.max(renderer_size.width, station_size.width);
         int new_height = renderer_size.height + station_size.height;
         this.setPreferredSize(new Dimension(new_width, new_height));

         // Only resize the cell view associated with this renderer if we are
         // told to do so.  This is a bit of an optimization.
         if ( resizeCellView )
         {
            this.revalidate();
            GraphHelpers.autoSizeCellView(this.graph, this.getPreferredSize(),
                                          mView);
         }
      }

      private void removeStationPanel(int stationNum)
      {
         StationGroup sg = (StationGroup) mStations.get(stationNum);

         TableLayoutConstraints tlc =
            mMainLayout.getConstraints(sg.stationPanel);

         int removed_height = sg.stationPanel.getPreferredSize().height;

         DefaultGraphCell cell = (DefaultGraphCell) mView.getCell();

         List removed_ports = new ArrayList();

         // Find the position unit associated with the removed station.
         for ( Iterator c = cell.getChildren().iterator(); c.hasNext(); )
         {
            Object child_obj = c.next();

            if ( child_obj instanceof DefaultPort )
            {
               DefaultPort unit_port = (DefaultPort) child_obj;
               UnitInfo port_info    = (UnitInfo) unit_port.getUserObject();

               if ( port_info.getUnitType().equals(UnitConstants.POSITION) &&
                    port_info.getUnitNumber().intValue() == stationNum )
               {
                  removed_ports.add(unit_port);
                  break;
               }
            }
         }

         // Find the digital and analog units (if any) associated with the
         // removed station.
         for ( Iterator u = sg.getUnits().iterator(); u.hasNext(); )
         {
            UnitInfo unit_info = (UnitInfo) u.next();

            for ( Iterator c = cell.getChildren().iterator(); c.hasNext(); )
            {
               Object child_obj = c.next();

               if ( child_obj instanceof DefaultPort )
               {
                  DefaultPort unit_port = (DefaultPort) child_obj;

                  if ( unit_info.equals(unit_port.getUserObject()) )
                  {
                     removed_ports.add(unit_port);
                     break;
                  }
               }
            }
         }

         // Clear out all the edges (and, optionally, vertices) associated
         // with the port(s) being removed.
         List removed_edges    = new ArrayList();
         List removed_vertices = new ArrayList();

         for ( Iterator p = removed_ports.iterator(); p.hasNext(); )
         {
            removePortEdges((DefaultPort) p.next(), removed_edges,
                            removed_vertices);
         }

         DeviceGraphModel model = (DeviceGraphModel) this.graph.getModel();
         model.remove(removed_edges.toArray(), removed_vertices.toArray(),
                      false);

         // Remove all the ports from our cell.
         for ( Iterator p = removed_ports.iterator(); p.hasNext(); )
         {
            cell.remove((DefaultPort) p.next());
         }

         // Delay removal of the UI components until after the graph has been
         // updated.  This is done for visual consistency more than anything.
         sg.stationPanel.removeAll();
         this.remove(sg.stationPanel);
         mMainLayout.deleteRow(tlc.row1);

         int removed_analog  = sg.getUnitCount(UnitConstants.ANALOG);
         int removed_digital = sg.getUnitCount(UnitConstants.DIGITAL);

         List changed_pos_info = new ArrayList();
         List changed_ana_info = new ArrayList();
         List changed_dig_info = new ArrayList();

         // Update the proxies referring to unit numbers that were greater
         // than the ones that have been removed.
         for ( Iterator p = removed_ports.iterator(); p.hasNext(); )
         {
            UnitInfo unit_info =
               (UnitInfo) ((DefaultPort) p.next()).getUserObject();
            Integer unit_num = unit_info.getUnitNumber();

            for ( Iterator c = cell.getChildren().iterator(); c.hasNext(); )
            {
               Object child_obj = c.next();

               if ( child_obj instanceof DefaultPort )
               {
                  DefaultPort unit_port = (DefaultPort) child_obj;
                  UnitInfo port_info = (UnitInfo) unit_port.getUserObject();

                  if ( unit_num.compareTo(port_info.getUnitNumber()) < 0 )
                  {
                     // Analog unit.
                     if ( port_info.getUnitType().equals(UnitConstants.ANALOG) )
                     {
                        decrementUnitNumber(unit_port, removed_analog);
                        changed_ana_info.add(port_info);
                     }
                     // Digital unit.
                     else if ( port_info.getUnitType().equals(UnitConstants.DIGITAL) )
                     {
                        decrementUnitNumber(unit_port, removed_digital);
                        changed_dig_info.add(port_info);
                     }
                     // Position unit.
                     else
                     {
                        decrementUnitNumber(unit_port, 1);
                        changed_pos_info.add(port_info);
                     }
                  }
               }
            }
         }

         mStations.remove(sg);

         int station_count = mStations.size();
         for ( int i = stationNum; i < station_count; ++i )
         {
            StationGroup cur_sg = (StationGroup) mStations.get(i);
            TitledBorder border =
               (TitledBorder) cur_sg.stationPanel.getBorder();
            border.setTitle("Station " + i);
         }

         // Change the preferred size of the renderer to account for the row
         // removal.
         Dimension cur_size  = this.getPreferredSize();
         Dimension pref_size = new Dimension(cur_size.width,
                                             cur_size.height - removed_height);
         this.setPreferredSize(pref_size);

         this.revalidate();

         // Resize the cell view now that the renderer has changed.
         GraphHelpers.autoSizeCellView(this.graph, pref_size, mView);
      }

      private JLabel createUnitLabel(UnitInfo unitInfo)
      {
         JLabel label = new UnitLabel(unitInfo);
         label.setFont(new Font("Dialog", Font.ITALIC,
                                nameLabel.getFont().getSize()));
         label.setBorder(BorderFactory.createLineBorder(Color.black));
         label.setBackground(
            gradientColor != null ? gradientColor.brighter().brighter()
                                  : this.getBackground().brighter().brighter()
         );

         return label;
      }

      private PortComponent createPortComponent(DefaultPort port)
      {
         PortComponent pc = new PortComponent(this.graph, port, this);
         pc.setMinimumSize(new Dimension(5, 5));
         pc.setPreferredSize(new Dimension(5, 5));
         pc.setForeground(nameLabel.getForeground());

         return pc;
      }

      /**
       * Handles the addition of a new unit to the device by the user.  This
       * can only happen by clicking the button for adding a new unit,
       * which implies that this device has a variable unit count.
       */
      private void addUnit(StationGroup stationGroup, Integer unitType)
      {
         int station_num = mStations.indexOf(stationGroup);
         ConfigElement dev_elt = mDeviceInfo.getElement();
         ConfigElement station_elt =
            (ConfigElement) dev_elt.getProperty(STATIONS_PROPERTY,
                                                station_num);

         if ( unitType.equals(UnitConstants.ANALOG) )
         {
            Integer cur_count =
               (Integer) station_elt.getProperty(ANALOG_COUNT_PROPERTY, 0);
            station_elt.setProperty(ANALOG_COUNT_PROPERTY, 0,
                                    new Integer(cur_count.intValue() + 1),
                                    mDeviceInfo.getContext());
         }
         else
         {
            Integer cur_count =
               (Integer) station_elt.getProperty(DIGITAL_COUNT_PROPERTY, 0);
            station_elt.setProperty(DIGITAL_COUNT_PROPERTY, 0,
                                    new Integer(cur_count.intValue() + 1),
                                    mDeviceInfo.getContext());
         }
      }

      private void removeUnit(StationGroup stationGroup, UnitInfo unitInfo)
      {
         int station_num = mStations.indexOf(stationGroup);
         ConfigElement dev_elt = mDeviceInfo.getElement();
         ConfigElement station_elt =
            (ConfigElement) dev_elt.getProperty(STATIONS_PROPERTY,
                                                station_num);

         mRemovedUnitInfo = unitInfo;

         if ( unitInfo.getUnitType().equals(UnitConstants.ANALOG) )
         {
            Integer cur_count =
               (Integer) station_elt.getProperty(ANALOG_COUNT_PROPERTY, 0);
            station_elt.setProperty(ANALOG_COUNT_PROPERTY, 0,
                                    new Integer(cur_count.intValue() - 1),
                                    mDeviceInfo.getContext());
         }
         else
         {
            Integer cur_count =
               (Integer) station_elt.getProperty(DIGITAL_COUNT_PROPERTY, 0);
            station_elt.setProperty(DIGITAL_COUNT_PROPERTY, 0,
                                    new Integer(cur_count.intValue() - 1),
                                    mDeviceInfo.getContext());
         }
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
       * @param stationGroup    
       * @param unitPort        the port for the input source represented by
       *                        the newly added row
       * @param resizeCellView  a flag indicating whether or not to revalidate
       *                        the renderer layout and to resize the cell
       *                        view associated with this renderer
       *
       * @see #revalidate()
       * @see GraphHelpers#autoSizeCellView(JGraph,Dimension,CellView)
       */
      private void addUnitRow(final StationGroup stationGroup,
                              DefaultPort unitPort, boolean resizeRenderer,
                              boolean resizeCellView)
      {
         final UnitInfo unit_info = (UnitInfo) unitPort.getUserObject();
         Integer unit_type = unit_info.getUnitType();

         PortComponent port_widget = createPortComponent(unitPort);

         JLabel name_field = createUnitLabel(unit_info);

         TableLayout sg_layout = stationGroup.mainLayout;

         // Add this here to get the unit count updated correctly before we
         // use that information to add to stationGroup.stationPanel.
         stationGroup.addUnit(unit_info);

         // Calculate the new row as an offset to the appropriate "Add" button.
         TableLayoutConstraints c =
            sg_layout.getConstraints(stationGroup.getAddButton(unit_type));
         int row = c.row1 + stationGroup.getUnitCount(unit_type);

         sg_layout.insertRow(row, TableLayoutConstraints.PREFERRED);
         stationGroup.stationPanel.add(
            port_widget,
            new TableLayoutConstraints(PORT_START_COLUMN, row,
                                       PORT_END_COLUMN, row,
                                       TableLayoutConstraints.CENTER,
                                       TableLayoutConstraints.CENTER)
         );
         stationGroup.stationPanel.add(
            name_field,
            new TableLayoutConstraints(LABEL_START_COLUMN, row,
                                       LABEL_END_COLUMN, row,
                                       TableLayoutConstraints.FULL,
                                       TableLayoutConstraints.CENTER)
         );

         JButton remove_btn = new JButton();
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

         // For some reason, we cannot use an action listener here.  Something
         // goes wrong when the MouseEvent is redispatched to the button.
         // Using a mouse listener works.
         remove_btn.addMouseListener(
            new MouseAdapter()
            {
               public void mouseClicked(MouseEvent evt)
               {
                  removeUnit(stationGroup, unit_info);
               }
            }
         );

         stationGroup.stationPanel.add(
            remove_btn,
            new TableLayoutConstraints(DEL_UNIT_START_COLUMN, row,
                                       DEL_UNIT_END_COLUMN, row,
                                       TableLayoutConstraints.CENTER,
                                       TableLayoutConstraints.CENTER)
         );

         if ( resizeRenderer )
         {
            // Update the preferred size of the renderer based on the
            // dimensions of the newly added row.
            int width  = port_widget.getPreferredSize().width +
                            name_field.getPreferredSize().width +
                            remove_btn.getPreferredSize().width;
            int height = name_field.getPreferredSize().height;

            Dimension pref_size = this.getPreferredSize();
            int new_width  = Math.max(width, pref_size.width);
            int new_height = height + pref_size.height;
            pref_size = new Dimension(new_width, new_height);
            this.setPreferredSize(pref_size);
         }

         // Only resize the cell view associated with this renderer if we are
         // told to do so.  This is a bit of an optimization.
         if ( resizeCellView )
         {
            this.revalidate();
            GraphHelpers.autoSizeCellView(this.graph, this.getPreferredSize(),
                                          mView);
         }
      }

      /**
       * Increments all unit numbers occurring after <code>unitInfo</code>
       * by one.
       */
      private void shiftUnitsUp(int stationNum, UnitInfo unitInfo)
      {
         Integer unit_type = unitInfo.getUnitType();
         Integer unit_num  = unitInfo.getUnitNumber();

         DefaultGraphCell cell = (DefaultGraphCell) mView.getCell();

         for ( Iterator c = cell.getChildren().iterator(); c.hasNext(); )
         {
            Object child_obj = c.next();

            if ( child_obj instanceof DefaultPort )
            {
               DefaultPort cur_port = (DefaultPort) child_obj;
               UnitInfo port_info   = (UnitInfo) cur_port.getUserObject();

               if ( unitInfo != port_info &&
                    unit_type.equals(port_info.getUnitType()) &&
                    unit_num.compareTo(port_info.getUnitNumber()) <= 0 )
               {
                  incrementUnitNumber(cur_port, 1);
               }
            }
         }
      }

      /**
       *
       */
      private void removeUnitRow(StationGroup stationGroup, DefaultPort port)
      {
         UnitInfo old_unit_info = (UnitInfo) port.getUserObject();
         Integer unit_type = old_unit_info.getUnitType();

         int station_num = mStations.indexOf(stationGroup);
         int prev_station_unit_count = 0;

         // Determine how many units of type unit_type are defined by the
         // preceding station(s).
         for ( int i = 0; i < station_num; ++i )
         {
            StationGroup cur_sg = (StationGroup) mStations.get(i);
            prev_station_unit_count += cur_sg.getUnitCount(unit_type);
         }

         // The row to remove is based on the offset from the appropriate
         // "Add" button within the given station group panel.
         TableLayoutConstraints tlc =
            stationGroup.mainLayout.getConstraints(
               stationGroup.getAddButton(unit_type)
            );
         int row = tlc.row1 + old_unit_info.getUnitNumber().intValue() -
                      prev_station_unit_count + 1;

         // Get the preferred size of the renderer so that we can modify it
         // after the row is removed.
         Dimension pref_size = this.getPreferredSize();

         // Set up the new preferred width and height based on the row that
         // is being removed.
         int width = pref_size.width;
         int height = pref_size.height;
         int max_height = 0;

         // Find the row containing the components being removed.
         Component[] components = stationGroup.stationPanel.getComponents();
         for ( int i = 0; i < components.length; ++i )
         {
            TableLayoutConstraints cur_tlc =
               stationGroup.mainLayout.getConstraints(components[i]);

            if ( cur_tlc.row1 == row )
            {
               max_height = Math.max(max_height,
                                     components[i].getPreferredSize().height);
               stationGroup.stationPanel.remove(components[i]);
            }
         }

         // The height of the renderer will be decreased by the component
         // with the largest height in the given array of components.
         height -= max_height;

         stationGroup.mainLayout.deleteRow(row);
         List removed_edges    = new ArrayList();
         List removed_vertices = new ArrayList();

         removePortEdges(port, removed_edges, removed_vertices);

         DeviceGraphModel model = (DeviceGraphModel) this.graph.getModel();
         model.remove(removed_edges.toArray(), removed_vertices.toArray(),
                      false);

         // Remove port as a child of our cell.
         DefaultGraphCell device_cell = (DefaultGraphCell) mView.getCell();
         device_cell.remove(port);

         // Complete the unit removal by removing it from its station group.
         stationGroup.removeUnit(old_unit_info);

         // Now, we need to update all the other ports that represent units
         // that are (sequentially) after the port that has been removed.
         // The ports need to have their contained Integer decremented, and
         // all proxies referring to each of those ports needs to have its
         // UNIT_PROPERTY updated to the new unit value.
         Integer unit_val = old_unit_info.getUnitNumber();

         for ( Enumeration children = device_cell.children();
               children.hasMoreElements(); )
         {
            Object child = children.nextElement();

            if ( child instanceof DefaultPort )
            {
               DefaultPort child_port = (DefaultPort) child;
               UnitInfo unit_info     = (UnitInfo) child_port.getUserObject();

               // If unit_value is less than child_port's unit number, then we
               // know that child_port's unit number comes after unit_value
               // sequentially.  Therefore, it needs to be updated to have its
               // Integer value decremented by one.
               if ( unit_type.equals(unit_info.getUnitType()) &&
                    unit_val.compareTo(unit_info.getUnitNumber()) < 0 )
               {
                  decrementUnitNumber(child_port, 1);
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
      }

      private void removePortEdges(DefaultPort port, List removedEdges,
                                   List removedVertices)
      {
         Window parent =
            (Window) SwingUtilities.getAncestorOfClass(Window.class, this);
         ConfigBroker broker = new ConfigBrokerProxy();

         // For each of the edges associated with port (port will always be a
         // target), update the proxy on the other end of te edge (the source).
         // The proxy will be updated to refer to no device since the unit it
         // was expecting has gone away.
         for ( Iterator e = this.graph.getModel().edges(port); e.hasNext(); )
         {
            Edge edge = (Edge) e.next();
            removedEdges.add(edge);

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
               ConfigContext ctx = proxy_info.getContext();
               List resources    = ctx.getResources();

               for ( Iterator a = proxy_info.getAliases().iterator();
                     a.hasNext(); )
               {
                  ConfigElement alias_elt = (ConfigElement) a.next();
                  broker.remove(ctx, alias_elt);
               }

               broker.remove(ctx, proxy_elt);
               removedVertices.add(proxy_cell);
            }
         }
      }

      private void incrementUnitNumber(DefaultPort port, int amount)
      {
         UnitInfo unit_info = (UnitInfo) port.getUserObject();
         unit_info.incrementUnitNumber(amount);

         // Update any proxies referring to the unit that has had its index
         // decremented.
         for ( Iterator i = this.graph.getModel().edges(port); i.hasNext(); )
         {
            Edge edge = (Edge) i.next();
            DefaultPort proxy_port = (DefaultPort) edge.getSource();
            DefaultGraphCell proxy_cell =
               (DefaultGraphCell) proxy_port.getParent();
            ProxyInfo proxy_info = (ProxyInfo) proxy_cell.getUserObject();
            proxy_info.getElement().setProperty(UNIT_PROPERTY, 0,
                                                unit_info.getUnitNumber(),
                                                proxy_info.getContext());
         }
      }

      private void decrementUnitNumber(DefaultPort port, int amount)
      {
         UnitInfo unit_info = (UnitInfo) port.getUserObject();
         unit_info.decrementUnitNumber(amount);

         // Update any proxies referring to the unit that has had its index
         // decremented.
         for ( Iterator i = this.graph.getModel().edges(port); i.hasNext(); )
         {
            Edge edge = (Edge) i.next();
            DefaultPort proxy_port = (DefaultPort) edge.getSource();
            DefaultGraphCell proxy_cell =
               (DefaultGraphCell) proxy_port.getParent();
            ProxyInfo proxy_info = (ProxyInfo) proxy_cell.getUserObject();
            proxy_info.getElement().setProperty(UNIT_PROPERTY, 0,
                                                unit_info.getUnitNumber(),
                                                proxy_info.getContext());
         }
      }

      private transient CellView   mView       = null;
      private transient DeviceInfo mDeviceInfo = null;
      private transient List       mStations   = new ArrayList();

      private TableLayout mMainLayout = null;

      private JButton mAddStationButton = new JButton();

      /**
       * This variable is used for a hack to communicate information between
       * removeUnit() and propertyValueChanged() indirectly.
       */
      private transient UnitInfo mRemovedUnitInfo = null;

      /**
       * A stack of all the unit rows added to this renderer dynamically
       * through user interaction.  The stack contains objects of type
       * <code>UnitRow</code>.  This is used in conjunction with the
       * mRemovedUnitInfo hack.
       */
      private transient Stack mAddedUnitRows = new Stack();
   }
}

/**
 * A helper class used as the value in <code>mStationGroups</code> that
 * assists with calculating unit numbers when adding and removing unit
 * rows from the layout.
 */
class StationGroup
{
   StationGroup(JPanel stationPanel, TableLayout mainLayout,
                JButton removeStationButton, JLabel posUnitLabel,
                JButton anaAddButton, JButton digAddButton)
   {
      this.stationPanel        = stationPanel;
      this.mainLayout          = mainLayout;
      this.removeStationButton = removeStationButton;

      this.positionUnitLabel = posUnitLabel;

      this.analogAddButton   = anaAddButton;
      this.digitalAddButton  = digAddButton;
   }

   public void addUnit(UnitInfo unitInfo)
   {
      this.units.add(unitInfo);

      if ( unitInfo.getUnitType().equals(UnitConstants.ANALOG) )
      {
         analogUnitCount++;
      }
      else
      {
         digitalUnitCount++;
      }
   }

   public void removeUnit(UnitInfo unitInfo)
   {
      this.units.remove(unitInfo);

      if ( unitInfo.getUnitType().equals(UnitConstants.ANALOG) )
      {
         analogUnitCount--;
      }
      else
      {
         digitalUnitCount--;
      }
   }

   /**
    * Returns the list of all analog and digital units in this station.
    */
   public List getUnits()
   {
      return this.units;
   }

   /**
    * Returns the number of units in this station of the given type (as
    * defined in <code>UnitConstants</code>.  The type value must be either
    * <code>UnitConstants.ANALOG</code> or <code>UnitConstants.DIGITAL</code>.
    * All stations always have exactly one positional unit.
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.UnitConstants
    */
   public int getUnitCount(Integer unitType)
   {
      if ( unitType.equals(UnitConstants.ANALOG) )
      {
         return this.analogUnitCount;
      }
      else
      {
         return this.digitalUnitCount;
      }
   }

   /**
    * Returns the JButton instance for adding a new unit of the given type
    * as defined in <code>UnitConstants</code>.  The type value must be either
    * <code>UnitConstants.ANALOG</code> or <code>UnitConstants.DIGITAL</code>.
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.UnitConstants
    */
   public JButton getAddButton(Integer unitType)
   {
      if ( unitType.equals(UnitConstants.ANALOG) )
      {
         return this.analogAddButton;
      }
      else
      {
         return this.digitalAddButton;
      }
   }

   public JPanel      stationPanel = null;
   public TableLayout mainLayout   = null;

   public JButton removeStationButton = null;

   public JLabel positionUnitLabel = null;

   private JButton analogAddButton  = null;
   private JButton digitalAddButton = null;

   private int analogUnitCount  = 0;
   private int digitalUnitCount = 0;

   /**
    * All the UnitInfo objects associated with this station group.
    */
   private List units = new ArrayList();
}
