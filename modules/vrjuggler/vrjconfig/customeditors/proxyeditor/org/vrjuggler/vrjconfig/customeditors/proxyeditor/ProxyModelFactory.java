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

public class ProxyModelFactory
{
   public ProxyModelFactory(ConfigContext ctx)
   {
      mBroker = new ConfigBrokerProxy();
      mModel = new ProxyGraphModel();
 
      getAllowedElements(ctx);
      
      // For each element
      for(Iterator itr = mAliases.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         handleConfigElementPointer(elm, "proxy");
      } 
      // For each element
      for(Iterator itr = mProxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         handleConfigElementPointer(elm, "device");
      }
   }  

   private void getAllowedElements(ConfigContext ctx)
   {
      // Get the DefinitionRepository
      ConfigDefinitionRepository repos = mBroker.getRepository();
      
      // Get a list of all allowed types for the proxy property of the alias element
      ConfigDefinition alias_cfg_def = repos.get("alias");
      PropertyDefinition alias_prop_def = alias_cfg_def.getPropertyDefinition("proxy");
      mAllowedProxies = alias_prop_def.getAllowedTypes();

      // Fill mAllowedProxies with all children of proxy.
      ConfigDefinition proxy_def = repos.get("proxy");
      mAllowedProxies = proxy_def.getSubDefinitions();

      // Fill mAllowedDevices with all children of input_device.
      ConfigDefinition input_device_def = repos.get("input_device");
      mAllowedDevices = input_device_def.getSubDefinitions();

      // Sort all needed elements by type
      java.util.List elements = mBroker.getElements(ctx); 
      for(Iterator itr = elements.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         testIfNeeded(elm);
      }
   }
   
   
   private boolean testIfNeeded(ConfigElement elm)   
   {
         String token = elm.getDefinition().getToken();
         System.out.println("Testing token: " + token);
         if(token.equals("alias"))
         {
            mAliases.add(elm);
            addElementNode(elm);
            findEmbeddedElements(elm, true);
            return(true);
         }
         else if(mAllowedProxies.contains(elm.getDefinition()))
         {
            mProxies.add(elm);
            addElementNode(elm);
            findEmbeddedElements(elm, true);
            return(true);
         }
         else if(mAllowedDevices.contains(elm.getDefinition()))
         {
            mDevices.add(elm);
            addElementNode(elm);
            findEmbeddedElements(elm, true);
            return(true);
         }
         else
         {
            findEmbeddedElements(elm, false);
            return(false);
         }
   }     

   private void findEmbeddedElements(ConfigElement elm, boolean connect_to_parent)
   {
      // Get all properties
      java.util.List properties = elm.getDefinition().getPropertyDefinitions();
      // For each property
      for(Iterator prop_itr = properties.iterator() ; prop_itr.hasNext() ; )
      {
         PropertyDefinition property = (PropertyDefinition)prop_itr.next();
         Class property_class = property.getType();
         String property_name = property.getName();
        
         // If an embedded element
         if(ConfigElement.class == property_class)
         {
            System.out.println("Embedded element"); 
            
            List property_values= elm.getPropertyValues(property_name);
            for(Iterator emb_itr = property_values.iterator() ; emb_itr.hasNext() ; )
            {
               ConfigElement embedded_elm = (ConfigElement)emb_itr.next();
               // If parent was added, and the embedded element was also added.
               // Connect them.
               if (connect_to_parent && testIfNeeded(embedded_elm))
               {
                  //TODO: get argument true if we want to connect to parent
                  ElementNode parent = (ElementNode)mNodes.get(elm.getName());
                  ElementNode child = (ElementNode)mNodes.get(embedded_elm.getName());
               
                  // Connect the two elements
                  connectEmbedded(parent.getPort(), child.getPort(),
                        property_name);
               }
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

   
   private void addElementNode(ConfigElement elm)
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
      source.setConnected(true);
      target.setConnected(true);


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
      GraphConstants.setLineStyle(map, GraphConstants.STYLE_QUADRATIC /*STYLE_BEZIER*/);
      GraphConstants.setBendable(map, true);
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

   public void connectEmbedded(ElementPort source, ElementPort target, String property_name) 
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
   
   private List mAliases = new ArrayList();
   private List mProxies = new ArrayList();
   private List mDevices = new ArrayList();
   private List mAllowedProxies = null;
   private List mAllowedDevices = new ArrayList();
   private ProxyGraphModel mModel = null;
   private ConfigBroker mBroker = null;
   private Map mNodes = new Hashtable();
}
