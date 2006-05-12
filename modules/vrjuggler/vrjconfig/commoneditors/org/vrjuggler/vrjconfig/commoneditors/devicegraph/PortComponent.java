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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.awt.Container;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.geom.Point2D;
import java.util.Hashtable;
import javax.swing.JComponent;
import javax.swing.SwingUtilities;

import org.jgraph.JGraph;
import org.jgraph.graph.AttributeMap;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.GraphConstants;


/**
 * This class is an attempt to work around the difficulty of figuring out where
 * to place a port within a vertex that uses a custom renderer.  A vertex
 * renderer using this class can add instances of this class to its layout,
 * and the ports will have their location (offset in JGraph parlance) set based
 * on the location of the component's instance within the layout.  Hence,
 * layout changes cause the ports to be relocated.
 *
 * This class is a colossal hack; it is an abomination.  It exists for two
 * reasons:
 *
 * <ol>
 *   <li>
 *     I could not figure out how to set port locations the way I wanted in
 *     the custom renderers.
 *   </li>
 *   <li>
 *     I could not make sense enough of how to use JGraph's port renderers
 *     to implement the capabilities I wanted using their system.
 *   </li>
 * </ol>
 *
 * This works, and it works behind the scenes.  It is, nonetheless, a hack.
 */
public class PortComponent
   extends JComponent
{
   public PortComponent(JGraph graph, DefaultPort port)
   {
      this(graph, port, null);
   }

   /**
    * Creates a new port component.
    *
    * @param graph      the graph containing the given port
    * @param port       the port associated with this component
    * @param renderer   the component acting as the renderer for the vertex
    *                   that has <code>port</code> as a child
    */
   public PortComponent(JGraph graph, DefaultPort port,
                        Container renderer)
   {
      super();
      setGraph(graph);
      setPort(port);
      setRootComponent(renderer);

      mNewPortOffset = port.getAttributes().createPoint(0.0, 0.0);
   }

   public void setBounds(int x, int y, int width, int height)
   {
      AttributeMap map = this.port.getAttributes();
      Point2D cur_loc  = GraphConstants.getOffset(map);

      int new_x = x, new_y = y;

      // As if things weren't already bad enough with this class, this is
      // another huge hack.  This ensures that nesting of components does not
      // screw up the calculation of the port offset.
      if ( this.rootComponent != null &&
           this.getParent() != this.rootComponent )
      {
         Point parent_point =
            SwingUtilities.convertPoint(this.getParent(), x, y,
                                        this.rootComponent);
         new_x = parent_point.x;
         new_y = parent_point.y;
      }

      mNewPortOffset.setLocation(new_x + width / 2.0, new_y + height / 2.0);

      // !!! Hack warning !!!
      // Posting an edit with the graph model for the port relocation causes
      // this method to be invoked again.  Hence, we get into an infinite
      // loop.  This is avoided by not changing the port location if it is
      // already the same as mNewPortOffset.
      if ( cur_loc == null || ! cur_loc.equals(mNewPortOffset) )
      {
         // Do not create the new point unless we have to.
         GraphConstants.setOffset(map, map.createPoint(mNewPortOffset));
         mEditTable.put(this.port, map);
         this.graph.getModel().edit(mEditTable, null, null, null);
      }

      super.setBounds(x, y, width, height);
   }

   public DefaultPort getPort()
   {
      return this.port;
   }

   /**
    * Sets the port to be used by this component.  The port's attributes are
    * modified to use absolute positioning.  If the caller were to revert back
    * to relative positioning, the results would be undesirable.
    */
   public void setPort(DefaultPort port)
   {
      this.port = port;

      // Ports have to use absolute positioning for this class to work.
      GraphConstants.setAbsolute(this.port.getAttributes(), true);
   }

   public JGraph getGraph()
   {
      return this.graph;
   }

   public void setGraph(JGraph graph)
   {
      this.graph = graph;
   }

   /**
    * Sets the root component containing this component.  This must be the
    * renderer component for the vertex that contains our port as a child.
    */
   public void setRootComponent(Container renderer)
   {
      this.rootComponent = renderer;
   }

   /**
    * Paints this component as a simple circle.
    */
   public void paint(Graphics g)
   {
      java.awt.Dimension d = getSize();
      g.fillOval(0, 0, d.width, d.height);
      g.setColor(getForeground());
   }

   private JGraph      graph         = null;
   private DefaultPort port          = null;
   private Container   rootComponent = null;

   private Hashtable mEditTable     = new Hashtable();
   private Point2D   mNewPortOffset = null;
}
