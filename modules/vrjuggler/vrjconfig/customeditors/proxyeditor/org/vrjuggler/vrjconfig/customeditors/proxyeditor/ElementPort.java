package org.vrjuggler.vrjconfig.customeditors.proxyeditor;

import com.jgraph.JGraph;
import com.jgraph.graph.DefaultGraphCell;
import java.awt.Graphics;
import java.awt.Point;
import com.jgraph.graph.DefaultPort;
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

class ElementPort extends DefaultPort
{
   public ElementPort()
   {
   }
   public void setConnected(boolean val)
   {
      mConnected = val;
   }
   public boolean getConnected()
   {
      return mConnected;
   }
   public boolean addEdge(Object edge)
   {
      mConnected = true;
      return(super.addEdge(edge));
   }
   public boolean removeEdge(Object edge)
   {
      mConnected = false;
      return(super.removeEdge(edge));
   }
 
   private boolean mConnected = false;
}
