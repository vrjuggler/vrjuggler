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
