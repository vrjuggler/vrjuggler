/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
package org.vrjuggler.vrjconfig.ui;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.EventListenerList;
import org.vrjuggler.vrjconfig.ui.placer.*;

/**
 * Allows the user to move displays about using Drag-and-Drop semantics. Such
 * a panel is useful for allowing the user to specify where simulator windows
 * should be placed and how they should be sized.
 * <p>
 * Displays placed within this panel are moveable using the mouse.
 */
public class Placer
   extends JPanel
{
   /**
    * Whether displays may be resized.
    */
   private boolean allowResize = true;

   /**
    * Whether displays may be move.
    */
   private boolean allowMove = true;

   /**
    * The index of the currently selected object, -1 if nothing selected.
    */
   private transient int selectedIndex = -1;

   /**
    * The position of the last mouse event.
    */
   private transient Point lastMouse = null;

   /**
    * The data model for this placer.
    */
   private PlacerModel model = null;

   /**
    * The renderer for this placer.
    */
   private PlacerRenderer renderer = null;

   /**
    * The pane used to render placer renderer components.
    */
   private CellRendererPane rendererPane = new CellRendererPane();

   /**
    * The foreground color for selected items.
    */
   private Color selectionForeground = Color.gray;

   /**
    * The background color for selected items.
    */
   private Color selectionBackground = Color.blue;

   /**
    * All listeners interested in this placer.
    */
   private EventListenerList listeners = new EventListenerList();

   /**
    * Handler for when the model changes.
    */
   private PlacerModelHandler modelHandler = new PlacerModelHandler();

   /**
    * Creates a new placer with a default model.
    */
   public Placer()
   {
      this(new AbstractPlacerModel()
      {
         public Object getElement(int idx) { return null; }
         public Object getElementAt(Point pt) { return null; }
         public int getIndexOfElementAt(Point pt) { return -1; }
         public Dimension getSizeOf(int idx) { return new Dimension(0,0); }
         public void setSizeOf(int idx, Dimension d) { }
         public Point getLocationOf(int idx) { return new Point(0,0); }
         public void setLocationOf(int idx, Point pt) { }
         public void moveToFront(int idx) { }
         public int getSize() { return 0; }
      });
   }

   /**
    * Create a new Placer with the given data model.
    */
   public Placer(PlacerModel model)
   {
      // init the model
      if (model == null)
      {
         throw new IllegalArgumentException("model can not be null");
      }
      this.model = model;

      // init the renderer
      renderer = new DefaultPlacerRenderer();

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   /**
    * Sets the data model to use with this placer.
    */
   public void setModel(PlacerModel model)
   {
      if (model == null)
      {
         throw new IllegalArgumentException("model can not be null");
      }
      PlacerModel oldValue = this.model;
      oldValue.removePlacerModelListener(modelHandler);
      this.model = model;
      this.model.addPlacerModelListener(modelHandler);
      firePropertyChange("model", oldValue, model);
   }

   /**
    * Gets the data model used with this placer.
    */
   public PlacerModel getModel()
   {
      return model;
   }

   /**
    * Sets the renderer to use with this placer.
    */
   public void setRenderer(PlacerRenderer renderer)
   {
      if (renderer == null)
      {
         throw new IllegalArgumentException("renderer can not be null");
      }
      PlacerRenderer oldValue = this.renderer;
      this.renderer = renderer;
      System.out.println("Set renderer to: "+this.renderer);
      firePropertyChange("renderer", oldValue, renderer);
   }

   /**
    * Gets the renderer used with this placer.
    */
   public PlacerRenderer getRenderer()
   {
      return renderer;
   }

   /**
   /**
    * Sets whether displays may be resized.
    */
   public void setAllowResize(boolean allowResize)
   {
      this.allowResize = allowResize;
   }

   /**
    * Gets whether displays may be resized.
    */
   public boolean getAllowResize()
   {
      return allowResize;
   }

   /**
    * Sets whether displays may be moved.
    */
   public void setAllowMove(boolean allowMove)
   {
      this.allowMove = allowMove;
   }

   /**
    * Gets whether displays may be moved.
    */
   public boolean getAllowMove()
   {
      return allowMove;
   }

   /**
    * Gets the foreground color for selected cells.
    */
   public Color getSelectionForeground()
   {
      return selectionForeground;
   }

   /**
    * Gets the background color for selected cells.
    */
   public Color getSelectionBackground()
   {
      return selectionBackground;
   }

   /**
    * Sets the foreground color for selected cells.
    */
   public void setSelectionForeground(Color color)
   {
      selectionForeground = color;
   }

   /**
    * Gets the background color for selected cells.
    */
   public void setSelectionBackground(Color color)
   {
      selectionBackground = color;
   }

   /**
    * Gets the index of the selected item.
    *
    * @return  the selected index; -1 if nothing is selected
    */
   public int getSelectedIndex()
   {
      return selectedIndex;
   }

   /**
    * Gets the value of the selected item.
    *
    * @return  the selected value; null if nothing is selected
    */
   public Object getSelectedValue()
   {
      return ((selectedIndex == -1) ? null : model.getElement(selectedIndex));
   }

   /**
    * Gets the top-most display that contains the given point.
    */
//   public Display getDisplayAt(Point pt)
//   {
//      for (Iterator itr = displays.iterator(); itr.hasNext(); )
//      {
//         Display d = (Display)itr.next();
//         Point loc = d.getLocation();
//         if (d.contains(pt.x - loc.x, pt.y - loc.y))
//         {
//            return d;
//         }
//      }
//      return null;
//   }

   /**
    * Initializes UI components and sets up listeners.
    */
   private void jbInit() throws Exception
   {
      this.setLayout(null);
      this.setOpaque(true);
      this.addMouseListener(new java.awt.event.MouseAdapter()
      {
         public void mousePressed(MouseEvent e)
         {
            this_mousePressed(e);
         }
      });
      this.addMouseMotionListener(new java.awt.event.MouseMotionAdapter()
      {
         public void mouseDragged(MouseEvent e)
         {
            this_mouseDragged(e);
         }
      });
      this.add(rendererPane);
   }

   /**
    * Handles a mouse-press action.
    */
   void this_mousePressed(MouseEvent e)
   {
      // Find the display that has been selected
      int idx = getModel().getIndexOfElementAt(e.getPoint());
      if (idx == -1)
      {
         selectedIndex = -1;
      }
      else
      {
         // Move the selected element up to the front
         getModel().moveToFront(idx);
         selectedIndex = 0;
      }
      lastMouse = e.getPoint();
      repaint();
      fireItemSelected(selectedIndex);
   }

   /**
    * Handles dragging of the selected object around.
    */
   void this_mouseDragged(MouseEvent e)
   {
      if (selectedIndex != -1)
      {
         // Figure out the distance moved
         Point mousePt = e.getPoint();
         Point diff = new Point(mousePt.x - lastMouse.x,
                                mousePt.y - lastMouse.y);

         // Move the selected display along
         Point newPos = new Point(getModel().getLocationOf(selectedIndex));
         newPos.translate(diff.x, diff.y);

         // Make sure we don't allow the user to drag a display outside of the
         // viewable area.
         Dimension dim = getModel().getSizeOf(selectedIndex);
         Dimension visDim = this.getSize();
         if (newPos.x + dim.width > visDim.width)
         {
            newPos.x = visDim.width - dim.width;
         }
         if (newPos.y + dim.height > visDim.height)
         {
            newPos.y = visDim.height - dim.height;
         }
         if (newPos.x < 0)
         {
            newPos.x = 0;
         }
         if (newPos.y < 0)
         {
            newPos.y = 0;
         }
         getModel().setLocationOf(selectedIndex, newPos);

         lastMouse = mousePt;
         repaint();
      }
   }

   /**
    * Draws the objects in this placer's data model using the current
    * PlacerRenderer.
    */
   public synchronized void paint(Graphics g)
   {
//      System.out.println("Painting the Placer!, selIdx = "+selectedIndex);
      super.paint(g);

      // Paint each object in the model using the current renderer
      for (int i=getModel().getSize()-1; i>=0; --i)
      {
         Component rendererComponent = renderer.getPlacerRendererComponent(
                                             this,
                                             getModel().getElement(i),
                                             (i == selectedIndex),
                                             false,
                                             i);
         Point pos = getModel().getLocationOf(i);
         Dimension dim = getModel().getSizeOf(i);
         rendererPane.paintComponent(g, rendererComponent, this,
                                     pos.x, pos.y, dim.width, dim.height);
      }
   }

   /**
    * Adds a listener that's notified each time the selection status of this
    * placer changes.
    */
   public void addPlacerSelectionListener(PlacerSelectionListener listener)
   {
      listeners.add(PlacerSelectionListener.class, listener);
   }

   /**
    * Removes a listener that's notified each time the selection status of this
    * placer changes.
    */
   public void removePlacerSelectionListener(PlacerSelectionListener listener)
   {
      listeners.remove(PlacerSelectionListener.class, listener);
   }

   /**
    * Notifies all listeners that the given index has been selected.
    *
    * @param idx  the selected index or -1 if nothing is selected
    */
   protected void fireItemSelected(int index)
   {
      // Get the value of the selected item
      Object value = ((index != -1) ? model.getElement(index) : null);

      // Notify all interested listeners
      PlacerSelectionEvent evt = null;
      Object[] listenerList = listeners.getListenerList();
      for (int i=listenerList.length-2; i>=0; i-=2)
      {
         if (listenerList[i] == PlacerSelectionListener.class)
         {
            // lazily instantiate the list
            if (evt == null)
            {
               evt = new PlacerSelectionEvent(this, index, value);
            }
            ((PlacerSelectionListener)listenerList[i+1]).valueChanged(evt);
         }
      }
   }

   /**
    * Helper class that handles events from the data model and updates the
    * state of the parent placer accordingly.
    */
   private class PlacerModelHandler
      implements PlacerModelListener
   {
      /**
       * Notifies this listener that the model it was listening to has changed
       * in some way.
       */
      public void placerItemsChanged(PlacerModelEvent evt)
      {
         repaint();
      }

      /**
       * Notifies this listener that items have been inserted into the placer.
       */
      public void placerItemsInserted(PlacerModelEvent evt)
      {
         repaint();
      }

      /**
       * Notifies this listener that items have been removed from the placer.
       */
      public void placerItemsRemoved(PlacerModelEvent evt)
      {
         // Check if the placer is now empty so that we can make sure we clear
         // the selection index.
         if (model.getSize() == 0)
         {
            selectedIndex = -1;
            fireItemSelected(selectedIndex);
         }
         repaint();
      }
   }
}

class DefaultPlacerRenderer
   extends JPanel
   implements PlacerRenderer
{
   private Placer placer;
   private int idx;
   private boolean selected;

   public Component getPlacerRendererComponent(Placer placer,
                                               Object value,
                                               boolean selected,
                                               boolean hasFocus,
                                               int idx)
   {
      this.placer = placer;
      this.idx = idx;
      this.selected = selected;
      if (selected)
      {
         setForeground(placer.getSelectionForeground());
         setBackground(placer.getSelectionBackground());
      }
      else
      {
         setForeground(placer.getSelectionBackground());
         setBackground(placer.getSelectionForeground());
      }

      return this;
   }

   public void paintComponent(Graphics g)
   {
      if (placer != null && idx >= 0)
      {
         super.paintComponent(g);
         Point pos = placer.getModel().getLocationOf(idx);
         Dimension dim = placer.getModel().getSizeOf(idx);
         g.setColor(Color.white);
         g.drawRect(0, 0, dim.width-1, dim.height-1);
         g.fillRect(0, 0, dim.width, 3);
      }
   }
}
