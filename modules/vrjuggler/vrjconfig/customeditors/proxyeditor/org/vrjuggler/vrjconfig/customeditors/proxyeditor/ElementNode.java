package org.vrjuggler.vrjconfig.customeditors.proxyeditor;

import com.jgraph.JGraph;
import com.jgraph.graph.DefaultGraphCell;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.Color;
import java.util.Map;
import java.util.Hashtable;
import java.util.List;
import java.util.ArrayList;
import javax.swing.ImageIcon;
import com.jgraph.graph.GraphConstants;
import java.util.Hashtable;

import org.vrjuggler.jccl.config.*;

class ElementNode extends DefaultGraphCell
{
   public ElementNode(Point point, ConfigElement elm)
   {
      DefaultGraphCell vertex = null;

      // Construct Vertex with no Label
//      vertex = new DefaultGraphCell("Position");
      //setUserObject("Position");
      setUserObject(elm.getName());

      ////////////////////////////////
      // Set location of input port
      Point port_pnt = new Point(0, 500);
      Dimension port_size = new Dimension(70, 85);
      Rectangle port_rect = new Rectangle(port_pnt, port_size);

      Map attrib = GraphConstants.createMap();
      GraphConstants.setOffset(attrib, port_pnt);
      GraphConstants.setBounds(attrib, port_rect);
      
      mPort.setAttributes(attrib);
      //
      ////////////////////////////////
      
      // Add one Floating Port
      add(mPort);
      
//      // Snap the Point to the Grid
//      point = graph.snap(new Point(point));
      
      // Default Size for the new Vertex
      Dimension size = new Dimension(70, 85);
      // Create a Map that holds the attributes for the Vertex
      Map map = GraphConstants.createMap();
      // Add a Bounds Attribute to the Map
      GraphConstants.setBounds(map, new Rectangle(point, size));
      // Add a Border Color Attribute to the Map
      GraphConstants.setBorderColor(map, Color.black);
      // Add a White Background
      GraphConstants.setBackground(map, Color.white);
      // Make Vertex Opaque
      GraphConstants.setOpaque(map, true);
      
      //XXX LoadIcon
      String token = elm.getDefinition().getToken();
      ImageIcon icon = getIcon(token);
      GraphConstants.setIcon(map, icon);


      

      // Construct a Map from cells to Maps (for insert)
      //Hashtable attributes = new Hashtable();
      // Associate the Vertex with its Attributes
      //attributes.put(this, map);
      
      setAttributes(map);
      // Insert the Vertex and its Attributes (can also use model)
      //graph.getGraphLayoutCache().insert(
      //   new Object[] { vertex },
      //   attributes,
      //   null,
      //   null,
      //   null);
   }

   public void addPort(String name)
   {
      System.out.println("addPort - " + getUserObject() + " : " + name);
      ElementPort temp = new ElementPort();
      temp.setUserObject(name);
      mPorts.put(name, temp);
      
      mLayout.layoutPorts(this);
      
      add(temp);
   }
   
   public ElementPort getPort(String prop_name)
   {
      return((ElementPort)mPorts.get(prop_name));
   }
   public ElementPort getPort()
   {
      return mPort;
   }

   public Map getPorts()
   {
      return mPorts;
   }
   
   //////////////////////////////////////////////
   private ImageIcon getIcon(String token)
   {
      if(token.equals("digital_proxy"))
      {
         return(loadIcon("digital642.jpg")); 
      }
      else if(token.equals("analog_proxy"))
      {
         return(loadIcon("analog642.jpg")); 
      }
      else if(token.equals("position_proxy"))
      {
         return(loadIcon("position642.jpg")); 
      }
      else if(token.equals("event_window_proxy"))
      {
         return(loadIcon("event_window642.png")); 
      }
      else if(token.equals("gesture_proxy"))
      {
         return(loadIcon("hand1642.png")); 
      }
      else if(token.equals("glove_proxy"))
      {
         return(loadIcon("glove2.png")); 
      }
      else if(token.equals("alias"))
      {
         return(loadIcon("alias64.png")); 
      }
      else
      {
         return(loadIcon("cluster764.jpg"));
      }
   }
  
   //////////////////////////////////////////////
   
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

   protected ElementPort mPort = new ElementPort();
   
   private RightBorderLayout mLayout = new RightBorderLayout();
   private Map mPorts = new Hashtable();
}

