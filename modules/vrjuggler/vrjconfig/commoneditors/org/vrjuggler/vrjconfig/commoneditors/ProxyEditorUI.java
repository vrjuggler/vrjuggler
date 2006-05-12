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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.util.*;
import javax.swing.*;

import org.jgraph.JGraph;
import org.jgraph.event.GraphSelectionEvent;
import org.jgraph.event.GraphSelectionListener;
import org.jgraph.graph.AttributeMap;
import org.jgraph.graph.BasicMarqueeHandler;
import org.jgraph.graph.ConnectionSet;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.GraphSelectionModel;
import org.jgraph.graph.PortView;
import org.jgraph.layout.JGraphLayoutAlgorithm;
import org.jgraph.layout.JGraphLayoutRegistry;
import org.jgraph.layout.JGraphLayoutSettings;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.ConfigDefinitionChooser;

import org.vrjuggler.vrjconfig.commoneditors.devicegraph.GraphHelpers;
import org.vrjuggler.vrjconfig.commoneditors.devicegraph.ProxyInfo;


/**
 * Adds a user interface to an instance of <code>DeviceProxyGraphEditor</code>.
 * This is a fully functional panel that acts as a custom editor in its own
 * right.
 *
 * @see DeviceProxyGraphEditor
 */
public class ProxyEditorUI
   extends JPanel
   implements EditorConstants
            , GraphSelectionListener
{
   /**
    * Creates a new user interface wrapper around device/proxy graph editor
    * instance that allows editing of all known types of device and proxy
    * config elements.
    */
   public ProxyEditorUI()
   {
      this(null);
   }

   /**
    * Creates a new user interface wrapper around device/proxy graph editor
    * instance that allows editing of only the given config element types.
    * If <code>allowedTypes</code> is null, then the editor will allow editing
    * of all known types of device and proxy config elements.
    *
    * @param allowedTypes       a list of <code>ConfigDefinition</code>
    *                           objects that identifies which types of device
    *                           and proxy config elements will be edited by
    *                           this editor instance or null to indicate that
    *                           all device and proxy elements will be edited
    */
   public ProxyEditorUI(List allowedTypes)
   {
      deviceProxyEditor = new DeviceProxyGraphEditor(allowedTypes);
      deviceProxyEditor.getGraph().setPortsVisible(true);
      deviceProxyEditor.getGraph().setMarqueeHandler(mMarqueeHandler);

      mEditorPane = new JScrollPane(deviceProxyEditor);

      JGraphLayoutRegistry reg =
         JGraphLayoutRegistry.getSharedJGraphLayoutRegistry();
      List layout_algos = reg.getLayouts();
      mLayoutChooser = new JComboBox(layout_algos.toArray());

      JGraphLayoutAlgorithm cur_algo =
         deviceProxyEditor.getGraphLayoutAlgorithm();

      for ( Iterator i = layout_algos.iterator(); i.hasNext(); )
      {
         Object algo = i.next();
         if ( cur_algo.getClass() == algo.getClass() )
         {
            mLayoutChooser.setSelectedItem(algo);
            break;
         }
      }

      try
      {
         initUI();
         updateLayoutUI(cur_algo);

         mSelectButton.setSelected(mMarqueeHandler.isSelectionEnabled());
         mPortToggleButton.setSelected(
            deviceProxyEditor.getGraph().isPortsVisible()
         );

         ButtonGroup group = new ButtonGroup();
         group.add(mSelectButton);
         group.add(mConnectTglButton);
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }

      try
      {
         String img_base = "org/vrjuggler/vrjconfig/commoneditors/images";
         ClassLoader loader = getClass().getClassLoader();

         mAddButton.setIcon(
            new ImageIcon(loader.getResource(img_base + "/Add16.gif"))
         );
         mRemoveButton.setIcon(
            new ImageIcon(loader.getResource(img_base + "/Remove16.gif"))
         );
         mSelectButton.setIcon(
            new ImageIcon(loader.getResource(img_base + "/Select16.gif"))
         );
         mConnectTglButton.setIcon(
            new ImageIcon(loader.getResource(img_base + "/Arrow16.gif"))
         );
         mDisconnectButton.setIcon(
            new ImageIcon(loader.getResource(img_base + "/Disconnect16.gif"))
         );
         mPortToggleButton.setIcon(
            new ImageIcon(loader.getResource(img_base + "/Ports16.gif"))
         );
         mLayoutButton.setIcon(
            new ImageIcon(loader.getResource(img_base + "/Refresh16.gif"))
         );
         mLayoutCfgButton.setIcon(
            new ImageIcon(loader.getResource(img_base + "/Preferences16.gif"))
         );
      }
      catch (NullPointerException ex)
      {
         System.err.println("WARNING: " + ex.getMessage());
         mAddButton.setIcon(null);
         mAddButton.setText("Add ...");
         mRemoveButton.setIcon(null);
         mRemoveButton.setText("Remove");
         mSelectButton.setIcon(null);
         mSelectButton.setText("Select");
         mConnectTglButton.setIcon(null);
         mConnectTglButton.setText("Connect");
         mDisconnectButton.setIcon(null);
         mDisconnectButton.setText("Disconnect");
         mPortToggleButton.setIcon(null);
         mPortToggleButton.setText("Show Ports");
         mLayoutButton.setIcon(null);
         mLayoutButton.setText("Refresh Layout");
         mLayoutCfgButton.setIcon(null);
         mLayoutCfgButton.setText("Configure Layout");
      }

      deviceProxyEditor.getGraph().addGraphSelectionListener(this);
   }

   /**
    * Initializes the contained <code>DeviceProxyGraphEditor</code> instance
    * using the given configuration information.  If <code>proxyElt</code> is
    * null, the device/proxy graph editor is initialized to edit <i>all</i>
    * device and proxy elements in the given context.
    *
    * @param ctx        the context containing the given config element and
    *                   any devices to which that config element may refer
    * @param proxyElt   the config element for the single proxy that will be
    *                   edited in this editor instance (it must be of type
    *                   <code>EditorConstants.PROXY_TYPE</code>) or null to
    *                   indicate that all devices and proxies should be
    *                   edited
    */
   public void setConfig(ConfigContext ctx, ConfigElement proxyElt)
   {
      mContext = ctx;
      deviceProxyEditor.setConfig(ctx, proxyElt);
   }

   /**
    * Clean up after ourself.  In particular, this is intended to break
    * circular object references.
    */
   public void editorClosing()
   {
      deviceProxyEditor.getGraph().removeGraphSelectionListener(this);
      deviceProxyEditor.editorClosing();
      mAddButton.removeActionListener(mAddAdapter);
      mRemoveButton.removeActionListener(mRemoveAdapter);
      mSelectButton.removeActionListener(mSelectAdapter);
      mConnectTglButton.removeActionListener(mConnectTglAdapter);
      mDisconnectButton.removeActionListener(mDisconnectAdapter);
      mPortToggleButton.removeActionListener(mPortToggleAdapter);
      mLayoutButton.removeActionListener(mLayoutBtnAdapter);
      mLayoutCfgButton.removeActionListener(mLayoutBtnCfgAdapter);
      mLayoutChooser.removeActionListener(mLayoutChooserAdapter);
      mContext = null;
      mMarqueeHandler = null;
      mAddAdapter = null;
      mRemoveAdapter = null;
      mSelectAdapter = null;
      mConnectTglAdapter = null;
      mDisconnectAdapter = null;
      mPortToggleAdapter = null;
      mLayoutBtnAdapter = null;
      mLayoutChooserAdapter = null;
      deviceProxyEditor = null;
      mContext = null;
      mAddButton = null;
      mRemoveButton = null;
      mLayoutButton = null;
      mLayoutChooser = null;
      mLayoutCfgButton = null;
      mButtonBar = null;
      mEditorPane = null;
   }

   /**
    * Returns the contained device/proxy graph editor instance.
    */
   public DeviceProxyGraphEditor getDeviceProxyEditor()
   {
      return this.deviceProxyEditor;
   }

   /**
    * Handles changes in the graph selection.
    */
   public void valueChanged(GraphSelectionEvent e)
   {
      int count =
         deviceProxyEditor.getGraph().getSelectionModel().getSelectionCount();
      mRemoveButton.setEnabled(count > 0);

      Object[] cells = deviceProxyEditor.getGraph().getSelectionCells();

      if ( cells.length == 2 )
      {
         // Only enable the "Disconnect" button if the two selected cells are
         // already connected.
         mDisconnectButton.setEnabled(
            deviceProxyEditor.isConnected(cells[0], cells[1])
         );
      }
      else
      {
         mDisconnectButton.setEnabled(false);
      }
   }

   private void initUI()
      throws Exception
   {
      mAddButton.setEnabled(true);
      mAddButton.addActionListener(mAddAdapter);
      mAddButton.setToolTipText("Add a new device or proxy");
      mRemoveButton.setEnabled(false);
      mRemoveButton.addActionListener(mRemoveAdapter);
      mRemoveButton.setToolTipText("Remove the selected item(s)");
      mSelectButton.setEnabled(true);
      mSelectButton.addActionListener(mSelectAdapter);
      mSelectButton.setToolTipText("Enable/disable selection mode");
      mConnectTglButton.setEnabled(true);
      mConnectTglButton.addActionListener(mConnectTglAdapter);
      mConnectTglButton.setToolTipText("Enable/disable connection mode");
      mDisconnectButton.setEnabled(false);
      mDisconnectButton.addActionListener(mDisconnectAdapter);
      mDisconnectButton.setToolTipText(
         "Disconnect the selected proxy from the selected device"
      );
      mPortToggleButton.setEnabled(true);
      mPortToggleButton.addActionListener(mPortToggleAdapter);
      mLayoutButton.setEnabled(true);
      mLayoutButton.addActionListener(mLayoutBtnAdapter);
      mLayoutButton.setToolTipText(
         "Refresh the layout of the graph or the selected vertices"
      );
      mLayoutChooser.setRenderer(new LayoutAlgorithmRenderer());
      mLayoutChooser.addActionListener(mLayoutChooserAdapter);
      mLayoutCfgButton.setEnabled(false);
      mLayoutCfgButton.addActionListener(mLayoutBtnCfgAdapter);
      mLayoutCfgButton.setToolTipText("Configure the selected layout");
      mButtonBar.add(mAddButton);
      mButtonBar.add(mRemoveButton);
      mButtonBar.addSeparator();
      mButtonBar.add(mSelectButton);
      mButtonBar.add(mConnectTglButton);
      mButtonBar.add(mDisconnectButton);
      mButtonBar.addSeparator();
      mButtonBar.add(mPortToggleButton);
      mButtonBar.add(mLayoutButton);
      mButtonBar.add(mLayoutChooser);
      mButtonBar.add(mLayoutCfgButton);
      mToolbarPanel.setLayout(new BorderLayout());
      mToolbarPanel.add(mButtonBar, BorderLayout.CENTER);
      this.setLayout(new BorderLayout());
      this.add(mToolbarPanel, BorderLayout.NORTH);
      this.add(mEditorPane, BorderLayout.CENTER);
   }

   private void updateLayoutUI(JGraphLayoutAlgorithm algo)
   {
      if ( algo == null )
      {
         mLayoutButton.setEnabled(false);
         mLayoutCfgButton.setEnabled(false);

         if ( mToolbarPanel.isAncestorOf(mLayoutHintLabel) )
         {
            mToolbarPanel.remove(mLayoutHintLabel);
            mToolbarPanel.revalidate();
            mToolbarPanel.repaint();
         }
      }
      else
      {
         mLayoutButton.setEnabled(true);
         mLayoutCfgButton.setEnabled(algo.createSettings() != null);

         String hint = algo.getHint();

         if ( hint != null && ! hint.equals("") )
         {
            mLayoutHintLabel.setText("<html><b>Layout hint</b>: " + hint +
                                     "</html>");

            if ( ! mToolbarPanel.isAncestorOf(mLayoutHintLabel) )
            {
               mToolbarPanel.add(mLayoutHintLabel, BorderLayout.SOUTH);
               mToolbarPanel.revalidate();
               mToolbarPanel.repaint();
            }
         }
         else
         {
            if ( mToolbarPanel.isAncestorOf(mLayoutHintLabel) )
            {
               mToolbarPanel.remove(mLayoutHintLabel);
               mToolbarPanel.revalidate();
               mToolbarPanel.repaint();
            }
         }
      }
   }

   private ConfigContext mContext = null;
   private MarqueeHandler mMarqueeHandler = new MarqueeHandler();

   private ProxyEditorUI_mAddButton_actionAdapter mAddAdapter =
      new ProxyEditorUI_mAddButton_actionAdapter(this);
   private ProxyEditorUI_mRemoveButton_actionAdapter mRemoveAdapter =
      new ProxyEditorUI_mRemoveButton_actionAdapter(this);
   private ProxyEditorUI_mSelectButton_actionAdapter mSelectAdapter =
      new ProxyEditorUI_mSelectButton_actionAdapter(this);
   private ProxyEditorUI_mConnectTglButton_actionAdapter mConnectTglAdapter =
      new ProxyEditorUI_mConnectTglButton_actionAdapter(this);
   private ProxyEditorUI_mDisconnectButton_actionAdapter mDisconnectAdapter =
      new ProxyEditorUI_mDisconnectButton_actionAdapter(this);
   private ProxyEditorUI_mPortToggleButton_actionAdapter mPortToggleAdapter =
      new ProxyEditorUI_mPortToggleButton_actionAdapter(this);
   private ProxyEditorUI_mLayoutButton_actionAdapter mLayoutBtnAdapter =
      new ProxyEditorUI_mLayoutButton_actionAdapter(this);
   private ProxyEditorUI_mLayoutChooser_actionAdapter mLayoutChooserAdapter =
      new ProxyEditorUI_mLayoutChooser_actionAdapter(this);
   private ProxyEditorUI_mLayoutCfgButton_actionAdapter mLayoutBtnCfgAdapter =
      new ProxyEditorUI_mLayoutCfgButton_actionAdapter(this);

   private JPanel mToolbarPanel = new JPanel();
   private JLabel mLayoutHintLabel = new JLabel();
   private JToolBar mButtonBar = new JToolBar();
   private JButton mAddButton = new JButton();
   private JButton mRemoveButton = new JButton();
   private JToggleButton mSelectButton = new JToggleButton();
   private JToggleButton mConnectTglButton = new JToggleButton();
   private JButton mDisconnectButton = new JButton();
   private JToggleButton mPortToggleButton = new JToggleButton();
   private JButton mLayoutButton = new JButton();
   private JComboBox mLayoutChooser = null;
   private JButton mLayoutCfgButton = new JButton();
   private DeviceProxyGraphEditor deviceProxyEditor = null;
   private JScrollPane mEditorPane = null;

   private class MarqueeHandler
      extends BasicMarqueeHandler
   {
      public MarqueeHandler()
      {
         setSelectionEnabled(true);
      }

      public void setSelectionEnabled(boolean enabled)
      {
         this.selectionEnabled = enabled;
         if ( enabled )
         {
            this.connectEnabled = false;
         }
      }

      public boolean isSelectionEnabled()
      {
         return this.selectionEnabled;
      }

      public void setConnectEnabled(boolean enabled)
      {
         this.connectEnabled = enabled;
         if ( enabled )
         {
            this.selectionEnabled = false;
         }
      }

      public boolean isForceMarqueeEvent(MouseEvent e)
      {
         return ! this.selectionEnabled || super.isForceMarqueeEvent(e);
      }

      public void mouseDragged(MouseEvent event)
      {
         if ( ! event.isConsumed() && ! this.selectionEnabled )
         {
            DeviceGraph graph = deviceProxyEditor.getGraph();

            Graphics g = graph.getGraphics();
            Color bg = graph.getBackground(), fg = Color.black;
            g.setColor(fg);
            g.setXORMode(bg);

            PortView new_port = graph.getPortViewAt(event.getX(),
                                                    event.getY());

            overlay(g, true);

            mCurrentPoint = graph.snap(event.getPoint());

            if ( this.connectEnabled && new_port != null &&
                 new_port != mFirstPort )
            {
               mCurrentPoint = graph.toScreen(new_port.getLocation(null));
            }

            if ( mStartPoint != null && mCurrentPoint != null )
            {
               if ( (new_port != mPort || new_port == null ||
                     new_port == mFirstPort) && this.connectEnabled )
               {
                  mPort = new_port;
               }

               Point temp_start = new Point((int) mStartPoint.getX(),
                                            (int) mStartPoint.getY());
               Point temp_cur   = new Point((int) mCurrentPoint.getX(),
                                            (int) mCurrentPoint.getY());

               mBounds =
                  new Rectangle(temp_start).union(new Rectangle(temp_cur));

               g.setColor(bg);
               g.setXORMode(fg);
            }

            overlay(g, false);
            event.consume();
         }

         if ( ! event.isConsumed() )
         {
            super.mouseDragged(event);
            event.consume();
         }
      }

      public void mouseMoved(MouseEvent event)
      {
         if ( ! event.isConsumed() && ! this.selectionEnabled )
         {
            DeviceGraph graph = deviceProxyEditor.getGraph();
            graph.setCursor(new Cursor(Cursor.CROSSHAIR_CURSOR));
            event.consume();

            if ( this.connectEnabled )
            {
               PortView old_port = mPort;
               PortView new_port = graph.getPortViewAt(event.getX(),
                                                       event.getY());

               // mPort can only be set to the value of new_port if new_port
               // is the port of a proxy cell and that proxy cell does not
               // already have any outgoing edges.
               boolean can_connect = true;
               if ( new_port != null )
               {
                  DefaultGraphCell parent =
                     (DefaultGraphCell) ((DefaultPort) new_port.getCell()).getParent();

                  if ( parent.getUserObject() instanceof ProxyInfo )
                  {
                     Object[] edges =
                        DefaultGraphModel.getOutgoingEdges(graph.getModel(), 
                                                           parent);
                     can_connect = (edges == null || edges.length == 0);
                  }
                  else
                  {
                     can_connect = false;
                  }
               }

               // If old_port and new_port are not the same and a connect
               // is allowed, then highlight the port (or its vertex) and set
               // mPort.
               if ( old_port != new_port && can_connect )
               {
                  Graphics g = graph.getGraphics();
                  Color bg = graph.getBackground();
                  Color fg = graph.getMarqueeColor();
                  g.setColor(fg);
                  g.setXORMode(bg);
                  overlay(g, true);
                  mPort = new_port;
                  g.setColor(bg);
                  g.setXORMode(fg);
                  overlay(g, false);
               }
            }
         }

         super.mouseMoved(event);
      }

      public void mousePressed(MouseEvent event)
      {
         if ( ! event.isConsumed() && ! this.selectionEnabled )
         {
            DeviceGraph graph = deviceProxyEditor.getGraph();
            mStartPoint = graph.snap(event.getPoint());
            mFirstPort  = mPort;

            if ( mPort != null )
            {
               mStartPoint = graph.toScreen(mPort.getLocation(null));
            }

            if ( this.connectEnabled && mFirstPort != null )
            {
               mStartPoint = graph.toScreen(mFirstPort.getLocation(null));
            }

            event.consume();
         }

         super.mousePressed(event);
         event.consume();
      }

      public void mouseReleased(MouseEvent event)
      {
         if ( ! event.isConsumed() && ! this.selectionEnabled )
         {
            if ( this.connectEnabled )
            {
               if ( mFirstPort != null && mPort != null )
               {
                  Map map = new AttributeMap();
                  ConnectionSet cs = new ConnectionSet();

                  DefaultPort proxy_port  = (DefaultPort) mFirstPort.getCell();
                  DefaultPort device_port = (DefaultPort) mPort.getCell();
                  DefaultEdge edge =
                     GraphHelpers.connectProxyToDevice(proxy_port, device_port,
                                                       cs, map);

                  DeviceGraph graph = deviceProxyEditor.getGraph();
                  graph.getGraphLayoutCache().insert(new Object[]{edge},
                                                     map, cs, null, null);
               }

               event.consume();
            }
         }

         if ( event.isShiftDown() && event.isControlDown() )
         {
            mSelectButton.doClick();
         }

         if ( ! event.isConsumed() )
         {
            super.mouseReleased(event);
         }

         mFirstPort    = null;
         mPort         = null;
         mStartPoint   = null;
         mCurrentPoint = null;
         mBounds       = null;
      }

      public void overlay(Graphics g, boolean clear)
      {
         super.overlay(deviceProxyEditor.getGraph(), g, clear);

         highlightPort(deviceProxyEditor.getGraph().getGraphics());

         if ( mBounds != null && mStartPoint != null )
         {
            if ( this.connectEnabled && mCurrentPoint != null )
            {
               g.drawLine((int) mStartPoint.getX(), (int) mStartPoint.getY(),
                          (int) mCurrentPoint.getX(),
                          (int) mCurrentPoint.getY());
            }
         }
      }

      private void highlightPort(Graphics g)
      {
         if ( mPort != null )
         {
            DeviceGraph graph = deviceProxyEditor.getGraph();

            boolean offset =
               (GraphConstants.getOffset(mPort.getAllAttributes()) != null);

            Rectangle2D r = (offset ? mPort.getBounds()
                                    : mPort.getParentView().getBounds());
            r = graph.toScreen(new Rectangle(r.getBounds()));

            int pad = 3;
            ((Rectangle) r).translate(-pad, -pad);
            r.setRect((int) r.getX(), (int) r.getY(),
                      (int) (r.getWidth() + 2 * pad),
                      (int) (r.getHeight() + 2 * pad));

            graph.getUI().paintCell(g, mPort, r, true);
         }
      }

      private boolean selectionEnabled = false;
      private boolean connectEnabled   = false;

      /**
       * The bounding box created when dragging the mouse pointer on the
       * marquee.
       */
      private Rectangle2D mBounds = null;

      /**
       * Used to keep track of the starting point for dragging a new edge
       * between two ports.
       */
      private Point2D mStartPoint = null;

      /**
       * Used to keep track of the current pointer location when dragging a
       * new edge between two ports.
       */
      private Point2D mCurrentPoint = null;

      private PortView mPort = null;

      /**
       * The first port selected when dragging a new edge between two ports.
       */
      private PortView mFirstPort = null;
   }

   private static class LayoutAlgorithmRenderer
      extends JLabel
      implements ListCellRenderer
   {
      private static String pkgPrefix = "org.jgraph.layout.";

      public LayoutAlgorithmRenderer()
      {
         setOpaque(true);
         setHorizontalAlignment(LEFT);
         setVerticalAlignment(CENTER);
      }

      public Component getListCellRendererComponent(JList list, Object value,
                                                    int index,
                                                    boolean isSelected,
                                                    boolean cellHasFocus)
      {
         if ( isSelected )
         {
            setBackground(list.getSelectionBackground());
            setForeground(list.getSelectionForeground());
         }
         else
         {
            setBackground(list.getBackground());
            setForeground(list.getForeground());
         }

         String full_class_name = value.getClass().getName();
         String class_name = full_class_name.substring(pkgPrefix.length());
         setFont(list.getFont());
         setText(class_name);

         return this;
      }
   }

   void mAddButton_actionPerformed(ActionEvent actionEvent)
   {
      ConfigDefinitionChooser chooser = new ConfigDefinitionChooser();
      chooser.setDefinitions(deviceProxyEditor.getAllowedTypes());

      Window parent = (Window) SwingUtilities.getAncestorOfClass(Window.class,
                                                                 this);

      if ( chooser.showDialog(parent) == ConfigDefinitionChooser.APPROVE_OPTION )
      {
         ConfigDefinition selected_def = chooser.getSelectedDefinition();

         ConfigBroker broker = new ConfigBrokerProxy();
         List all_defs = broker.getRepository().getAllLatest();

         ConfigElementFactory factory = new ConfigElementFactory(all_defs);
         broker.add(mContext, factory.createUnique(selected_def, mContext));
      }
   }

   void mRemoveButton_actionPerformed(ActionEvent actionEvent)
   {
      JGraph graph = deviceProxyEditor.getGraph();
      graph.getModel().remove(graph.getSelectionCells());
      mRemoveButton.setEnabled(false);
   }

   void mSelectButton_actionPerformed(ActionEvent actionEvent)
   {
      if ( mSelectButton.getIcon() == null )
      {
         mSelectButton.setText(
            mSelectButton.isSelected() ? "Disable selection"
                                       : "Enable selection"
         );
      }

      mMarqueeHandler.setSelectionEnabled(mSelectButton.isSelected());
   }

   void mConnectTglButton_actionPerformed(ActionEvent actionEvent)
   {
      if ( mConnectTglButton.getIcon() == null )
      {
         mConnectTglButton.setText(
            mConnectTglButton.isSelected() ? "Disable connecting"
                                           : "Enable connecting"
         );
      }

      mMarqueeHandler.setConnectEnabled(mConnectTglButton.isSelected());
   }

   void mDisconnectButton_actionPerformed(ActionEvent actionEvent)
   {
      Object[] cells = deviceProxyEditor.getGraph().getSelectionCells();
      deviceProxyEditor.disconnect(cells[0], cells[1]);
      mDisconnectButton.setEnabled(false);
   }

   void mPortToggleButton_actionPerformed(ActionEvent actionEvent)
   {
      if ( mPortToggleButton.getIcon() == null )
      {
         mPortToggleButton.setText(
            mPortToggleButton.isSelected() ? "Hide Ports"
                                           : "Show Ports"
         );
      }

      if ( mPortToggleButton.isSelected() )
      {
         mPortToggleButton.setToolTipText("Hide vertex ports");
         deviceProxyEditor.getGraph().setPortsVisible(true);
      }
      else
      {
         mPortToggleButton.setToolTipText("Show vertex ports");
         deviceProxyEditor.getGraph().setPortsVisible(false);
      }
   }

   void mLayoutButton_actionPerformed(ActionEvent actionEvent)
   {
      mLayoutButton.setEnabled(false);
      final DeviceProxyGraphEditor editor = deviceProxyEditor;
      final JGraphLayoutAlgorithm controller =
         deviceProxyEditor.getGraphLayoutAlgorithm();

      JDialog parent =
         (JDialog) SwingUtilities.getAncestorOfClass(JDialog.class, this);
      final ProgressMonitor monitor =
         new ProgressMonitor(parent, "Applying layout to graph...", "", 0,
                             controller.getMaximumProgress());
      monitor.setProgress(0);
      monitor.setMillisToDecideToPopup(1000);

      ActionListener l = new ActionListener()
         {
           public void actionPerformed(ActionEvent e)
           {
              monitor.setProgress(controller.getProgress());
              controller.setAllowedToRun(! monitor.isCanceled());
           }
         };
      final javax.swing.Timer updater = new javax.swing.Timer(1000, l);
            
      Thread t = new Thread()
         {
            public void run()
            {
               try
               {
                  GraphSelectionModel sel_model =
                     editor.getGraph().getSelectionModel();

                  if ( sel_model.getSelectionCount() > 0 )
                  {
                     editor.applyGraphLayoutAlgorithm(
                        editor.getGraph().getSelectionCells()
                     );
                  }
                  else
                  {
                     editor.applyGraphLayoutAlgorithm();
                  }
               }
               finally
               {
                  monitor.close();
                  updater.stop();
                  mLayoutButton.setEnabled(true);
               }
            }
         };
      t.start();
   }

   void mLayoutChooser_actionPerformed(ActionEvent actionEvent)
   {
      JGraphLayoutAlgorithm cur_algo =
         (JGraphLayoutAlgorithm) mLayoutChooser.getSelectedItem();
      deviceProxyEditor.setGraphLayoutAlgorithm(cur_algo);
      updateLayoutUI(cur_algo);
   }

   void mLayoutCfgButton_actionPerformed(ActionEvent actionEvent)
   {
      JGraphLayoutAlgorithm algo = deviceProxyEditor.getGraphLayoutAlgorithm();

      JGraphLayoutSettings settings = algo.createSettings();
      if ( settings != null )
      {
         JDialog parent =
            (JDialog) SwingUtilities.getAncestorOfClass(JDialog.class, this);
         System.out.println("parent: " + parent);
         JDialog dialog =
            JGraphLayoutAlgorithm.createDialog(settings, parent,
                                               "Configure Layout Settings",
                                               "Cancel", "Apply");
         dialog.setModal(true);
         dialog.pack();
         dialog.setLocationRelativeTo(parent);
         dialog.setVisible(true);
      }
   }
}

