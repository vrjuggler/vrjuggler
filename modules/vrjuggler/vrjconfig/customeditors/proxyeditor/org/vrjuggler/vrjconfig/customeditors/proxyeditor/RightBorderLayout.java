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
