/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
 * This class serves as the data model used to store the hierarchy of Panel
 * Beans loaded at any given point during execution.  The leaves of the tree
 * contain the actual Panel Bean objects while the non-leaves provide the
 * "path" to the individual Panel Beans.  Viewer Beans use this data model to
 * complete the Model View design pattern, and thus the Viewer Beans may
 * provide any representation they wish of the data contained herein.
 */
public class BeanTreeModel
   extends DefaultTreeModel
   implements BeanRegistrationListener
{
   public BeanTreeModel (TreeNode root)
   {
      super(root);
      BeanRegistry.instance().addBeanRegistrationListener( this );
   }

   public BeanTreeModel (TreeNode root, boolean askAllowsChildren)
   {
      super(root, askAllowsChildren);
      BeanRegistry.instance().addBeanRegistrationListener( this );
   }

   /**
    * Called by the BeanRegistry singleton whenever a new bean is registered
    * with it.
    *
    * @param evt     the event describing the bean that got registered
    */
   public void beanRegistered( BeanRegistrationEvent evt )
   {
      TweekBean bean = evt.getBean();
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
    * Gets the node that corresponds to the given path in the tree this model
    * represents.
    *
    * @param path          the path to the node to retrieve
    *
    * @return  the node at the path if found; null if the path is invalid
    */
   public synchronized TreeNode getNode(String path)
   {
      DefaultMutableTreeNode root = (DefaultMutableTreeNode)getRoot();
      return getNode(root, path);
   }

   /**
    * A recursive helper method for getNode(String). This will recursively walk
    * through the given path from the given parent node looking for the node
    * corresponding to the path.
    *
    * @param parent        the parent node for the path
    * @param path          the path corresponding to the parent
    */
   protected synchronized TreeNode getNode(DefaultMutableTreeNode parent,
                                           String path)
   {
      // If the path is null, we found the node we're looking for
      if (path == null)
      {
         return parent;
      }

      // Make sure this part of the path is valid
      if (path.startsWith("/"))
      {
         int begin_index = 1;
         int end_index = path.indexOf('/', 1);
         String path_element, path_remainder;

         // Check if this is the last node in the path
         if (end_index == -1)
         {
            path_element = path.substring(begin_index);
            path_remainder = null;
         }
         // Not the last node in the path
         else
         {
            path_element = path.substring(begin_index, end_index);
            path_remainder = path.substring(end_index);
         }

         // Find the child that corresponds to the current path_element
         for (Enumeration children = parent.children(); children.hasMoreElements(); )
         {
            DefaultMutableTreeNode node =
                  (DefaultMutableTreeNode)children.nextElement();
            Object value = node.getUserObject();

            // Figure out what type of node this is
            if (value instanceof String)
            {
               // Node is just a sub-path
               if (((String)value).equals(path_element))
               {
                  return getNode(node, path_remainder);
               }
            }
            else if (value instanceof PanelBean)
            {
               // Node is a PanelBean (leaf node)
               PanelBean bean = (PanelBean)value;
               if (bean.getName().equals(path_element))
               {
                  return getNode(node, path_remainder);
               }
            }
            else
            {
               System.err.println("WARNING: Invalid node '" + node + "'");
            }
         }
         // Couldn't find that child
         System.err.println("WARNING: Invalid tree path '" + path + "'");
         return null;
      }
      else
      {
         System.err.println("WARNING: Invalid tree path '" + path + "'");
         return null;
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
}
