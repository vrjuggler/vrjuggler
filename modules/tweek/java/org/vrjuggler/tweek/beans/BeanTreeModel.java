/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.beans;

import java.util.Enumeration;
import java.util.Iterator;
import java.util.Vector;
import javax.swing.tree.*;

/**
 * @since 1.0
 */
public class BeanTreeModel
   extends DefaultTreeModel
   implements BeanInstantiationListener
{
   public BeanTreeModel (TreeNode root)
   {
      super(root);
      BeanInstantiationCommunicator.instance().addBeanInstantiationListener( this );
   }

   public BeanTreeModel (TreeNode root, boolean askAllowsChildren)
   {
      super(root, askAllowsChildren);
      BeanInstantiationCommunicator.instance().addBeanInstantiationListener( this );
   }

   /**
    * Called by the BeanInstantiationCommunicator singleton whenever a new bean
    * is instantiated.
    */
   public void beanInstantiation( BeanInstantiationEvent evt )
   {
      Object bean = evt.getBean();
      if ( bean instanceof PanelBean ) {
         addBean( (PanelBean)bean );
      }
   }

   /**
    * Adds a new PanelBean to this model. This will extend this model to include
    * the paths this bean would like to be located.
    *
    * @param bean    the new bean to add to this model
    */
   public synchronized void addBean( PanelBean bean )
   {
      DefaultMutableTreeNode new_node = new DefaultMutableTreeNode( bean );
      for ( Iterator itr = bean.getPaths().iterator(); itr.hasNext(); ) {
         String path = (String)itr.next();
         addNodeToPath( new_node, path );
      }
   }

   /**
    * Adds the given node to the given path of the tree this model represents.
    *
    * @param new_node      the node to add
    * @param path          the path to the node in which to attach the new node
    */
   protected synchronized void addNodeToPath( DefaultMutableTreeNode new_node,
                                              String path )
   {
      DefaultMutableTreeNode root = (DefaultMutableTreeNode)getRoot();

      if ( path.startsWith("/") )
      {
         String work_path = path;

         DefaultMutableTreeNode cur_node = root;

         int begin_index = 1;
         int end_index   = path.indexOf('/', 1);

         while ( end_index != -1 || begin_index < path.length() )
         {
            if ( end_index == -1 )
            {
               end_index = path.length();
            }

            String path_element = path.substring(begin_index, end_index);

            Enumeration children = cur_node.children();
            boolean found = false;

            while ( children.hasMoreElements() )
            {
               DefaultMutableTreeNode node =
                  (DefaultMutableTreeNode) children.nextElement();

               try
               {
                  String level = (String) node.getUserObject();

                  if ( level.equals(path_element) )
                  {
                     cur_node = node;
                     found    = true;
                     break;
                  }
               }
               // This will happen if the current child is actually a BeanPanel
               // object.  Perhaps there is a better way to deal with this?
               catch (ClassCastException e)
               {
                  /* Move on. */ ;
               }
            }

            if ( ! found )
            {
               DefaultMutableTreeNode n = new DefaultMutableTreeNode(path_element);
               int[] indices = new int[1];
               indices[0] = cur_node.getChildCount();
               cur_node.add(n);
               this.nodesWereInserted(cur_node, indices);
               cur_node = n;
            }

            begin_index = end_index + 1;
            end_index   = path.indexOf('/', begin_index);
         }

         int[] indices = new int[1];
         indices[0] = cur_node.getChildCount();
         cur_node.add(new_node);
         this.nodesWereInserted(cur_node, indices);
      }
      else
      {
         System.err.println("WARNING: Invalid tree path '" + path + "'");
      }
   }

   public synchronized void addTreeModelRefreshListener (TreeModelRefreshListener l)
   {
      m_listeners.add(l);
   }

   public synchronized void removeTreeModelRefreshListener (TreeModelRefreshListener l)
   {
      m_listeners.remove(l);
   }

   public void fireTreeModelRefreshEvent ()
   {
      TreeModelRefreshEvent e = new TreeModelRefreshEvent(this);

      TreeModelRefreshListener l = null;
      Vector listeners;

      synchronized (this)
      {
         listeners = (Vector) m_listeners.clone();
      }

      for ( int i = 0; i < listeners.size(); i++ )
      {
         l = (TreeModelRefreshListener) listeners.elementAt(i);
         l.treeModelRefresh(e);
      }
   }

   private Vector m_listeners = new Vector();
}
