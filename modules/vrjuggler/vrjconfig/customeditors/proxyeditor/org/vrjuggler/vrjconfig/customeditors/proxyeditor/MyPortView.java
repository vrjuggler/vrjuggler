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

package org.vrjuggler.vrjconfig.customeditors.proxyeditor;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.net.URL;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JScrollPane;

import com.jgraph.JGraph;
import com.jgraph.graph.CellMapper;
import com.jgraph.graph.CellViewRenderer;
import com.jgraph.graph.Port;
import com.jgraph.graph.DefaultPort;
import com.jgraph.graph.PortRenderer;
import com.jgraph.graph.PortView;

public class MyPortView extends PortView 
{
   protected static ImageIcon mPortOnIcon = null;
   protected static ImageIcon mPortOffIcon = null;

   protected MyPortRenderer renderer = new MyPortRenderer();
   
   protected ElementPort port = null;

   public MyPortView(Object cell, JGraph graph, CellMapper cm) 
   {
      super(cell, graph, cm);
      if(cell instanceof ElementPort)
      {
         port = (ElementPort)cell;
      }
      mPortOnIcon = loadIcon("porton.gif");
      mPortOffIcon = loadIcon("portoff.gif");
   }
   
   private ImageIcon loadIcon(String filename)
   {
      ImageIcon insertIcon = new ImageIcon();

      String full_path = "org/vrjuggler/vrjconfig/customeditors/proxyeditor/images/" + filename;
      
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         insertIcon = new ImageIcon(loader.getResource(full_path));
      }
      catch (NullPointerException npe)
      {
         System.out.println("ERROR: Could not load" + full_path);
         return(new ImageIcon());
         // Silently ignore all errors
      }

      return insertIcon;
   }
   
   /** 
   * Returns the bounds for the port view. 
   */
   public Rectangle getBounds() 
   {
      if (mPortOnIcon != null) 
      {
         Rectangle bounds = new Rectangle(getLocation(null));
         int width = mPortOnIcon.getIconWidth();
         int height = mPortOnIcon.getIconHeight();
         bounds.x = bounds.x - width / 2;
         bounds.y = bounds.y - height / 2;
         bounds.width = width;
         bounds.height = height;
         return bounds;
      }
      return super.getBounds();
   }

   public CellViewRenderer getRenderer() 
   {
      return renderer;
   }

   public class MyPortRenderer extends PortRenderer 
   {

      public void paint(Graphics g) 
      {
         g.setColor(graph.getBackground());
         //g.setXORMode(graph.getBackground());
         if (preview) 
         {
            Dimension d = getSize();
            g.setColor(Color.red);
            g.drawRect(1, 1, d.width - 3, d.height - 3);
            g.drawRect(2, 2, d.width - 5, d.height - 5);
         }
         else
         {  
               if(port.getConnected() == true)
               {
                  mPortOnIcon.paintIcon(graph, g, 0, 0);
               }
               else
               {
                  mPortOffIcon.paintIcon(graph, g, 0, 0);
               }
         }
      }

   }
}
