/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
import java.awt.Cursor;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
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
    * Drag type where the user hasn't selected anything making dragging invalid.
    */
   private static final int INVALID = 0;

   /**
    * Drag type for moving the component.
    */
   private static final int MOVE = 1;

   /**
    * Drag type for resizing the component.
    */
   private static final int RESIZE = 2;

   /**
    * The current drag mode.
    */
   private int dragMode = INVALID;

   /**
    * The resize direction. This can be any logical combindation of the
    * Rectangle OUT_XXX types
    */
   private int resizeDirection = Rectangle.OUT_LEFT;

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
         public Dimension getDesktopSize() { return new Dimension(0,0); }
         public void setDesktopSize(Dimension d) { }
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
      boolean old = this.allowResize;
      this.allowResize = allowResize;
      firePropertyChange("allowResize", old, this.allowResize);
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
      boolean old = this.allowMove;
      this.allowMove = allowMove;
      firePropertyChange("alloveMove", old, this.allowMove);
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
    * Converts the given point in desktop coordinates to component coordinates.
    *
    * @param ptDesktop     the point in desktop coordinates
    *
    * @return  the point in component coordinates
    */
   protected Point desktopToComponent(Point ptDesktop)
   {
      Dimension compSize = getSize();
      Dimension desktopSize = getModel().getDesktopSize();
      float scale_x = (float)compSize.width / desktopSize.width;
      float scale_y = (float)compSize.height / desktopSize.height;

      return new Point((int)((float)ptDesktop.x * scale_x),
                       (int)((float)ptDesktop.y * scale_y));
   }

   /**
    * Converts the given point in component coordinates to desktop coordinates.
    *
    * @param ptComponent   the point in component coordinates
    *
    * @return  the point in desktop coordinates
    */
   protected Point componentToDesktop(Point ptComponent)
   {
      Dimension compSize = getSize();
      Dimension desktopSize = getModel().getDesktopSize();
      float scale_x = (float)desktopSize.width / compSize.width;
      float scale_y = (float)desktopSize.height / compSize.height;

      return new Point((int)((float)ptComponent.x * scale_x),
                       (int)((float)ptComponent.y * scale_y));
   }

   /**
    * Converts the given dimension in desktop coordiantes to component coordinates.
    *
    * @param dimDesktop    the dimension in desktop coordinates
    *
    * @return  the dimension in component coordinates
    */
   protected Dimension desktopToComponent(Dimension dimDesktop)
   {
      Dimension compSize = getSize();
      Dimension desktopSize = getModel().getDesktopSize();
      float scale_x = (float)compSize.width / desktopSize.width;
      float scale_y = (float)compSize.height / desktopSize.height;

      return new Dimension((int)((float)dimDesktop.width * scale_x),
                           (int)((float)dimDesktop.height * scale_y));
   }

   /**
    * Converts the given dimension in component coordinates to desktop coordinates.
    *
    * @param dimComponent     the dimension in component coordinates
    *
    * @return  the dimension in desktop coordinates
    */
   protected Dimension componentToDesktop(Dimension dimComponent)
   {
      Dimension compSize = getSize();
      Dimension desktopSize = getModel().getDesktopSize();
      float scale_x = (float)desktopSize.width / compSize.width;
      float scale_y = (float)desktopSize.height / compSize.height;

      return new Dimension((int)((float)dimComponent.width * scale_x),
                           (int)((float)dimComponent.height * scale_y));
   }

   /**
    * Initializes UI components and sets up listeners.
    */
   private void jbInit() throws Exception
   {
      this.setLayout(null);
      this.setOpaque(true);
      this.addMouseListener(new MouseAdapter()
      {
         public void mousePressed(MouseEvent e)
         {
            this_mousePressed(e);
         }
      });
      this.addMouseMotionListener(new MouseMotionAdapter()
      {
         public void mouseDragged(MouseEvent e)
         {
            this_mouseDragged(e);
         }

         public void mouseMoved(MouseEvent e)
         {
            this_mouseMoved(e);
         }
      });
      this.add(rendererPane);
   }

   /**
    * Handles a mouse-press action.
    */
   void this_mousePressed(MouseEvent e)
   {
      Point componentPt = e.getPoint();
      Point desktopPt = componentToDesktop(componentPt);

//      System.out.println("Mouse Clicked!");
//      System.out.println("\tcomponentPt: "+componentPt);
//      System.out.println("\tcomponentSize: "+getSize());
//      System.out.println("\tdesktopPt: "+desktopPt);
//      System.out.println("\tdesktopSize: "+getModel().getDesktopSize());

      // If this is the start of a resize action, the point may be outside the
      // bounds of the object, but we want to give them a bit of a larger region
      // to hit. In such a case, don't try to select a different window.
      if (dragMode != RESIZE)
      {
         // Find the display that has been selected
         int idx = getModel().getIndexOfElementAt(desktopPt);
         if (idx == -1)
         {
            selectedIndex = -1;
         }
         else
         {
            // Move the selected element up to the front
            getModel().moveToFront(idx);
            selectedIndex = 0;
            updateCursor(componentPt, getBoundsFor(selectedIndex));
         }
         repaint();
         fireItemSelected(selectedIndex);
      }
      lastMouse = componentPt;
   }

   /**
    * Handles dragging of the selected object around.
    */
   void this_mouseDragged(MouseEvent e)
   {
      if (selectedIndex != -1)
      {
         // Figure out the distance moved
         Point componentPt = e.getPoint();
         Point componentDiff = new Point(componentPt.x - lastMouse.x,
                                         componentPt.y - lastMouse.y);
         Point desktopDiff = componentToDesktop(componentDiff);

         // User wants to move the selected object
         if (dragMode == MOVE)
         {
            // Move the selected display along
            Point newDesktopPos = new Point(getModel().getLocationOf(selectedIndex));
            newDesktopPos.translate(desktopDiff.x, desktopDiff.y);

            // Make sure we don't allow the user to drag a display outside of the
            // viewable area.
            Dimension desktopObjectSize = getModel().getSizeOf(selectedIndex);
            Dimension desktopSize = getModel().getDesktopSize();
            if (newDesktopPos.x + desktopObjectSize.width > desktopSize.width)
            {
               newDesktopPos.x = desktopSize.width - desktopObjectSize.width;
            }
            if (newDesktopPos.y + desktopObjectSize.height > desktopSize.height)
            {
               newDesktopPos.y = desktopSize.height - desktopObjectSize.height;
            }
            if (newDesktopPos.x < 0)
            {
               newDesktopPos.x = 0;
            }
            if (newDesktopPos.y < 0)
            {
               newDesktopPos.y = 0;
            }
            getModel().setLocationOf(selectedIndex, newDesktopPos);

            lastMouse = componentPt;
            repaint();
         }
         // User want to resize the current object
         else if (dragMode == RESIZE)
         {
            Point oldDesktopPos = getModel().getLocationOf(selectedIndex);
            Dimension oldDesktopSize = getModel().getSizeOf(selectedIndex);
            Point newDesktopPos = null;
            Dimension newDesktopSize = null;

            // Get the mouse point in desktop coordinates
            Point desktopPt = componentToDesktop(componentPt);

            switch (resizeDirection)
            {
            // left
            case Rectangle.OUT_LEFT:
               newDesktopPos = new Point(desktopPt.x, oldDesktopPos.y);
               newDesktopSize = new Dimension(oldDesktopSize.width + (oldDesktopPos.x - newDesktopPos.x),
                                              oldDesktopSize.height);
               break;
            // top left
            case (Rectangle.OUT_LEFT | Rectangle.OUT_TOP):
               newDesktopPos = new Point(desktopPt);
               newDesktopSize = new Dimension(oldDesktopSize.width + (oldDesktopPos.x - newDesktopPos.x),
                                              oldDesktopSize.height + (oldDesktopPos.y - newDesktopPos.y));
               break;
            // top
            case Rectangle.OUT_TOP:
               newDesktopPos = new Point(oldDesktopPos.x, desktopPt.y);
               newDesktopSize = new Dimension(oldDesktopSize.width,
                                              oldDesktopSize.height + (oldDesktopPos.y - newDesktopPos.y));
               break;
            // top right
            case (Rectangle.OUT_RIGHT | Rectangle.OUT_TOP):
               newDesktopPos = new Point(oldDesktopPos.x, desktopPt.y);
               newDesktopSize = new Dimension(desktopPt.x - oldDesktopPos.x,
                                              oldDesktopSize.height + (oldDesktopPos.y - newDesktopPos.y));
               break;
            // right
            case Rectangle.OUT_RIGHT:
               newDesktopPos = new Point(oldDesktopPos.x, oldDesktopPos.y);
               newDesktopSize = new Dimension(desktopPt.x - oldDesktopPos.x,
                                              oldDesktopSize.height);
               break;
            // bottom right
            case (Rectangle.OUT_RIGHT | Rectangle.OUT_BOTTOM):
               newDesktopPos = new Point(oldDesktopPos.x, oldDesktopPos.y);
               newDesktopSize = new Dimension(desktopPt.x - oldDesktopPos.x,
                                              desktopPt.y - oldDesktopPos.y);
               break;
            // bottom
            case Rectangle.OUT_BOTTOM:
               newDesktopPos = new Point(oldDesktopPos.x, oldDesktopPos.y);
               newDesktopSize = new Dimension(oldDesktopSize.width,
                                              desktopPt.y - oldDesktopPos.y);
               break;
            // bottom left
            case (Rectangle.OUT_LEFT | Rectangle.OUT_BOTTOM):
               newDesktopPos = new Point(desktopPt.x, oldDesktopPos.y);
               newDesktopSize = new Dimension(oldDesktopSize.width + (oldDesktopPos.x - newDesktopPos.x),
                                              desktopPt.y - oldDesktopPos.y);
               break;
            }

            // Update the object as necessary
            if (newDesktopSize != null)
            {
               System.out.println("Updated size to: ("+newDesktopSize.width+", "+newDesktopSize.height+")");
               getModel().setSizeOf(selectedIndex, newDesktopSize);
            }
            if (newDesktopPos != null)
            {
               System.out.println("Updated location to: ("+newDesktopPos.x+", "+newDesktopPos.y+")");
               getModel().setLocationOf(selectedIndex, newDesktopPos);
            }

            lastMouse = componentPt;
            repaint();
         }
      }
   }

   /**
    * Handles movement of the mouse inside this placer when no buttons are being
    * held down. This method will check if the mouse cursor is suitably close
    * to the edge of the currently selected item and change the cursor to allow
    * for resizing.
    */
   void this_mouseMoved(MouseEvent e)
   {
      if (selectedIndex != -1)
      {
         // Figure out where we are
         Point componentPt = e.getPoint();

         // Get the bounds of the currently selected item
         Rectangle bounds = getBoundsFor(selectedIndex);

         // Update the cursor accordingly
         updateCursor(componentPt, bounds);
      }
   }

   /**
    * Computes the bounds of the object at the given index.
    *
    * @param index   a valid index into the model
    */
   private Rectangle getBoundsFor(int index)
   {
      return new Rectangle(desktopToComponent(model.getLocationOf(selectedIndex)),
                           desktopToComponent(model.getSizeOf(selectedIndex)));
   }

   /**
    * Updates the state of the cursor for the currently selected object based
    * on the given mouse cursor position and the given bounds.
    */
   private void updateCursor(Point mousePt, Rectangle bounds)
   {
      // Compute an inner bounds rectangle 5 pixels smaller than the bounds
      // of the selected object. Note we are  being careful not to create an
      // invalid rectangle.
      Rectangle inner = new Rectangle(bounds);
      if (inner.width > 10)
      {
         inner.x += 5;
         inner.width -= 10;
      }
      if (inner.height > 10)
      {
         inner.y += 5;
         inner.height -= 10;
      }

      // Computer an outer bounds rectangle 5 pixels bigger than the bounds
      // of the selected object.
      Rectangle outer = new Rectangle(bounds);
      outer.x -= 5;
      outer.y -= 5;
      outer.width += 10;
      outer.height += 10;

      // Check if the mouse cursor is located in the non-overlapping region
      // of inner and outer.
      if (outer.contains(mousePt.x, mousePt.y) &&
          !inner.contains(mousePt.x, mousePt.y) &&
          allowResize)
      {
         // Figure out which part of the region the mouse is in
         int outcode = inner.outcode((double)mousePt.x, (double)mousePt.y);
         switch (outcode)
         {
         // left
         case Rectangle.OUT_LEFT:
            setCursor(new Cursor(Cursor.W_RESIZE_CURSOR));
            break;
         // top left
         case (Rectangle.OUT_LEFT | Rectangle.OUT_TOP):
            setCursor(new Cursor(Cursor.NW_RESIZE_CURSOR));
            break;
         // top
         case Rectangle.OUT_TOP:
            setCursor(new Cursor(Cursor.N_RESIZE_CURSOR));
            break;
         // top right
         case (Rectangle.OUT_RIGHT | Rectangle.OUT_TOP):
            setCursor(new Cursor(Cursor.NE_RESIZE_CURSOR));
            break;
         // right
         case Rectangle.OUT_RIGHT:
            setCursor(new Cursor(Cursor.E_RESIZE_CURSOR));
            break;
         // bottom right
         case (Rectangle.OUT_RIGHT | Rectangle.OUT_BOTTOM):
            setCursor(new Cursor(Cursor.SE_RESIZE_CURSOR));
            break;
         // bottom
         case Rectangle.OUT_BOTTOM:
            setCursor(new Cursor(Cursor.S_RESIZE_CURSOR));
            break;
         // bottom left
         case (Rectangle.OUT_LEFT | Rectangle.OUT_BOTTOM):
            setCursor(new Cursor(Cursor.SW_RESIZE_CURSOR));
            break;
         }

         dragMode = RESIZE;
         resizeDirection = outcode;
      }
      // Check if cursor is inside the object's bounds, but not on an edge
      else if (bounds.contains(mousePt.x, mousePt.y) && allowMove)
      {
         setCursor(new Cursor(Cursor.MOVE_CURSOR));
         dragMode = MOVE;
      }
      // Cursor is nowhere near the selected object
      else
      {
         setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
         dragMode = INVALID;
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
         Point pos = desktopToComponent(getModel().getLocationOf(i));
         Dimension dim = desktopToComponent(getModel().getSizeOf(i));
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
         selectedIndex = 0;
         fireItemSelected(0);
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
         }
         else
         {
            selectedIndex = 0;
         }
         fireItemSelected(selectedIndex);
         repaint();
      }

      /**
       * Notifies this listener that the desktop size has changed.
       */
      public void placerDesktopSizeChanged(PlacerModelEvent evt)
      {
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
//         Point pos = placer.getModel().getLocationOf(idx);
//         Dimension dim = placer.getModel().getSizeOf(idx);
         Dimension dim = getSize();
         g.setColor(Color.white);
         g.drawRect(0, 0, dim.width-1, dim.height-1);
         g.fillRect(0, 0, dim.width, 3);
      }
   }
}
