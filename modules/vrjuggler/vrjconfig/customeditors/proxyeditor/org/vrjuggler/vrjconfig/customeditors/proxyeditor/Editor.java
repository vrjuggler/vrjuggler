/*
 * @(#)Editor.java	1.0.5 06/01/03
 *
 * Copyright (C) 2002 Gaudenz Alder
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

// package graphed;

package org.vrjuggler.vrjconfig.customeditors.proxyeditor;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.net.URL;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Map;
import java.util.Properties;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;
import javax.swing.event.UndoableEditEvent;

import com.jgraph.layout.*;

import com.jgraph.JGraph;
import com.jgraph.event.GraphSelectionEvent;
import com.jgraph.event.GraphSelectionListener;
import com.jgraph.graph.BasicMarqueeHandler;
import com.jgraph.graph.CellMapper;
import com.jgraph.graph.CellView;
import com.jgraph.graph.ConnectionSet;
import com.jgraph.graph.DefaultEdge;
import com.jgraph.graph.DefaultGraphCell;
import com.jgraph.graph.DefaultGraphModel;
import com.jgraph.graph.DefaultPort;
import com.jgraph.graph.Edge;
import com.jgraph.graph.EdgeView;
import com.jgraph.graph.GraphConstants;
import com.jgraph.graph.GraphModel;
import com.jgraph.graph.GraphUndoManager;
import com.jgraph.graph.ParentMap;
import com.jgraph.graph.Port;
import com.jgraph.graph.PortView;
import org.vrjuggler.jccl.editors.CustomEditor;
import org.vrjuggler.jccl.editors.CustomEditorRegistry;
import org.vrjuggler.jccl.config.*;

public class Editor
	extends JPanel
	implements GraphSelectionListener, KeyListener, CustomEditor 
{

   public void setConfigElement(ConfigElement elm)
   {
      String token = elm.getDefinition().getToken();
      if(token.equals("digital_proxy"))
      {
         ElementNode digital = new ElementNode(new Point(0,0), elm);
         //ProxyCell device = new Device(new Point(100, 100), elm.getProperty("device", 0).toString());
         insert(digital);
         //insert(device);
         //connect(digital.getPort(), device.getPort());
      }
      else if(token.equals("analog_proxy"))
      {
         ElementNode analog = new ElementNode(new Point(0,0), elm);
         //ProxyCell device = new Device(new Point(100, 100), elm.getProperty("device", 0).toString());
         insert(analog);
         //insert(device);
         //connect(analog.getPort(), device.getPort());
      }
      else if(token.equals("position_proxy"))
      {
         ElementNode position = new ElementNode(new Point(0,0), elm);
         //ProxyCell device = new Device(new Point(100, 100), elm.getProperty("device", 0).toString());
         insert(position);
         //insert(device);
         //connect(position.getPort(), device.getPort());
      }
      else if(token.equals("event_window_proxy"))
      {
         ElementNode event_window = new ElementNode(new Point(0,0), elm);
         //ProxyCell device = new Device(new Point(100, 100), elm.getProperty("device", 0).toString());
         insert(event_window);
         //insert(device);
         //connect(event_window.getPort(), device.getPort());
      }
      else if(token.equals("gesture_proxy"))
      {
         ElementNode gesture = new ElementNode(new Point(0,0), elm);
         //ProxyCell device = new Device(new Point(100, 100), elm.getProperty("device", 0).toString());
         insert(gesture);
         //insert(device);
         //connect(gesture.getPort(), device.getPort());
      }
      else if(token.equals("glove_proxy"))
      {
         ElementNode glove = new ElementNode(new Point(0,0), elm);
         //ProxyCell device = new Device(new Point(100, 100), elm.getProperty("device", 0).toString());
         insert(glove);
         //insert(device);
         //connect(glove.getPort(), device.getPort());
      }
      else if(token.equals("alias"))
      {
         ElementNode alias = new ElementNode(new Point(0,0), elm);
         //ProxyCell device = new Device(new Point(100, 100), elm.getProperty("proxy", 0).toString());
         insert(alias);
         //insert(device);
         //connect(alias.getPort(), device.getPort());
      }
   }

   public Container getPanel()
   {
      return this;
   }

   public String getTitle()
   {
      return("Proxy Editor");
   }

   public void setContext(ConfigContext ctx)
   {
      // Create the model
      graph.setModel(new ProxyModelFactory(ctx).getModel());

      // Create a graph layout controller
      //final LayoutController controller = new SpringEmbeddedLayoutController();
      //final LayoutController controller = new AnnealingLayoutController();
      //final LayoutController controller = new   SugiyamaLayoutController();
      //final LayoutController controller = new GEMLayoutController();
      //final LayoutController controller = new TreeLayoutController();
      
   }
   
   private LayoutController controller = new SpringEmbeddedLayoutController();
   private boolean mApplyToAll = true;
   private Properties mLayoutConfig = controller.getConfiguration();
   
   
   private void sort()
   {
      final LayoutDialog diag = new LayoutDialog(new javax.swing.JFrame());
      diag.setVisible(true); 
      if(!diag.isCanceled())
      { 
         controller = diag.getSelectedLayoutController();
         mApplyToAll = diag.isApplyLayoutToAll();
         mLayoutConfig = controller.getConfiguration();
         doSort(); 
      }
   }

   private void doSort()
   {
      final boolean test = mApplyToAll;
      final Properties testing = mLayoutConfig;
          // Start the layout algorithm in a seoerate thread.
         Thread t = new Thread("Layout Algorithm " + controller.toString()) 
            {
               public void run() 
               {
	          LayoutAlgorithm algorithm = controller.getLayoutAlgorithm();
                  algorithm.perform(graph,
                        test,
                        testing);
	       }
	    };
         t.start();
   }

   
   // JGraph instance
   protected JGraph graph;

   // Undo Manager
   protected GraphUndoManager undoManager;

   // Actions which Change State
   protected Action undo,
      redo,
      remove,
      group,
      ungroup,
      tofront,
      toback,
      cut,
      copy,
      paste;

   //
   // Main
   //

   // Main Method
   public static void main(String[] args) 
   {
      // Construct Frame
      JFrame frame = new JFrame("GraphEd");
      // Set Close Operation to Exit
      //frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      // Add an Editor Panel
      frame.getContentPane().add(new Editor());
      // Fetch URL to Icon Resource
      URL jgraphUrl = Editor.class.getClassLoader().getResource("jgraph.gif");
      // If Valid URL
      if (jgraphUrl != null) 
      {
         // Load Icon
         ImageIcon jgraphIcon = new ImageIcon(jgraphUrl);
         // Use in Window
         frame.setIconImage(jgraphIcon.getImage());
      }

      //REPLACE WITH THIS
      //ImageIcon removeIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/proxyeditor/images/delete.gif"));

      // Set Default Size
      frame.setSize(520, 390);
      // Show Frame
      frame.show();
   }

   //
   // Editor Panel
   //

   // Construct an Editor Panel
   public Editor() 
   {
      CustomEditorRegistry.registerEditor("digital_proxy", Editor.class);
      CustomEditorRegistry.registerEditor("analog_proxy", Editor.class);
      CustomEditorRegistry.registerEditor("position_proxy", Editor.class);
      CustomEditorRegistry.registerEditor("event_window_proxy", Editor.class);
      CustomEditorRegistry.registerEditor("gesture_proxy", Editor.class);
      CustomEditorRegistry.registerEditor("glove_proxy", Editor.class);
      CustomEditorRegistry.registerEditor("alias", Editor.class);
      // Use Border Layout
      setLayout(new BorderLayout());
      // Construct the Graph
      graph = new ProxyGraph(new ProxyGraphModel(), new ProxyMarqueeHandler());

      // Construct Command History
      //
      // Create a GraphUndoManager which also Updates the ToolBar
      undoManager = new GraphUndoManager() 
         {
            // Override Superclass
            public void undoableEditHappened(UndoableEditEvent e) 
            {
               // First Invoke Superclass
               super.undoableEditHappened(e);
               // Then Update Undo/Redo Buttons
               updateHistoryButtons();
            }
         };

      // Add Listeners to Graph
      //
      // Register UndoManager with the Model
      graph.getModel().addUndoableEditListener(undoManager);
      // Update ToolBar based on Selection Changes
      graph.getSelectionModel().addGraphSelectionListener(this);
      // Listen for Delete Keystroke when the Graph has Focus
      graph.addKeyListener(this);

      // Construct Panel
      //
      // Add a ToolBar
      add(createToolBar(), BorderLayout.NORTH);
      // Add the Graph as Center Component
      add(new JScrollPane(graph), BorderLayout.CENTER);
//      addMouseMotionListener(new MouseMotionAdapter()
//         {
//            public void mouseDragged(MouseEvent e)
//            {
//               doSort();
//            }
//         });
   }
   
   //
   // ToolBar
   //
   public JToolBar createToolBar() 
   {
      JToolBar toolbar = new JToolBar();
      toolbar.setFloatable(false);
      // Insert

      //URL insertUrl = getClass().getClassLoader().getResource("insert.gif");
      //ImageIcon insertIcon = new ImageIcon(insertUrl);


      ImageIcon insertIcon = loadIcon("insert.gif");

      toolbar.add(new AbstractAction("", insertIcon) 
            {
               public void actionPerformed(ActionEvent e) 
               {
                  //insert(new Point(10, 10), 0)
                  //insert(new DigitalProxy(new Point(10,10), "Digital"));
               }
            });

      // Toggle Connect Mode
      //URL connectUrl =
      //	getClass().getClassLoader().getResource("connecton.gif");
      //ImageIcon connectIcon = new ImageIcon(connectUrl);
      ImageIcon connectIcon = loadIcon("connecton.gif");
      
      toolbar.add(new AbstractAction("", connectIcon) 
            {
               public void actionPerformed(ActionEvent e) 
               {
                  graph.setPortsVisible(!graph.isPortsVisible());
                  //URL connectUrl;
                  ImageIcon connectIcon = null;
                  ClassLoader loader = getClass().getClassLoader();
                  if (graph.isPortsVisible())
                  {
                     connectIcon = loadIcon("conneton.gif");
                     //connectUrl =
                     //	getClass().getClassLoader().getResource(
                     //		"connecton.gif");
                  }
                  else
                  {
                     connectIcon = loadIcon("connectoff.gif");
                     //connectUrl =
                     //	getClass().getClassLoader().getResource(
                     //		"connectoff.gif");
                     //ImageIcon connectIcon = new ImageIcon(connectUrl);
                  }
                  putValue(SMALL_ICON, connectIcon);
               }
            });

      // Undo
      toolbar.addSeparator();
      //URL undoUrl = getClass().getClassLoader().getResource("undo.gif");
      //ImageIcon undoIcon = new ImageIcon(undoUrl);
      ImageIcon undoIcon = loadIcon("undo.gif");
      undo = new AbstractAction("", undoIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               undo();
            }
         };
      undo.setEnabled(false);
      toolbar.add(undo);

      // Redo
      //URL redoUrl = getClass().getClassLoader().getResource("redo.gif");
      //ImageIcon redoIcon = new ImageIcon(redoUrl);
      ImageIcon redoIcon = loadIcon("redo.gif");
      redo = new AbstractAction("", redoIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               redo();
            }
         };
      redo.setEnabled(false);
      toolbar.add(redo);

      //
      // Edit Block
      //
      toolbar.addSeparator();
      Action action;
      URL url;

      // Copy
      action = graph.getTransferHandler().getCopyAction();
      //url = getClass().getClassLoader().getResource("copy.gif");
      //action.putValue(Action.SMALL_ICON, new ImageIcon(url));
      ImageIcon copyIcon = loadIcon("copy.gif");
      action.putValue(Action.SMALL_ICON, copyIcon);
      toolbar.add(copy = new EventRedirector(action));

      // Paste
      action = graph.getTransferHandler().getPasteAction();
      //url = getClass().getClassLoader().getResource("paste.gif");
      //action.putValue(Action.SMALL_ICON, new ImageIcon(url));
      ImageIcon pasteIcon = loadIcon("paste.gif");
      action.putValue(Action.SMALL_ICON, pasteIcon);
      toolbar.add(paste = new EventRedirector(action));

      // Cut
      action = graph.getTransferHandler().getCutAction();
      //url = getClass().getClassLoader().getResource("cut.gif");
      //action.putValue(Action.SMALL_ICON, new ImageIcon(url));
      ImageIcon cutIcon = loadIcon("cut.gif");
      action.putValue(Action.SMALL_ICON, cutIcon);
      toolbar.add(cut = new EventRedirector(action));

      // Remove
      //URL removeUrl = getClass().getClassLoader().getResource("delete.gif");
      //ImageIcon removeIcon = new ImageIcon(removeUrl);
      ImageIcon removeIcon = loadIcon("delete.gif");
      remove = new AbstractAction("", removeIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               if (!graph.isSelectionEmpty()) 
               {
                  Object[] cells = graph.getSelectionCells();
                  cells = graph.getDescendants(cells);
                  graph.getModel().remove(cells);
               }
            }
         };
      remove.setEnabled(false);
      toolbar.add(remove);

      // Zoom Std
      toolbar.addSeparator();
      //URL zoomUrl = getClass().getClassLoader().getResource("zoom.gif");
      //ImageIcon zoomIcon = new ImageIcon(zoomUrl);
      ImageIcon zoomIcon = loadIcon("zoom.gif");
      toolbar.add(new AbstractAction("", zoomIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               graph.setScale(1.0);
            }
         });
      // Zoom In
      //URL zoomInUrl = getClass().getClassLoader().getResource("zoomin.gif");
      //ImageIcon zoomInIcon = new ImageIcon(zoomInUrl);
      ImageIcon zoomInIcon = loadIcon("zoomin.gif");
      toolbar.add(new AbstractAction("", zoomInIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               graph.setScale(2 * graph.getScale());
            }
         });
      // Zoom Out
      //URL zoomOutUrl = getClass().getClassLoader().getResource("zoomout.gif");
      //ImageIcon zoomOutIcon = new ImageIcon(zoomOutUrl);
      ImageIcon zoomOutIcon = loadIcon("zoomout.gif");
      toolbar.add(new AbstractAction("", zoomOutIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               graph.setScale(graph.getScale() / 2);
            }
         });

      // Group
      toolbar.addSeparator();
      //URL groupUrl = getClass().getClassLoader().getResource("group.gif");
      //ImageIcon groupIcon = new ImageIcon(groupUrl);
      ImageIcon groupIcon = loadIcon("group.gif");
      group = new AbstractAction("", groupIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               //XXX: This should be group, not sort
               //group(graph.getSelectionCells());
               sort();
            }
         };
      group.setEnabled(false);
      toolbar.add(group);

      // Ungroup
      //URL ungroupUrl = getClass().getClassLoader().getResource("ungroup.gif");
      //ImageIcon ungroupIcon = new ImageIcon(ungroupUrl);
      ImageIcon ungroupIcon = loadIcon("ungroup.gif");
      ungroup = new AbstractAction("", ungroupIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               ungroup(graph.getSelectionCells());
            }
         };
      ungroup.setEnabled(false);
      toolbar.add(ungroup);

      // To Front
      toolbar.addSeparator();
      //URL toFrontUrl = getClass().getClassLoader().getResource("tofront.gif");
      //ImageIcon toFrontIcon = new ImageIcon(toFrontUrl);
      ImageIcon toFrontIcon = loadIcon("tofront.gif");
      tofront = new AbstractAction("", toFrontIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               if (!graph.isSelectionEmpty())
               {
                  toFront(graph.getSelectionCells());
               }
            }
         };
      tofront.setEnabled(false);
      toolbar.add(tofront);

      // To Back
      //URL toBackUrl = getClass().getClassLoader().getResource("toback.gif");
      //ImageIcon toBackIcon = new ImageIcon(toBackUrl);
      ImageIcon toBackIcon = loadIcon("toback.gif");
      toback = new AbstractAction("", toBackIcon) 
         {
            public void actionPerformed(ActionEvent e) 
            {
               if (!graph.isSelectionEmpty())
               {
                  toBack(graph.getSelectionCells());
               }
            }
         };
      toback.setEnabled(false);
      toolbar.add(toback);
      return toolbar;
   }

   // Insert a new Vertex at point
   public void insert(ElementNode vertex) 
   {
      // Snap the Point to the Grid
      //point = graph.snap(new Point(point));
      
      Map attrib_map = vertex.getAttributes();
      
      // Construct a Map from cells to Maps (for insert)
      Hashtable attributes = new Hashtable();
      // Associate the Vertex with its Attributes
      attributes.put(vertex, attrib_map);
      
      graph.getGraphLayoutCache().insert(
         new Object[] { vertex },
         attributes,
         null,
         null,
         null);
   }


	// Insert a new Vertex at point
//	public void insert(/*Point point, int type*/ProxyCell vertexold)
/*        {
           Point point = new Point(20,20);
           int type = 0;
               DefaultGraphCell vertex = null;

                switch(type)
                {
                   case 0:
                      {
		// Construct Vertex with no Label
		vertex = new DefaultGraphCell("Digital");
                break;
                      }
                   case 1:
                      {
		// Construct Vertex with no Label
		vertex = new DefaultGraphCell("Analog");
                break;
                      }
                   case 2:
                      {
		// Construct Vertex with no Label
		vertex = new DefaultGraphCell("Position");
                break;
                      }
                   case 3:
                      {
		// Construct Vertex with no Label
		vertex = new DefaultGraphCell("Alias");
                break;
                      }
                }
		// Add one Floating Port
		vertex.add(new ElementPort());
		// Snap the Point to the Grid
		point = graph.snap(new Point(point));
		// Default Size for the new Vertex
		Dimension size = new Dimension(70, 85);
		// Create a Map that holds the attributes for the Vertex
		Map map = GraphConstants.createMap();
		// Add a Bounds Attribute to the Map
		GraphConstants.setBounds(map, new Rectangle(point, size));
		// Add a Border Color Attribute to the Map
		GraphConstants.setBorderColor(map, Color.black);
		// Add a White Background
		GraphConstants.setBackground(map, Color.white);
		// Make Vertex Opaque
		GraphConstants.setOpaque(map, true);
          switch(type)
                {
                   case 0:
                      {
                ImageIcon digital_img = new ImageIcon("/home/aronb/ControlPanelIcons/digital64.jpg");
                GraphConstants.setIcon(map, digital_img);
                break;
                      }
                   case 1:
                      {
                ImageIcon digital_img = new ImageIcon("/home/aronb/ControlPanelIcons/analog64.jpg");
                GraphConstants.setIcon(map, digital_img);
                break;
                      }
                   case 2:
                      {
                ImageIcon digital_img = new ImageIcon("/home/aronb/ControlPanelIcons/position64.jpg");
                GraphConstants.setIcon(map, digital_img);
                break;
                      }
                   case 3:
                      {
                ImageIcon digital_img = new ImageIcon("/home/aronb/ControlPanelIcons/hand164.jpg");
                GraphConstants.setIcon(map, digital_img);
                break;
                      }
                }
                
		// Construct a Map from cells to Maps (for insert)
		Hashtable attributes = new Hashtable();
		// Associate the Vertex with its Attributes
		attributes.put(vertex, map);
		// Insert the Vertex and its Attributes (can also use model)
		graph.getGraphLayoutCache().insert(
			new Object[] { vertex },
			attributes,
			null,
			null,
			null);
	}
*/
   
   

   // Insert a new Edge between source and target
   public void connect(Port source, Port target) 
   {
      // Connections that will be inserted into the Model
      ConnectionSet cs = new ConnectionSet();
      // Construct Edge with no label
      DefaultEdge edge = new DefaultEdge();
      // Create Connection between source and target using edge
      cs.connect(edge, source, target);
      // Create a Map thath holds the attributes for the edge
      Map map = GraphConstants.createMap();
      // Add a Line End Attribute
      GraphConstants.setLineEnd(map, GraphConstants.ARROW_TECHNICAL);
      // Construct a Map from cells to Maps (for insert)
      Hashtable attributes = new Hashtable();
      // Associate the Edge with its Attributes
      attributes.put(edge, map);
      // Insert the Edge and its Attributes
      graph.getGraphLayoutCache().insert(
         new Object[] { edge },
         attributes,
         cs,
         null,
         null);
   }

   // Create a Group that Contains the Cells
   public void group(Object[] cells)
   {
      // Order Cells by View Layering
      cells = graph.getGraphLayoutCache().order(cells);
      // If Any Cells in View
      if (cells != null && cells.length > 0) 
      {
         // Create Group Cell
         int count = getCellCount(graph);
         DefaultGraphCell group =
         new DefaultGraphCell(new Integer(count - 1));
         // Create Change Information
         ParentMap map = new ParentMap();
         // Insert Child Parent Entries
         for (int i = 0; i < cells.length; i++)
         {
            map.addEntry(cells[i], group);
         }
         // Insert into model
         graph.getGraphLayoutCache().insert(
            new Object[] { group },
            null,
            null,
            map,
            null);
      }
   }

   // Returns the total number of cells in a graph
   protected int getCellCount(JGraph graph) 
   {
      Object[] cells = graph.getDescendants(graph.getRoots());
      return cells.length;
   }

   // Ungroup the Groups in Cells and Select the Children
   public void ungroup(Object[] cells) 
   {
      // If any Cells
      if (cells != null && cells.length > 0) 
      {
         // List that Holds the Groups
         ArrayList groups = new ArrayList();
         // List that Holds the Children
         ArrayList children = new ArrayList();
         // Loop Cells
         for (int i = 0; i < cells.length; i++) 
         {
            // If Cell is a Group
            if (isGroup(cells[i])) 
            {
               // Add to List of Groups
               groups.add(cells[i]);
               // Loop Children of Cell
               for (int j = 0; j < graph.getModel().getChildCount(cells[i]); j++)
               {
                  // Get Child from Model
                  Object child = graph.getModel().getChild(cells[i], j);
                  // If Not Port
                  if (!(child instanceof Port))
                  {
                     // Add to Children List
                     children.add(child);
                  }
               }
            }
         }
         // Remove Groups from Model (Without Children)
         graph.getGraphLayoutCache().remove(groups.toArray());
         // Select Children
         graph.setSelectionCells(children.toArray());
      }
   }

   // Determines if a Cell is a Group
   public boolean isGroup(Object cell) 
   {
      // Map the Cell to its View
      CellView view = graph.getGraphLayoutCache().getMapping(cell, false);
      if (view != null)
      {
         return !view.isLeaf();
      }
      return false;
   }

   // Brings the Specified Cells to Front
   public void toFront(Object[] c) 
   {
      graph.getGraphLayoutCache().toFront(c);
   }

   // Sends the Specified Cells to Back
   public void toBack(Object[] c) 
   {
      graph.getGraphLayoutCache().toBack(c);
   }

   // Undo the last Change to the Model or the View
   public void undo() 
   {
      try 
      {
         undoManager.undo(graph.getGraphLayoutCache());
      } 
      catch (Exception ex) 
      {
         System.err.println(ex);
      } 
      finally 
      {
         updateHistoryButtons();
      }
   }

   // Redo the last Change to the Model or the View
   public void redo() 
   {
      try 
      {
         undoManager.redo(graph.getGraphLayoutCache());
      } 
      catch (Exception ex) 
      {
         System.err.println(ex);
      } 
      finally 
      {
         updateHistoryButtons();
      }
   }

   // Update Undo/Redo Button State based on Undo Manager
   protected void updateHistoryButtons() 
   {
      // The View Argument Defines the Context
      undo.setEnabled(undoManager.canUndo(graph.getGraphLayoutCache()));
      redo.setEnabled(undoManager.canRedo(graph.getGraphLayoutCache()));
   }

   //
   // Listeners
   //

   // From GraphSelectionListener Interface
   public void valueChanged(GraphSelectionEvent e) 
   {
      // Group Button only Enabled if more than One Cell Selected
      group.setEnabled(graph.getSelectionCount() > 1);
      // Update Button States based on Current Selection
      boolean enabled = !graph.isSelectionEmpty();
      remove.setEnabled(enabled);
      ungroup.setEnabled(enabled);
      tofront.setEnabled(enabled);
      toback.setEnabled(enabled);
      copy.setEnabled(enabled);
      cut.setEnabled(enabled);
   }

   //
   // KeyListener for Delete KeyStroke
   //
   public void keyReleased(KeyEvent e) 
   {;
   }
   public void keyTyped(KeyEvent e) 
   {;
   }
   public void keyPressed(KeyEvent e) 
   {
      // Listen for Delete Key Press
      if (e.getKeyCode() == KeyEvent.VK_DELETE)
      {
         // Execute Remove Action on Delete Key Press
         remove.actionPerformed(null);
      }
   }
   
   //
   //
   //
   //
   // Custom MarqueeHandler

   // MarqueeHandler that Connects Vertices and Displays PopupMenus
   public class ProxyMarqueeHandler extends BasicMarqueeHandler 
   {

      // Holds the Start and the Current Point
      protected Point start, current;

      // Holds the First and the Current Port
      protected PortView port, firstPort;

      // Override to Gain Control (for PopupMenu and ConnectMode)
      public boolean isForceMarqueeEvent(MouseEvent e) 
      {
         // If Right Mouse Button we want to Display the PopupMenu
         if (SwingUtilities.isRightMouseButton(e))
         {
            // Return Immediately
            return true;
         }
         // Find and Remember Port
         port = getSourcePortAt(e.getPoint());
         // If Port Found and in ConnectMode (=Ports Visible)
         if (port != null && graph.isPortsVisible())
         {
            return true;
         }
         // Else Call Superclass
         return super.isForceMarqueeEvent(e);
      }

      // Display PopupMenu or Remember Start Location and First Port
      public void mousePressed(final MouseEvent e)
      {
         // If Right Mouse Button
         if (SwingUtilities.isRightMouseButton(e)) 
         {
            // Scale From Screen to Model
            Point loc = graph.fromScreen(e.getPoint());
            // Find Cell in Model Coordinates
            Object cell = graph.getFirstCellForLocation(loc.x, loc.y);
            // Create PopupMenu for the Cell
            JPopupMenu menu = createPopupMenu(e.getPoint(), cell);
            // Display PopupMenu
            menu.show(graph, e.getX(), e.getY());

         // Else if in ConnectMode and Remembered Port is Valid
         } 
         else if (port != null && !e.isConsumed() && graph.isPortsVisible()) 
         {
            // Remember Start Location
            start = graph.toScreen(port.getLocation(null));
            // Remember First Port
            firstPort = port;
            // Consume Event
            e.consume();
         } 
         else
         {
            // Call Superclass
            super.mousePressed(e);
         }
      }

      // Find Port under Mouse and Repaint Connector
      public void mouseDragged(MouseEvent e) 
      {
         // If remembered Start Point is Valid
         if (start != null && !e.isConsumed()) 
         {
            // Fetch Graphics from Graph
            Graphics g = graph.getGraphics();
            // Xor-Paint the old Connector (Hide old Connector)
            paintConnector(Color.black, graph.getBackground(), g);
            // Reset Remembered Port
            port = getTargetPortAt(e.getPoint());
            // If Port was found then Point to Port Location
            if (port != null)
            {
               current = graph.toScreen(port.getLocation(null));
            }
            else // Else If no Port was found then Point to Mouse Location
            {
               current = graph.snap(e.getPoint());
            }
            // Xor-Paint the new Connector
            paintConnector(graph.getBackground(), Color.black, g);
            // Consume Event
            e.consume();
         }
         // Call Superclass
         super.mouseDragged(e);
      }

      public PortView getSourcePortAt(Point point)
      {
         // Scale from Screen to Model
         Point tmp = graph.fromScreen(new Point(point));
         // Find a Port View in Model Coordinates and Remember
         return graph.getPortViewAt(tmp.x, tmp.y);
      }

      // Find a Cell at point and Return its first Port as a PortView
      protected PortView getTargetPortAt(Point point) 
      {
         // Find Cell at point (No scaling needed here)
         Object cell = graph.getFirstCellForLocation(point.x, point.y);
         // Loop Children to find PortView
         for (int i = 0; i < graph.getModel().getChildCount(cell); i++) 
         {
            // Get Child from Model
            Object tmp = graph.getModel().getChild(cell, i);
            // Get View for Child using the Graph's View as a Cell Mapper
            tmp = graph.getGraphLayoutCache().getMapping(tmp, false);
            // If Child View is a Port View and not equal to First Port
            if (tmp instanceof PortView && tmp != firstPort)
            {
               // Return as PortView
               return (PortView) tmp;
            }
         }
         // No Port View found
         return getSourcePortAt(point);
      }

      // Connect the First Port and the Current Port in the Graph or Repaint
      public void mouseReleased(MouseEvent e) 
      {
         // If Valid Event, Current and First Port
         if (e != null
            && !e.isConsumed()
            && port != null
            && firstPort != null
            && firstPort != port) 
         {
            // Then Establish Connection
            connect((Port) firstPort.getCell(), (Port) port.getCell());
            // Consume Event
            e.consume();
            // Else Repaint the Graph
         } 
         else
         {
            graph.repaint();
         }
         // Reset Global Vars
         firstPort = port = null;
         start = current = null;
         // Call Superclass
         super.mouseReleased(e);
      }

      // Show Special Cursor if Over Port
      public void mouseMoved(MouseEvent e) 
      {
         // Check Mode and Find Port
         if (e != null
            && getSourcePortAt(e.getPoint()) != null
            && !e.isConsumed()
            && graph.isPortsVisible()) 
         {
            // Set Cusor on Graph (Automatically Reset)
            graph.setCursor(new Cursor(Cursor.HAND_CURSOR));
            // Consume Event
            e.consume();
         }
         // Call Superclass
         super.mouseReleased(e);
      }

      // Use Xor-Mode on Graphics to Paint Connector
      protected void paintConnector(Color fg, Color bg, Graphics g) 
      {
         // Set Foreground
         g.setColor(fg);
         // Set Xor-Mode Color
         g.setXORMode(bg);
         // Highlight the Current Port
         paintPort(graph.getGraphics());
         // If Valid First Port, Start and Current Point
         if (firstPort != null && start != null && current != null)
         {
            // Then Draw A Line From Start to Current Point
            g.drawLine(start.x, start.y, current.x, current.y);
         }
      }

      // Use the Preview Flag to Draw a Highlighted Port
      protected void paintPort(Graphics g) 
      {
         // If Current Port is Valid
         if (port != null) 
         {
            // If Not Floating Port...
            boolean o = (GraphConstants.getOffset(port.getAttributes()) != null);
            // ...Then use Parent's Bounds
            Rectangle r = (o) ? port.getBounds() : port.getParentView().getBounds();
            // Scale from Model to Screen
            r = graph.toScreen(new Rectangle(r));
            // Add Space For the Highlight Border
            r.setBounds(r.x - 3, r.y - 3, r.width + 6, r.height + 6);
            // Paint Port in Preview (=Highlight) Mode
            graph.getUI().paintCell(g, port, r, true);
         }
      }
   } // End of Editor.ProxyMarqueeHandler

   //
   // PopupMenu and ToolBar
   //

   //
   //
   //

   //
   // PopupMenu
   //
   public JPopupMenu createPopupMenu(final Point pt, final Object cell) 
   {
      JPopupMenu menu = new JPopupMenu();
      if (cell != null) 
      {
         // Edit
         menu.add(new AbstractAction("Edit") 
            {
               public void actionPerformed(ActionEvent e) 
               {
                  graph.startEditingAtCell(cell);
               }
            });
      }
      // Remove
      if (!graph.isSelectionEmpty()) 
      {
         menu.addSeparator();
         menu.add(new AbstractAction("Remove") 
            {
               public void actionPerformed(ActionEvent e) 
               {
                  remove.actionPerformed(e);
               }
            });
      }
      menu.addSeparator();
      // Insert digital
      menu.add(new AbstractAction("Insert Digital") 
         {
            public void actionPerformed(ActionEvent ev) 
            {
               //insert(new DigitalProxy(pt, "Digital"));
               //insert(pt, 0);
            }
         });
      // Insert analog
      menu.add(new AbstractAction("Insert Analog") 
         {
            public void actionPerformed(ActionEvent ev) 
            {
               //insert(pt, 1);
               //insert(new AnalogProxy(pt, "Analog"));
            }
         });
      // Insert position
      menu.add(new AbstractAction("Insert Position") 
         {
            public void actionPerformed(ActionEvent ev) 
            {
               //insert(pt, 2);
               //insert(new PositionProxy(pt, "Position"));
            }
         });
      // Insert event window
      menu.add(new AbstractAction("Insert EventWindow") 
         {
            public void actionPerformed(ActionEvent ev) 
            {
               //insert(pt, 2);
               //insert(new EventWindowProxy(pt, "EventWindow"));
            }
         });
      // Insert gesture
      menu.add(new AbstractAction("Insert Gesture") 
         {
            public void actionPerformed(ActionEvent ev) 
            {
               //insert(pt, 2);
               //insert(new GestureProxy(pt, "Gesture"));
            }
         });
      // Insert position
      menu.add(new AbstractAction("Insert Glove") 
         {
            public void actionPerformed(ActionEvent ev) 
            {
               //insert(pt, 2);
               //insert(new GloveProxy(pt, "Glove"));
            }
         });
      // Insert alias
      menu.add(new AbstractAction("Insert Alias") 
         {
            public void actionPerformed(ActionEvent ev) 
            {
               //insert(pt, 3);
               //insert(new Alias(pt, "Alias"));
            }
         });
      return menu;
   }

   private ImageIcon loadIcon(String filename)
   {
      ImageIcon insertIcon = new ImageIcon();

      String full_path = "org/vrjuggler/vrjconfig/customeditors/proxyeditor/images/" + filename;
      
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         insertIcon = new ImageIcon(loader.getResource(full_path));
      }
      catch (NullPointerException npe)
      {
         System.out.println("ERROR: Could not load" + full_path);
         // Silently ignore all errors
      }

      return insertIcon;
   }


   // This will change the source of the actionevent to graph.
   protected class EventRedirector extends AbstractAction 
   {
      protected Action action;

      // Construct the "Wrapper" Action
      public EventRedirector(Action a) 
      {
         super("", (ImageIcon) a.getValue(Action.SMALL_ICON));
         this.action = a;
      }

      // Redirect the Actionevent
      public void actionPerformed(ActionEvent e) 
      {
         e = new ActionEvent(
            graph,
            e.getID(),
            e.getActionCommand(),
            e.getModifiers());
         action.actionPerformed(e);
      }
   }
}
