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

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import javax.swing.*;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;


public class JugglerWindowFrame
   extends JInternalFrame
{
   public JugglerWindowFrame(String title, Dimension resolution,
                             Dimension desktopSize, ConfigContext ctx,
                             ConfigElement elt)
   {
      super(title, true, true, true, false);

      mResolution = resolution;
      mDesktopSize = desktopSize;
      mContext = ctx;
      mElement = elt;

      this.addComponentListener(new JugglerWindowFrame_this_componentAdapter(this));
   }

   /**
    * Changes the location of this internal frame based on the new resolution.
    * @param resolution Dimension
    */
   public void updateResolution(Dimension resolution)
   {
      mResolution = resolution;
      placeMyself();
   }

   public void updateDesktopSize(Dimension size)
   {
      mDesktopSize = size;
      placeMyself();
   }

   public ConfigElement getConfiguration()
   {
      return mElement;
   }

   protected void placeMyself()
   {
      int cfg_size_x = ((Integer) mElement.getProperty("size", 0)).intValue();
      int cfg_size_y = ((Integer) mElement.getProperty("size", 1)).intValue();
      Point size = resolutionToDesktop(new Point(cfg_size_x, cfg_size_y));
      System.out.println("Setting my size to " + size);
      this.setSize(size.x, size.y);

      int cfg_origin_x = ((Integer) mElement.getProperty("origin", 0)).intValue();
      int cfg_origin_y =
         mResolution.height -
            (((Integer) mElement.getProperty("origin", 1)).intValue() +
            cfg_size_y);
      Point origin = resolutionToDesktop(new Point(cfg_origin_x,
                                                   cfg_origin_y));
      System.out.println("Setting my origin to " + origin);
      this.setLocation(origin.x, origin.y);
   }

   protected void updatePosition(Rectangle newBounds)
   {
      int y_pos = mDesktopSize.height - (newBounds.y + newBounds.height);
      Point origin = desktopToResolution(new Point(newBounds.x, y_pos));
      mElement.setProperty("origin", 0, new Integer(origin.x), mContext);
      mElement.setProperty("origin", 1, new Integer(origin.y), mContext);
   }

   protected void updateSize(Rectangle newBounds)
   {
      Point size = desktopToResolution(new Point(newBounds.width,
                                                 newBounds.height));
      mElement.setProperty("size", 0, new Integer(size.x), mContext);
      mElement.setProperty("size", 1, new Integer(size.y), mContext);
   }

   private Point desktopToResolution(Point desktopPoint)
   {
      float width_conv  = (float) mResolution.width / (float) mDesktopSize.width;
      float height_conv = (float) mResolution.height / (float) mDesktopSize.height;
//      System.out.println("To-resolution width conversion: " + width_conv);
//      System.out.println("To-resolution height conversion: " + height_conv);
      return new Point((int) ((float) desktopPoint.x * width_conv),
                       (int) ((float) desktopPoint.y * height_conv));
   }

   private Point resolutionToDesktop(Point resolutionPoint)
   {
      float width_conv  = (float) mDesktopSize.width / (float) mResolution.width;
      float height_conv = (float) mDesktopSize.height / (float) mResolution.height;
//      System.out.println("To-desktop width conversion: " + width_conv);
//      System.out.println("To-desktop height conversion: " + height_conv);
      return new Point((int) ((float) resolutionPoint.x * width_conv),
                       (int) ((float) resolutionPoint.y * height_conv));
   }

   protected ConfigContext mContext = null;
   protected ConfigElement mElement = null;
   protected Dimension mDesktopSize = null;
   protected Dimension mResolution = null;

   void this_componentMoved(ComponentEvent e)
   {
//      if ( ! mMousePressed )
//      {
         updatePosition(e.getComponent().getBounds());
//      }
   }

   void this_componentResized(ComponentEvent e)
   {
//      if ( ! mMousePressed )
//      {
         Rectangle bounds = e.getComponent().getBounds();
         updateSize(bounds);
         updatePosition(bounds);
//      }
   }
}

class JugglerWindowFrame_this_componentAdapter extends ComponentAdapter
{
   private JugglerWindowFrame adaptee;

   JugglerWindowFrame_this_componentAdapter(JugglerWindowFrame adaptee)
   {
      this.adaptee = adaptee;
   }
   public void componentMoved(ComponentEvent e)
   {
      adaptee.this_componentMoved(e);
   }
   public void componentResized(ComponentEvent e)
   {
      adaptee.this_componentResized(e);
   }
}
