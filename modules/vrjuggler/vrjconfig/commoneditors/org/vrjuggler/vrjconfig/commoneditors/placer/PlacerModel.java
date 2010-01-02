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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.placer;

import java.awt.Dimension;
import java.awt.Point;

/**
 * Interface that defines a suitable data model for a Placer.
 */
public interface PlacerModel
{
   /**
    * Gets the object at the given index.
    */
   public Object getElement(int idx);

   /**
    * Gets the topmost element that contains the given point. Returns null if no
    * objects contain the given point.
    */
   public Object getElementAt(Point pt);

   /**
    * Gets the index of the topmost element that contains the given point.
    * Returns -1 if no objects contain the given point.
    */
   public int getIndexOfElementAt(Point pt);

   /**
    * Gets the size of the element at the given index.
    */
   public Dimension getSizeOf(int idx);

   /**
    * Sets the size of the element at the given index.
    */
   public void setSizeOf(int idx, Dimension size);

   /**
    * Gets the location of the upper-left corner of the element at the given
    * index.
    */
   public Point getLocationOf(int idx);

   /**
    * Sets the location of the upper-left corner of the element at the given
    * index.
    */
   public void setLocationOf(int idx, Point pt);

   /**
    * Moves the object at the selected index to the front. In other words, it
    * is moved from its current index to index 0.
    */
   public void moveToFront(int idx);

   /**
    * Gets the number of objects in this data model.
    */
   public int getSize();

   /**
    * Gets the size of the desktop in this data model.
    */
   public Dimension getDesktopSize();

   /**
    * Sets the size of the desktop in this data model.
    */
   public void setDesktopSize(Dimension desktopSize);

   /**
    * Adds a listener that's notified each time this data model changes.
    */
   public void addPlacerModelListener(PlacerModelListener listener);

   /**
    * Removes a listener that's notified each time this data model changes.
    */
   public void removePlacerModelListener(PlacerModelListener listener);
}
