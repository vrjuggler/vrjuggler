package org.vrjuggler.vrjconfig.customeditors.proxyeditor;
import java.util.Map;
import java.util.Collection;
import java.util.Iterator;
import java.awt.Rectangle;
import java.awt.Point;
import org.vrjuggler.jccl.config.*;
import com.jgraph.graph.GraphConstants;
import com.jgraph.graph.DefaultPort;

public class DiceLayout
{
   public void layoutPorts(ElementNode node)
   {
      mRect = GraphConstants.getBounds(node.getAttributes());      
      
      Collection ports = node.getPorts().values();
      // (1/3)+1=1
      // (2/3)+1=1
      // (3/3)+1=2

      // (4/3)+1=2    4-(2*2)=0
      // (5/3)+1=2    5-(2*2)=1
      // (6/3)+1=3    6-(2*2)=2
      
      // (11/3)+1=4   11-(4*2)=3    L=4   C=3   R=4
      
      if(ports.size() == 1)
      {
      }
      else if(ports.size() == 2 || ports.size() == 3)
      {      
         float height = (float)mRect.getHeight() * 7;
         int dist = (int)(height/3); 
         
         Point pnt = new Point(0, dist);
         
         Map attrib = GraphConstants.createMap();
         GraphConstants.setOffset(attrib, pnt);
         DefaultPort port = (DefaultPort)ports.toArray()[0];
         port.changeAttributes(attrib);
         
         Point pnt_two = new Point(0, -dist);
         
         Map attrib_two = GraphConstants.createMap();
         GraphConstants.setOffset(attrib_two, pnt_two);
         DefaultPort port_two = (DefaultPort)ports.toArray()[ports.size()-1];
         port_two.changeAttributes(attrib_two);
      }
      else
      {
         System.out.println("# of ports: " + ports.size());
         int side_num = (ports.size()/3)+1;
         int center_num = ports.size()-(side_num*2);
         // Layout left side
         
         float width = (float)mRect.getWidth() * 7;
         int x_delta = (int)width/3;
         
         int total_count = 0;
         
         // Layout the Left Column
         for(int left = 0 ; left < side_num ; left++)
         {
            Point pnt = getPoint(side_num, left, x_delta);
            Map attrib = GraphConstants.createMap();
            GraphConstants.setOffset(attrib, pnt);
            DefaultPort port = (DefaultPort)ports.toArray()[total_count];
            port.changeAttributes(attrib);

            total_count++;
         }
         // Layout the Center Column
         for(int center = 0 ; center < center_num ; center++)
         {
            Point pnt = getPoint(center_num, center, 2*x_delta);
            Map attrib = GraphConstants.createMap();
            GraphConstants.setOffset(attrib, pnt);
            DefaultPort port = (DefaultPort)ports.toArray()[total_count];
            port.changeAttributes(attrib);

            total_count++;
         }
         // Layout the Right Column
         for(int right = 0 ; right < side_num ; right++)
         {
            Point pnt = getPoint(side_num, right, 3*x_delta);
            Map attrib = GraphConstants.createMap();
            GraphConstants.setOffset(attrib, pnt);
            DefaultPort port = (DefaultPort)ports.toArray()[total_count];
            port.changeAttributes(attrib);

            total_count++;
         }
         
      }
   }

   private Point getPoint(int column_total, int num, int x_pos)
   {
      System.out.println("getPoint: column_total= " + column_total + " num= " +
            num + " x_pos= " + x_pos);
      float height = (float)mRect.getHeight() * 7;
      float dist = height/(column_total+1); 
      
      float offset = (num+1)*dist + (height/2);
      Point pnt = new Point((int)x_pos, (int)offset);
      System.out.println("Point: " + pnt);

      return(pnt);
   }

   Rectangle mRect = null;
}
