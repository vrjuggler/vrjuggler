package org.vrjuggler.vrjconfig.customeditors.proxyeditor;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.net.URL;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Map;
import java.util.List;
import java.util.Iterator;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;
import javax.swing.event.UndoableEditEvent;

import com.jgraph.JGraph;
import com.jgraph.event.GraphSelectionEvent;
import com.jgraph.event.GraphSelectionListener;
import com.jgraph.graph.BasicMarqueeHandler;
import com.jgraph.graph.CellMapper;
import com.jgraph.graph.CellView;
import com.jgraph.graph.ConnectionSet;
import com.jgraph.graph.DefaultEdge;
import com.jgraph.graph.DefaultGraphCell;
import com.jgraph.graph.DefaultGraphModel;
import com.jgraph.graph.DefaultPort;
import com.jgraph.graph.Edge;
import com.jgraph.graph.EdgeView;
import com.jgraph.graph.GraphConstants;
import com.jgraph.graph.GraphModel;
import com.jgraph.graph.GraphUndoManager;
import com.jgraph.graph.ParentMap;
import com.jgraph.graph.Port;
import com.jgraph.graph.PortView;
import org.vrjuggler.jccl.editors.CustomEditor;
import org.vrjuggler.jccl.editors.CustomEditorRegistry;
import org.vrjuggler.jccl.config.*;

public class ModelFactory
{
   public ModelFactory(ConfigContext ctx)
   {
      mBroker = new ConfigBrokerProxy();
      mModel = new ProxyGraphModel();
      
      // Get all elements
      java.util.List elements = mBroker.getElements(ctx); 

      // Insert each element
      for(Iterator itr = elements.iterator() ; itr.hasNext() ; )
      {
         insert((ConfigElement)itr.next());
      }

      // For each element
      for(Iterator itr = elements.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         addConfigElement(elm); 
      }
      
      // For each element
      for(Iterator itr = elements.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         linkConfigElement(elm); 
      }
   }

   private void addConfigElement(ConfigElement elm)
   {
      //Insert Element
      insert(elm);

      // Get all properties
      java.util.List properties = elm.getDefinition().getPropertyDefinitions();
      // For each property
      for(Iterator prop_itr = properties.iterator() ; prop_itr.hasNext() ; )
      {
         PropertyDefinition property = (PropertyDefinition)prop_itr.next();
         Class property_class = property.getType();
         String property_name = property.getName();
        
         // If the property is a pointer to another element...
         //if(ConfigElementPointer.class == property_class)
         //{
         //   handleConfigElementPointer(elm, property_name);
         //} // If the property is an embedded element
         if(ConfigElement.class == property_class)
         {
            System.out.println("Embedded element"); 
            
            List property_values= elm.getPropertyValues(property_name);
            for(Iterator emb_itr = property_values.iterator() ; emb_itr.hasNext() ; )
            {
               ConfigElement embedded_elm = (ConfigElement)emb_itr.next();
               addConfigElement(embedded_elm);
               
               ElementNode parent = (ElementNode)mNodes.get(elm.getName());
               ElementNode child = (ElementNode)mNodes.get(embedded_elm.getName());
               
               // Connect the two elements
               connectEmbedded(parent.getPort(), child.getPort(),
                     property_name);
            }
         }
      }
   }

   private void linkConfigElement(ConfigElement elm)
   {
      // Get all properties
      java.util.List properties = elm.getDefinition().getPropertyDefinitions();
      // For each property
      for(Iterator prop_itr = properties.iterator() ; prop_itr.hasNext() ; )
      {
         PropertyDefinition property = (PropertyDefinition)prop_itr.next();
         Class property_class = property.getType();
         String property_name = property.getName();
        
         // If the property is a pointer to another element...
         if(ConfigElementPointer.class == property_class)
         {
            handleConfigElementPointer(elm, property_name);
         }
         else if(ConfigElement.class == property_class)
         {
            System.out.println("Embedded element"); 
            
            List property_values= elm.getPropertyValues(property_name);
            for(Iterator emb_itr = property_values.iterator() ; emb_itr.hasNext() ; )
            {
               ConfigElement embedded_elm = (ConfigElement)emb_itr.next();
               linkConfigElement(embedded_elm);
            }
         } 
      }
   }
   
