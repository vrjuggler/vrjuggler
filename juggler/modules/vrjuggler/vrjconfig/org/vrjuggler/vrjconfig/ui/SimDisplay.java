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

import java.awt.Dimension;
import java.awt.Point;
import java.beans.*;
import java.io.Serializable;

/**
 * Helper class that describes the property of a display in simulator mode.
 * This is implemented as a bean to make probing its properties easier.
 */
public class SimDisplay
   implements Serializable
{
   public SimDisplay()
   {
      this(50, 50);
   }

   public SimDisplay(int width, int height)
   {
      this.width = width;
      this.height = height;
      this.propSupport = new PropertyChangeSupport(this);
   }

   public String getName()
   {
      return name;
   }

   public void setName(String name)
   {
      String oldValue = this.name;
      this.name = name;
      propSupport.firePropertyChange("name", oldValue, name);
   }

   public boolean contains(int x, int y)
   {
      return (x > 0 && y > 0 && x < width && y < height);
   }

   public void setX(int x)
   {
      int oldValue = this.x;
      this.x = x;
      propSupport.firePropertyChange("x", oldValue, x);
   }

   public int getX()
   {
      return x;
   }

   public void setY(int y)
   {
      int oldValue = this.y;
      this.y = y;
      propSupport.firePropertyChange("y", oldValue, y);
   }

   public int getY()
   {
      return y;
   }

   public void setWidth(int width)
   {
      int oldValue = this.width;
      this.width = width;
      propSupport.firePropertyChange("width", oldValue, width);
   }

   public int getWidth()
   {
      return width;
   }

   public void setHeight(int height)
   {
      int oldValue = this.height;
      this.height = height;
      propSupport.firePropertyChange("height", oldValue, height);
   }

   public int getHeight()
   {
      return height;
   }

   public void addPropertyChangeListener(PropertyChangeListener listener)
   {
      propSupport.addPropertyChangeListener(listener);
   }

   public void removePropertyChangeListener(PropertyChangeListener listener)
   {
      propSupport.removePropertyChangeListener(listener);
   }

   /**
    * The name of this display.
    */
   private String name = "Untitled";

   /**
    * The x position of this display.
    */
   private int x = 0;

   /**
    * The y position of this display.
    */
   private int y = 0;

   /**
    * The width of this display.
    */
   private int width = 0;

   /**
    * The height of this display.
    */
   private int height = 0;

   /**
    * Helper class for dealing with property change events.
    */
   private transient PropertyChangeSupport propSupport;
}
