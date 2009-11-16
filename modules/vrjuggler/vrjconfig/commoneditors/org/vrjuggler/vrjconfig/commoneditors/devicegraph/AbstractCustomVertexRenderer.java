/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.util.Map;
import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.jgraph.JGraph;
import org.jgraph.graph.CellView;
import org.jgraph.graph.CellViewRenderer;
import org.jgraph.graph.GraphConstants;


/**
 * Basic (abstract) custom vertex renderer&mdash;based on JPanel&mdash;for use
 * with JGraph.  This code is based on examples included with
 * <a href="http://www.jgraph.com/">JGraph</a> and JGraphAddons for creating
 * custom vertex renderers.
 */
public abstract class AbstractCustomVertexRenderer
   extends JPanel
   implements CellViewRenderer
{
   public AbstractCustomVertexRenderer()
   {
      defaultForeground = UIManager.getColor("Tree.textForeground");
      defaultBackground = UIManager.getColor("Tree.textBackground");
   }

   /**
    * Configures and returns the renderer component based on the passed-in
    * cell view.
    * 
    * @param graph      the graph that that defines the rendering context
    * @param view       the object that should be rendered
    * @param selected   whether the object is selected
    * @param hasFocus   whether the object has the focus
    * @param preview    whether we are drawing a preview
    *
    * @see org.jgraph.graph.CellViewRenderer#getRendererComponent(JGraph,CellView,boolean,boolean,boolean)
    */
   public abstract Component getRendererComponent(JGraph graph, CellView view,
                                                  boolean selected,
                                                  boolean hasFocus,
                                                  boolean preview);

   public void paint(Graphics g)
   {
      if ( gradientColor != null && ! preview )
      {
         setOpaque(false);
         Graphics2D graphics = (Graphics2D) g;
         graphics.setPaint(new GradientPaint(0, 0, getBackground(),
                                             getWidth(), getHeight(),
                                             gradientColor, true));
         graphics.fillRect(0, 0, getWidth(), getHeight());
      }

      super.paint(g);
      paintSelectionBorder(g);
   }

   private void paintSelectionBorder(Graphics g)
   {
      ((Graphics2D) g).setStroke(GraphConstants.SELECTION_STROKE);

      if ( hasFocus && selected )
      {
         g.setColor(graph.getLockedHandleColor());
      }
      else if ( selected )
      {
         g.setColor(graph.getHighlightColor());
      }

      if ( selected )
      {
         Dimension d = getSize();
         g.drawRect(0, 0, d.width - 1, d.height - 1);
      }
   }

   /**
    * Sets up the attributes for the renderer as determined by the given
    * <code>JGraph</code> instance and the given attribute map.
    *
    * @param graph      the graph where this renderer is being used
    * @param attributes the attributes for the cell view with which this
    *                   renderer is associated
    *
    * @see org.jgraph.graph.GraphConstants
    */
   protected void installAttributes(JGraph graph, Map attributes)
   {
      setOpaque(GraphConstants.isOpaque(attributes));

      nameLabel.setIcon(GraphConstants.getIcon(attributes));
      nameLabel.setVerticalAlignment(
         GraphConstants.getVerticalAlignment(attributes)
      );
      nameLabel.setHorizontalAlignment(
         GraphConstants.getHorizontalAlignment(attributes)
      );
      nameLabel.setVerticalTextPosition(
         GraphConstants.getVerticalTextPosition(attributes)
      );
      nameLabel.setHorizontalTextPosition(
         GraphConstants.getHorizontalTextPosition(attributes)
      );

      Color foreground = GraphConstants.getForeground(attributes);

      if ( foreground == null )
      {
         setForeground(defaultForeground);
      }
      else
      {
         setForeground(foreground);
      }

      Color background = GraphConstants.getBackground(attributes);

      if ( background == null )
      {
         setBackground(defaultBackground);
      }
      else
      {
         setBackground(background);
      }

      java.awt.Font font = GraphConstants.getFont(attributes);

      if ( font == null )
      {
         nameLabel.setFont(graph.getFont());
      }
      else
      {
         nameLabel.setFont(font);
      }

      Border border      = GraphConstants.getBorder(attributes);
      Color border_color = GraphConstants.getBorderColor(attributes);

      if ( border != null )
      {
         setBorder(border);
      }
      else if ( border_color != null )
      {
         int border_width =
            Math.max(1, Math.round(GraphConstants.getLineWidth(attributes)));
         setBorder(BorderFactory.createLineBorder(border_color,
                                                  border_width));
      }

      gradientColor = GraphConstants.getGradientColor(attributes);
   }

   /** The graph where this vertex renderer is used. */
   protected transient JGraph graph = null;

   /**
    * The gradient (if any) associated with the attributes of the cell view
    * being rendered.  This is set in <code>installAttributes()</code>.
    *
    * @see #installAttributes(JGraph,Map)
    */
   protected transient Color gradientColor = null;

   /** A flag indicating whether the vertex is selected. */
   protected transient boolean selected = false;

   /**
    * A flag indicating whether the vertex view should be rendered as a
    * preview is selected.
    */
   protected transient boolean preview = false;

   /** A flag indicating if the vertex view has focus. */
   protected transient boolean hasFocus = false;

   /**
    * The default background color to use for rendering if none is defined
    * in the cell view attributes.
    *
    * @see #installAttributes(JGraph,Map)
    */
   protected Color defaultBackground = null;

   /**
    * The default foreground color to use for rendering if none is defined
    * in the cell view attributes.
    *
    * @see #installAttributes(JGraph,Map)
    */
   protected Color defaultForeground = null;

   /**
    * The Swing component used for displaying the name of the object held by
    * the vertex cell.
    */
   protected JLabel nameLabel = new JLabel();
}
