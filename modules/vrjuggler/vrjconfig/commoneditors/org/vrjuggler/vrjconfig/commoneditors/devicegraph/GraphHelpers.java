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

import java.awt.Dimension;
import java.awt.geom.Rectangle2D;
import java.util.Hashtable;

import org.jgraph.JGraph;
import org.jgraph.graph.AttributeMap;
import org.jgraph.graph.CellView;
import org.jgraph.graph.GraphConstants;


/**
 * A collection of helper functions that can be helpful in extending JGraph.
 */
public abstract class GraphHelpers
{
   /**
    * Changes the size of the given <code>CellView</code> reference to be the
    * given dimensions.  The graph cell associated with the given view must be
    * in the given graph.  If the cell view is resized successfully, an edit is
    * posted with the graph's layout cache, and the graph is repainted.
    *
    * @param graph      the graph that knows about the cell view and its cell
    * @param size       the new dimensions for the cell view
    * @param view       the cell view to be resized
    *
    * @see #autoSizeCellView(Dimension,CellView,Hashtable)
    */
   public static void autoSizeCellView(JGraph graph, Dimension size,
                                       CellView view)
   {
      Hashtable table = new Hashtable();
      if ( autoSizeCellView(size, view, table) )
      {
         graph.getGraphLayoutCache().edit(table, null, null, null);
         graph.repaint();
      }
   }

   /**
    * Changes the size of the given <code>CellView</code> reference to be the
    * given dimensions.  The resizing is done by modifying the bounds in the
    * cell view's attributes.  The position of the cell view is left unchanged,
    * but the width and height are changed to the values given in
    * <code>size</code>.  The new attributes are placed into the given
    * <code>Hashtable</code> object using the view's cell as the key.
    *
    * @param size       the new dimensions for the cell view
    * @param view       the cell view to be resized
    * @param table      a non-null hashtable used for storing the modified cell
    *                   view attributes
    *
    * @see org.jgraph.graph.GraphConstants#setBounds(Map,Rectangle2D)
    * @see org.jgraph.graph.CellView#getCell()
    */
   public static boolean autoSizeCellView(Dimension size, CellView view,
                                          Hashtable table)
   {
      boolean edited = false;
      Rectangle2D bounds = GraphConstants.getBounds(view.getAllAttributes());

      if ( bounds != null )
      {
         bounds = (Rectangle2D) bounds.clone();
         bounds.setFrame(bounds.getX(), bounds.getY(), size.width, size.height);
         AttributeMap map = new AttributeMap();
         GraphConstants.setBounds(map, bounds);
         table.put(view.getCell(), map);
         edited = true;
      }

      return edited;
   }
}
