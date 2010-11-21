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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.util.HashMap;
import java.util.Map;
import org.jgraph.graph.DefaultCellViewFactory;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.VertexView;

import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;


public class ProxiedDeviceCellViewFactory
   extends DefaultCellViewFactory
{
   public ProxiedDeviceCellViewFactory()
   {
      this(new HashMap());
   }

   public ProxiedDeviceCellViewFactory(Map creatorMap)
   {
      mCreatorMap = creatorMap;
   }

   /**
    * Pairs up the given <code>ConfigDefinition</code> with the given
    * <code>Class</code> instance for later creation of custom views for cells
    * holding <code>ConfigElement</code> objects with the given definition.
    * The <code>Class</code> instance must be for a type that is a subclass
    * of <code>org.jgraph.graph.AbstractCellView</code>.
    *
    * @param def        the config definition that will be used for looking
    *                   up the <code>Class</code> reference in the view
    *                   creation methods
    * @param creator    the creator class reference
    *
    * @see org.jgraph.graph.AbstractCellView
    */
   public void registerCreator(ConfigDefinition def, Class creator)
   {
      mCreatorMap.put(def, creator);
   }

   /**
    * Creates a <code>VertexView</code> for the given cell, which is expected
    * to be for a vertex in the graph.  If the cell is a type we are expecting
    * and it holds a user object that we know how to handle, we determine if a
    * custom renderer has been registered with us for that cell type.  If no
    * custom view is found, we fall back on the superclass version of this
    * method.
    *
    * @return An instance of <code>VertexView</code> (or a subclass thereof)
    *         for the given cell.
    *
    * @see org.jgraph.graph.VertexView
    * @see org.jgraph.graph.DefaultCellViewFactory#createVertexView(Object)
    */
   protected VertexView createVertexView(Object cell)
   {
      VertexView view = null;

      // Right now, we expect that vertex cells are instances of
      // DefaultGraphCell (or some subclass thereof) and that they contain a
      // user object of type ConfigElementHolder (or some subclass thereof).
      // This is not inifinitely flexible, but considering the purpose of the
      // devicegraph package, it is a reasonable assumption.
      if ( cell instanceof DefaultGraphCell &&
           ((DefaultGraphCell) cell).getUserObject() instanceof ConfigElementHolder )
      {
         // Extract the user object which we know to be an instance of
         // ConfigElementHolder.
         ConfigElementHolder holder =
            (ConfigElementHolder) ((DefaultGraphCell) cell).getUserObject();

         // Look up the config definition for the held ConfigElement and check
         // to see if a VertexView was registered with us for that definition.
         ConfigElement elt    = holder.getElement();
         ConfigDefinition def = elt.getDefinition();
         Class view_class     = (Class) mCreatorMap.get(def);

         // If a VertexView for the cell's config definition was registered
         // with us, try to instantiate and use that view for the given
         // cell.
         if ( null != view_class )
         {
            try
            {
               view = (VertexView) view_class.newInstance();
               view.setCell(cell);
            }
            // If we catch an exception trying to instantiate the custom
            // VertexView type, ignore it and leave view set to null.
            catch (Exception e)
            {
               e.printStackTrace();
            }
         }
      }

      // If we have no custom view at this point, use the JGraph version of
      // createVertexView() to get a default view for the given cell.
      if ( null == view )
      {
         view = super.createVertexView(cell);
      }

      return view;
   }

   private Map mCreatorMap = null;
}
