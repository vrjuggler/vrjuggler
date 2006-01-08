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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.editors;

import javax.swing.JPanel;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import info.clearthought.layout.*;

public class PropertyComponent extends JPanel
{
   public void finalize()
   {
      for (int i = 0 ; i < getComponentCount() ; i++)
      {
         Component c = getComponent(i);
         if (c instanceof PropertyComponent)
         {
            ((PropertyComponent)c).finalize();
         }
      }
      removeAll();
   }

   public Color getNextColor()
   {
      boolean inc = false;

      int r = mColor.getRed();
      int g = mColor.getGreen();
      int b = mColor.getBlue();
      float hsb[] = {0.0f,0.0f,0.0f};

      Color.RGBtoHSB(r,g,b,hsb);

      if(hsb[1] > 0.7)
      {
         inc = false;
      }
      else
      {
         inc = true;
      }

      if(inc)
      {
         hsb[1] += 0.20;
      }
      else
      {
         hsb[1] -= 0.20;
      }

      Color new_color = Color.getHSBColor(hsb[0], hsb[1], hsb[2]);
      return new_color;

      /*
      int red_val = mColor.getRed();
      if(red_val == 85)
      {
         return(new Color(124, 100, 140));
      }
      else if(red_val == 124)
      {
         return(new Color(242, 211, 118));
      }
      else if(red_val == 242)
      {
         return(new Color(85, 140, 77));
      }
      */
      //return mColor;
   }
   public void setColor(Color new_color)
   {
      mColor = new_color;
   }
   public Color getColor()
   {
      return mColor;
   }
   protected Color mColor;
}