class ProxyEditorUI_mAddButton_actionAdapter
   implements ActionListener 
{
   private ProxyEditorUI adaptee;

   public ProxyEditorUI_mAddButton_actionAdapter(ProxyEditorUI adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mAddButton_actionPerformed(actionEvent);
   }
}

class ProxyEditorUI_mRemoveButton_actionAdapter
   implements ActionListener 
{
   private ProxyEditorUI adaptee;

   public ProxyEditorUI_mRemoveButton_actionAdapter(ProxyEditorUI adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mRemoveButton_actionPerformed(actionEvent);
   }
}

class ProxyEditorUI_mSelectButton_actionAdapter
   implements ActionListener 
{
   private ProxyEditorUI adaptee;

   public ProxyEditorUI_mSelectButton_actionAdapter(ProxyEditorUI adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mSelectButton_actionPerformed(actionEvent);
   }
}

class ProxyEditorUI_mConnectTglButton_actionAdapter
   implements ActionListener 
{
   private ProxyEditorUI adaptee;

   public ProxyEditorUI_mConnectTglButton_actionAdapter(ProxyEditorUI adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mConnectTglButton_actionPerformed(actionEvent);
   }
}

class ProxyEditorUI_mDisconnectButton_actionAdapter
   implements ActionListener 
{
   private ProxyEditorUI adaptee;

   public ProxyEditorUI_mDisconnectButton_actionAdapter(ProxyEditorUI adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mDisconnectButton_actionPerformed(actionEvent);
   }
}

class ProxyEditorUI_mPortToggleButton_actionAdapter
   implements ActionListener 
{
   private ProxyEditorUI adaptee;

   public ProxyEditorUI_mPortToggleButton_actionAdapter(ProxyEditorUI adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mPortToggleButton_actionPerformed(actionEvent);
   }
}

class ProxyEditorUI_mLayoutButton_actionAdapter
   implements ActionListener 
{
   private ProxyEditorUI adaptee;

   public ProxyEditorUI_mLayoutButton_actionAdapter(ProxyEditorUI adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mLayoutButton_actionPerformed(actionEvent);
   }
}

class ProxyEditorUI_mLayoutChooser_actionAdapter
   implements ActionListener 
{
   private ProxyEditorUI adaptee;

   public ProxyEditorUI_mLayoutChooser_actionAdapter(ProxyEditorUI adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mLayoutChooser_actionPerformed(actionEvent);
   }
}

class ProxyEditorUI_mLayoutCfgButton_actionAdapter
   implements ActionListener 
{
   private ProxyEditorUI adaptee;

   public ProxyEditorUI_mLayoutCfgButton_actionAdapter(ProxyEditorUI adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mLayoutCfgButton_actionPerformed(actionEvent);
   }
}