   ////////////////////////////////////////////
   private void handleConfigElementPointer(ConfigElement elm, String
         property_name)
   {
      List property_values= elm.getPropertyValues(property_name);
      for(int i = 0 ; i < property_values.size() ; i++)
      {
         String value = ((ConfigElementPointer)property_values.get(i)).toString();

         ElementNode start = (ElementNode)mNodes.get(elm.getName());
         
         String num_as_string = new Integer(i).toString();
         start.addPort(property_name + num_as_string);
           
         if(null == start)
         {
            System.out.println("Could not find a starting device by the name of: " + elm.getName());
         }
         else
         {
            ElementNode end = (ElementNode)mNodes.get(value);
            if(null == end)
            {
               System.out.println("Could not find an ending device by the name of: " + value);
               System.out.println(mNodes.toString());
            }
            else
            {
               connect(start.getPort(property_name + num_as_string), end.getPort(), property_name);
            }
         }
      }
   }
   /////////////////////////////////////////

   
   private void insert(ConfigElement elm)
   {
      ElementNode element_node = new ElementNode(getNextPoint(), elm);
      
      String name = elm.getName();
      
      // Snap the Point to the Grid
      //point = graph.snap(new Point(point));
      
      mNodes.put(name, element_node);
      
      System.out.println("Adding element: " + name);
      
      Map attrib_map = element_node.getAttributes();
      //System.out.println("Testing" + attributes); 
      
      // Construct a Map from cells to Maps (for insert)
      Hashtable attributes = new Hashtable();
      // Associate the Vertex with its Attributes
      attributes.put(element_node, attrib_map);
      
      mModel.insert(
         new Object[] { element_node },
         attributes,
         null,
         null,
         null);
   }

     // Insert a new Edge between source and target
   public void connect(ElementPort source, ElementPort target, String property_name) 
   {
      //source.setConnected(true);
      //target.setConnected(true);
      
      // Connections that will be inserted into the Model
      ConnectionSet cs = new ConnectionSet();
      // Construct Edge with no label
      DefaultEdge edge = new DefaultEdge();

      edge.setUserObject(property_name);
      // Create Connection between source and target using edge
      cs.connect(edge, source, target);
      // Create a Map thath holds the attributes for the edge
      Map map = GraphConstants.createMap();
      // Add a Line End Attribute
      GraphConstants.setLineEnd(map, GraphConstants.ARROW_SIMPLE);
      // Construct a Map from cells to Maps (for insert)
      Hashtable attributes = new Hashtable();
      // Associate the Edge with its Attributes
      attributes.put(edge, map);
      // Insert the Edge and its Attributes
      mModel.insert(
         new Object[] { edge },
         attributes,
         cs,
         null,
         null);
   }


   public void connectEmbedded(Port source, Port target, String property_name) 
   {
      // Connections that will be inserted into the Model
      ConnectionSet cs = new ConnectionSet();
      // Construct Edge with no label
      DefaultEdge edge = new DefaultEdge();

      edge.setUserObject(property_name);
      // Create Connection between source and target using edge
      cs.connect(edge, source, target);
      // Create a Map thath holds the attributes for the edge
      Map map = GraphConstants.createMap();
      // Add a Line End Attribute
      GraphConstants.setLineEnd(map, GraphConstants.ARROW_SIMPLE);
      // Set the color differently since this is an embedded element
      GraphConstants.setLineColor(map, new Color(0.0f,0.0f,1.0f));
      GraphConstants.setLineWidth(map, 3.0f);
      // Construct a Map from cells to Maps (for insert)
      Hashtable attributes = new Hashtable();
      // Associate the Edge with its Attributes
      attributes.put(edge, map);
      // Insert the Edge and its Attributes
      mModel.insert(
         new Object[] { edge },
         attributes,
         cs,
         null,
         null);
   }
   
   public GraphModel getModel()
   {
      return mModel;
   }
   
   private static int x = 0;
   private static int y = 0;
   
   private static Point getNextPoint()
   {
      if (x > 500)
      {
         y += 100;
         x = 0;
      }
      else
      {
         x += 100;
      }
      return(new Point(x, y));
   }
   
   private ProxyGraphModel mModel = null;
   private ConfigBroker mBroker = null;
   private Map mNodes = new Hashtable();
}
