package org.vrjuggler.vrjconfig.customeditors.proxyeditor;
import java.util.Map;
import java.util.Collection;
import java.util.Iterator;
import java.awt.Rectangle;
import java.awt.Dimension;
import java.awt.Point;
import org.vrjuggler.jccl.config.*;
import com.jgraph.graph.GraphConstants;
import com.jgraph.graph.DefaultPort;

public class RightBorderLayout
{
   public void layoutPorts(ElementNode node)
   {
      // We will find the correct location for each out port by finding the
      // distance between each node. We do this by dividing the total height by
      // the # of ports + 1, this will give us the gap between the nodes and the
      // border. We must also 
      
      Collection ports = node.getPorts().values();
          
      int diff = 1000/(ports.size()+1);

      for(int i = 0 ; i < ports.size() ; i++)
      {
         // Get the port that we want to layout
         ElementPort port = (ElementPort)ports.toArray()[i];
         
         // Set the point that we want to offset to
         Point port_pnt = new Point(1000, diff*(i+1)); 
         Dimension port_size = new Dimension(70, 85);
         Rectangle port_rect = new Rectangle(port_pnt, port_size);

         // Set the offset attribute
         Map attrib = port.getAttributes();
         GraphConstants.setOffset(attrib, port_pnt);
         GraphConstants.setBounds(attrib, port_rect);
         port.setAttributes(attrib);
      }
   }
}
