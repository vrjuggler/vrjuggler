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
import java.net.URL;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Map;

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

   //
   // Custom Graph
   //

   // Defines a Graph that uses the Shift-Button (Instead of the Right
   // Mouse Button, which is Default) to add/remove point to/from an edge.
   public class ProxyGraph extends JGraph 
   {
      // Construct the Graph using the Model as its Data Source and the given
      // marquee handler
      public ProxyGraph(GraphModel model, BasicMarqueeHandler marquee) 
      {
         super(model);
         // Use a Custom Marquee Handler
         setMarqueeHandler(marquee);
         // Tell the Graph to Select new Cells upon Insertion
         setSelectNewCells(true);
         // Make Ports Visible by Default
         setPortsVisible(true);
         // Use the Grid (but don't make it Visible)
         setGridEnabled(true);
         // Set the Grid Size to 10 Pixel
         setGridSize(6);
         // Set the Tolerance to 2 Pixel
         setTolerance(2);
      }

      // Override Superclass Method to Return Custom EdgeView
      protected EdgeView createEdgeView(Edge e, CellMapper cm) 
      {
         // Return Custom EdgeView
         EdgeView edge = new EdgeView(e, this, cm) 
            {
               // Override Superclass Method
               public boolean isAddPointEvent(MouseEvent event) 
               {
                  // Points are Added using Shift-Click
                  return event.isShiftDown();
               }
               // Override Superclass Method
               public boolean isRemovePointEvent(MouseEvent event) 
               {
                  // Points are Removed using Shift-Click
                  return event.isShiftDown();
               }
            };
         return edge;
      }
      /**
       * Constructs a PortView view for the specified object.
       */
      protected PortView createPortView(Port p, CellMapper cm) 
      {
         if(p == null)
         {
            System.out.println("Port is null");
         }
         else
         {
            if(p instanceof DefaultPort)
            {
               System.out.println("Object: " +
                     ((DefaultPort)p).getUserObject());
            }
         }
         return new MyPortView(p, this, cm);
      }
   }
