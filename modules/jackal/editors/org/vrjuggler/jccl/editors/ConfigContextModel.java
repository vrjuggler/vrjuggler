/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
package org.vrjuggler.jccl.editors;

import java.util.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigContextEvent;
import org.vrjuggler.jccl.config.event.ConfigContextListener;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;
import org.vrjuggler.jccl.config.event.ConfigEvent;
import org.vrjuggler.jccl.config.event.ConfigListener;

/**
 * A data model for a configuration context. For a tree view, this
 */
public class ConfigContextModel
   extends DefaultTreeModel
{
   /**
    * Creates a new, empty model.
    */
   public ConfigContextModel()
   {
      this(new ConfigContext());
   }

   /**
    * Creates a model for the given context.
    *
    * @param context       the context to model
    */
   public ConfigContextModel(ConfigContext context)
   {
      super(new DefaultMutableTreeNode(new Category("/")));
      setContext(context);

      // Listen to the config broker
      getBroker().addConfigListener(mBrokerListener);
   }

   /**
    * Sets the context this model should use.
    */
   public void setContext(ConfigContext context)
   {
      // Change the context.
      if (mContext != null)
      {
         mContext.removeConfigContextListener(mConfigContextListener);
      }
      mContext = context;
      mContext.addConfigContextListener(mConfigContextListener);

      rebuildTree();
   }

   /**
    * Gets the context this model is using.
    */
   public ConfigContext getContext()
   {
      return mContext;
   }

   /**
    * Clears out the current tree and rebuilds it based on the current context.
    */
   private void rebuildTree()
   {
      // First clear out the current tree
      clearTree();

      // Add all elements in the context
      addElements(getBroker().getElements(mContext));
   }

   /**
    * Adds all the elements in the given list to this tree.
    */
   private void addElements(List elements)
   {
      for (Iterator itr = elements.iterator(); itr.hasNext(); )
      {
         addElement((ConfigElement)itr.next());
      }
   }

   /**
    * Removes all elements in the given list from this tree.
    */
   private void removeElements(List elements)
   {
      for (Iterator itr = elements.iterator(); itr.hasNext(); )
      {
         removeElement((ConfigElement)itr.next());
      }
   }

   /**
    * Adds the given element to the tree for each category that it belongs to.
    */
   private void addElement(ConfigElement elt)
   {
      // Listen for changes to the element and all of its embedded elements
      addListenerTo(elt);

      // If the element does not specify a category, default to the root
      List categories = new ArrayList(elt.getDefinition().getCategories());
      if (categories.size() == 0)
      {
         categories.add(new Category("/"));
      }

      // Add the element as a child to each of its categories
      for (Iterator itr = categories.iterator(); itr.hasNext(); )
      {
         // Get the node for this category
         Category cat = (Category)itr.next();
         DefaultMutableTreeNode cat_node = getCategoryNode(cat);
         if (cat_node == null)
         {
            cat_node = addCategoryNode(cat);
         }
         // Add the element to the category (sorted)
         addElement(elt, cat_node, true);
      }
   }

   /**
    * Removes the given configuration element from all of its locations in the
    * tree.
    */
   private void removeElement(ConfigElement elt)
   {
      // Stop listening for changes to the element and all of its embedded elements
      removeListenerFrom(elt);

      // Remove the element from all of its locations in the tree
      for (Iterator itr = getNodesFor(elt).iterator(); itr.hasNext(); )
      {
         // Remove the element node from its parent.
         DefaultMutableTreeNode elt_node = (DefaultMutableTreeNode)itr.next();
         MutableTreeNode parent = (MutableTreeNode)elt_node.getParent();
         removeNodeFromParent(elt_node);

         // Walk our way up the tree cleaning up empty parents
         while ((parent.getParent() != null) && (parent.getChildCount() == 0))
         {
            MutableTreeNode next_parent = (MutableTreeNode)parent.getParent();
            removeNodeFromParent(parent);
            parent = next_parent;
         }
      }
   }

   /**
    * Adds the given element to the tree as a child of the given parent node.
    */
   private void addElement(ConfigElement elt, DefaultMutableTreeNode parent,
                           boolean sorted)
   {
      // Add the node to the parent
      DefaultMutableTreeNode elt_node;
      if (sorted)
      {
         elt_node = addSorted(elt, parent);
      }
      else
      {
         elt_node = new DefaultMutableTreeNode(elt);
         insertNodeInto(elt_node, parent, parent.getChildCount());
      }

      // Add the properties with embedded elements to the element node
      List prop_defs = elt.getDefinition().getPropertyDefinitions();
      for (Iterator prop_itr = prop_defs.iterator(); prop_itr.hasNext(); )
      {
         // Check if this property is an embedded type
         PropertyDefinition prop_def = (PropertyDefinition)prop_itr.next();
         if (prop_def.getType() == ConfigElement.class)
         {
            // Add a node for the property (to contain the embedded types)
            DefaultMutableTreeNode prop_node =
               new DefaultMutableTreeNode(prop_def);
            insertNodeInto(prop_node, elt_node, elt_node.getChildCount());

            // Add all embedded elements to the property node
            List emb_elts = elt.getPropertyValues(prop_def.getToken());
            for (Iterator elt_itr = emb_elts.iterator(); elt_itr.hasNext(); )
            {
               ConfigElement emb_elt = (ConfigElement)elt_itr.next();
               // XXX: Don't need this since we have called addListenerTo up the stack.
               //emb_elt.addConfigElementListener(mElementListener);
               addElement(emb_elt, prop_node, false);
            }
         }
      }
   }

   /**
    * Adds the given element to the given parent node.
    */
   private DefaultMutableTreeNode addSorted(ConfigElement elt,
                                            DefaultMutableTreeNode parent)
   {
      // Make sure the element gets added in alphabetical order
      int insert_idx = 0;
      while (insert_idx < parent.getChildCount())
      {
         // Get the child at the current index
         DefaultMutableTreeNode child_node =
            (DefaultMutableTreeNode)parent.getChildAt(insert_idx);
         Object child = child_node.getUserObject();

         // First assume that we need to insert after the current child. If we
         // decide we need to insert ourselves before such child, we'll back up
         // the insertion index.
         ++insert_idx;

         // If the child is a config element, make sure we go alphabetically
         // after it.
         if (child instanceof ConfigElement)
         {
            ConfigElement child_elt = (ConfigElement)child;
            if (elt.getName().compareTo(child_elt.getName()) <= 0)
            {
               // Insert the element before the current node
               --insert_idx;
               break;
            }
         }
      }

      DefaultMutableTreeNode elt_node = new DefaultMutableTreeNode(elt);
      insertNodeInto(elt_node, parent, insert_idx);
      return elt_node;
   }

   /**
    * Gets the node for the given category.
    *
    * @return  the node for the category if it exists, null otherwise
    */
   private DefaultMutableTreeNode getCategoryNode(Category category)
   {
      Category parent = category.getParent();

      // If the parent is null, this is the root category
      if (parent == null)
      {
         return (DefaultMutableTreeNode)getRoot();
      }

      // Otherwise, get the child node of the parent category node for category
      DefaultMutableTreeNode parent_node = getCategoryNode(parent);
      if (parent_node == null)
      {
         // No node for parent category, propogate failure
         return null;
      }
      for (Enumeration e = parent_node.children(); e.hasMoreElements(); )
      {
         DefaultMutableTreeNode child_node = (DefaultMutableTreeNode)e.nextElement();
         if (child_node.getUserObject().equals(category))
         {
            return child_node;
         }
      }

      // Couldn't find a node for this category, fail
      return null;
   }

   /**
    * Adds the node for the given category to the tree.
    */
   private DefaultMutableTreeNode addCategoryNode(Category category)
   {
      // Get the parent category node
      Category parent = category.getParent();
      DefaultMutableTreeNode parent_node = getCategoryNode(parent);
      if (parent_node == null)
      {
         parent_node = addCategoryNode(parent);
      }

      // Add this category as a child to the parent category node alphabetically
      int insert_idx = 0;
      while (insert_idx < parent_node.getChildCount())
      {
         // Get the child at the current index
         DefaultMutableTreeNode child_node = (DefaultMutableTreeNode)parent_node.getChildAt(insert_idx);
         Object child = child_node.getUserObject();

         // First assume that we need to insert after the current child. If we
         // decide we need to insert ourselves before such child, we'll back up
         // the insertion index.
         ++insert_idx;

         // If the child is a configuration element, we go before it
         if (child instanceof ConfigElement)
         {
            --insert_idx;
            break;
         }

         // If the child is a category, make sure we go alphabetically after it
         if (child instanceof Category)
         {
            Category child_cat = (Category)child;
            if (category.getName().compareTo(child_cat.getName()) <= 0)
            {
               // Insert the category before the current node
               --insert_idx;
               break;
            }
         }
      }

      DefaultMutableTreeNode cat_node = new DefaultMutableTreeNode(category);
      insertNodeInto(cat_node, parent_node, insert_idx);
      
      // Inform all listeners of the change. This seems to only be an issue
      // when adding the first category.
      reload();

      return cat_node;
   }

   /**
    * Clears out the current tree making sure to stop listening for changes to
    * the elements that were in the tree.
    */
   private void clearTree()
   {
      // TODO: Stop listening

      // Clear the tree
      ((DefaultMutableTreeNode)getRoot()).removeAllChildren();
   }

   /**
    * Called when the user has altered the node at the path to be the given
    * new value.
    */
   public void valueForPathChanged(TreePath path, Object newValue)
   {
      // TODO: implement this when the tree has been made editable
   }

   /**
    * Adds a listener to the given config element and all of its embedded
    * elements recursively.
    */
   private void addListenerTo(ConfigElement elt)
   {
      // Listen to the element
      elt.addConfigElementListener(mElementListener);

      // Listen to the element's embedded elements
      List prop_defs = elt.getDefinition().getPropertyDefinitions();
      for (Iterator itr = prop_defs.iterator(); itr.hasNext(); )
      {
         // Check if this property definition is an embedded type
         PropertyDefinition prop_def = (PropertyDefinition)itr.next();
         if (prop_def.getType() == ConfigElement.class)
         {
            // Listen to all values for this property
            List emb_elts = elt.getPropertyValues(prop_def.getToken());
            for (Iterator elt_itr = emb_elts.iterator(); elt_itr.hasNext(); )
            {
               addListenerTo((ConfigElement)elt_itr.next());
            }
         }
      }
   }

   private void removeListenerFrom(ConfigElement elt)
   {
      // Don't listen to the element or its embedded elements anymore
      List prop_defs = elt.getDefinition().getPropertyDefinitions();
      for (Iterator itr = prop_defs.iterator(); itr.hasNext(); )
      {
         // Check if this property definition is an embedded type
         PropertyDefinition prop_def = (PropertyDefinition)itr.next();
         if (prop_def.getType() == ConfigElement.class)
         {
            // Listen to all values for this property
            List emb_elts = elt.getPropertyValues(prop_def.getToken());
            for (Iterator elt_itr = emb_elts.iterator(); elt_itr.hasNext(); )
            {
               removeListenerFrom((ConfigElement)elt_itr.next());
            }
         }
      }

      // Stop listening to the element
      elt.removeConfigElementListener(mElementListener);
   }

   /**
    * Gets a list of all the tree nodes that contain the given Object.
    */
   protected List getNodesFor(Object obj)
   {
      return getNodesFor(obj, (DefaultMutableTreeNode)getRoot());
   }

   /**
    * Recursive helper for getNodesFor(Object).
    *
    * @see #getNodesFor(Object)
    */
   private List getNodesFor(Object obj, DefaultMutableTreeNode node)
   {
      List results = new ArrayList();

      // Check if we found a match
      if (node.getUserObject().equals(obj))
      {
         results.add(node);
      }

      // Check all children of the current node
      for (Enumeration e = node.children(); e.hasMoreElements(); )
      {
         DefaultMutableTreeNode child = (DefaultMutableTreeNode)e.nextElement();
         results.addAll(getNodesFor(obj, child));
      }

      return results;
   }

   /**
    * Gets a handle to the configuration broker.
    */
   private ConfigBroker getBroker()
   {
      if (mBroker == null)
      {
         synchronized (this)
         {
            if (mBroker == null)
            {
               mBroker = new ConfigBrokerProxy();
            }
         }
      }
      return mBroker;
   }

   /** The context being modeled. */
   private ConfigContext mContext;

   /** The handle to the configuration broker. */
   private ConfigBroker mBroker;

   /** All listeners interested in this model. */
   private EventListenerList mListeners = new EventListenerList();

   /** The custom listener for changes to the config broker. */
   private BrokerChangeListener mBrokerListener = new BrokerChangeListener();

   /** The custom listener for changes to the config context. */
   private ContextChangeListener mConfigContextListener = new ContextChangeListener();

   /** The custom listener for changes to the elements in the context. */
   private ElementChangeListener mElementListener = new ElementChangeListener();

   /**
    * Custom comparator for sorting configuration elements by label.
    */
   private class ConfigElementLabelComparator
      implements Comparator
   {
      public int compare(Object o1, Object o2)
      {
         ConfigElement e1 = (ConfigElement)o1;
         ConfigElement e2 = (ConfigElement)o2;
         return e1.getName().compareTo(e2.getName());
      }
   }

   /**
    * Custom comparator for sorting categories.
    */
   private class CategoryComparator
      implements Comparator
   {
      public int compare(Object o1, Object o2)
      {
         Category c1 = (Category)o1;
         Category c2 = (Category)o2;
         return c1.getName().compareTo(c2.getName());
      }
   }

   /**
    * Custom listener for changes to the config broker.
    */
   private class BrokerChangeListener
      implements ConfigListener
   {
      public void configElementAdded(ConfigEvent evt)
      {
         System.out.println("ConfigElement added.");

         if (getContext().contains(evt.getResource()))
         {
            addElement(evt.getElement());
         }
      }

      public void configElementRemoved(ConfigEvent evt)
      {
         if (getContext().contains(evt.getResource()))
         {
            removeElement(evt.getElement());
         }
      }
   }

   /**
    * Custom listener for changes to the configuration context.
    */
   private class ContextChangeListener
      implements ConfigContextListener
   {
      public void resourceAdded(ConfigContextEvent evt)
      {
         String resource = evt.getResource();
         addElements(getBroker().getElementsIn(resource));
      }

      public void resourceRemoved(ConfigContextEvent evt)
      {
         String resource = evt.getResource();
         removeElements(getBroker().getElementsIn(resource));
      }
   }

   /**
    * Custom listener for changes to configuration elements.
    */
   private class ElementChangeListener
      implements ConfigElementListener
   {
      public void nameChanged(ConfigElementEvent evt)
      {
         // Let listeners of the model know that the representation of the nodes
         // in the tree for the element that changed have changed.
         ConfigElement elt = (ConfigElement)evt.getSource();
         for (Iterator itr = getNodesFor(elt).iterator(); itr.hasNext(); )
         {
            nodeChanged((TreeNode)itr.next());
         }
      }

      public void propertyValueChanged(ConfigElementEvent evt)
      {
         ConfigElement elt = (ConfigElement)evt.getSource();
         PropertyDefinition prop_def =
            elt.getDefinition().getPropertyDefinition(evt.getProperty());

         // If the property is not an embedded element type, we don't care about it
         if (prop_def.getType() != ConfigElement.class)
         {
            return;
         }

         // Update the property for each element node in the tree
         for (Iterator itr = getNodesFor(elt).iterator(); itr.hasNext(); )
         {
            DefaultMutableTreeNode elt_node = (DefaultMutableTreeNode)itr.next();

            // Go through the children of this node looking for the property
            // that matches
            for (Enumeration e = elt_node.children(); e.hasMoreElements(); )
            {
               DefaultMutableTreeNode prop_node = (DefaultMutableTreeNode)e.nextElement();
               
               String property_token = ((PropertyDefinition)prop_node.getUserObject()).getToken();
               String changed_property = evt.getProperty();
               
               if (property_token.equals(changed_property))
               {
                  // Get the appropriate value node and mark it as changed
                  TreeNode value_node = prop_node.getChildAt(evt.getIndex());
                  nodeChanged(value_node);
               }
            }
         }
      }

      public void propertyValueOrderChanged(ConfigElementEvent evt)
      {
         ConfigElement elt = (ConfigElement) evt.getSource();
         PropertyDefinition prop_def =
            elt.getDefinition().getPropertyDefinition(evt.getProperty());

         // If the property is not an embedded element type, we don't care
         // about it.
         if ( prop_def.getType() != ConfigElement.class )
         {
            return;
         }

         // Add the new property to each element node in the tree.
         for (Iterator itr = getNodesFor(elt).iterator(); itr.hasNext(); )
         {
            DefaultMutableTreeNode elt_node =
               (DefaultMutableTreeNode)itr.next();

            // Go through the children of this node looking for the property
            // that maches.
            for ( Enumeration e = elt_node.children(); e.hasMoreElements(); )
            {
               DefaultMutableTreeNode prop_node =
                  (DefaultMutableTreeNode) e.nextElement();
               
               String property_token =
                  ((PropertyDefinition) prop_node.getUserObject()).getToken();
               String changed_property = evt.getProperty();
               
               if ( property_token.equals(changed_property) )
               {
                  int start_index = Math.min(evt.getIndex0(), evt.getIndex1());
                  int end_index   = Math.max(evt.getIndex0(), evt.getIndex1());

                  List removed_children = new ArrayList();
                  for ( int c = start_index; c <= end_index; ++c )
                  {
                     removed_children.add(getChild(prop_node, c));
                  }

                  for ( Iterator c = removed_children.iterator();
                        c.hasNext(); )
                  {
                     removeNodeFromParent((MutableTreeNode) c.next());
                  }

                  for ( int v = start_index; v <= end_index; ++v )
                  {
                     // Create a new node for the reordered property value.
                     DefaultMutableTreeNode new_node =
                        new DefaultMutableTreeNode(elt.getProperty(property_token, v));

                     // Add the new node into the tree.
                     insertNodeInto(new_node, prop_node, v);
                  }
               }
            }
         }
      }

      public void propertyValueAdded(ConfigElementEvent evt)
      {
         ConfigElement elt = (ConfigElement)evt.getSource();
         PropertyDefinition prop_def =
            elt.getDefinition().getPropertyDefinition(evt.getProperty());

         // If the property is not an embedded element type, we don't care about it
         if (prop_def.getType() != ConfigElement.class)
         {
            return;
         }

         // Add the new property to each element node in the tree
         for (Iterator itr = getNodesFor(elt).iterator(); itr.hasNext(); )
         {
            DefaultMutableTreeNode elt_node = (DefaultMutableTreeNode)itr.next();

            // Go through the children of this node looking for the property
            // that maches
            for (Enumeration e = elt_node.children(); e.hasMoreElements(); )
            {
               DefaultMutableTreeNode prop_node = (DefaultMutableTreeNode)e.nextElement();
               
               String property_token = ((PropertyDefinition)prop_node.getUserObject()).getToken();
               String changed_property = evt.getProperty();
               
               if (property_token.equals(changed_property))
               {
                  // Create a new node for the new property value
                  ConfigElement new_elt = (ConfigElement)evt.getValue();
                  DefaultMutableTreeNode new_node =
                     new DefaultMutableTreeNode(new_elt);

                  // Add a listener to the newly added embedded element
                  new_elt.addConfigElementListener(mElementListener);

                  // Add the new node into the tree
                  insertNodeInto(new_node, prop_node, evt.getIndex());
               }
            }
         }
      }

      public void propertyValueRemoved(ConfigElementEvent evt)
      {
         ConfigElement elt = (ConfigElement)evt.getSource();
         PropertyDefinition prop_def =
            elt.getDefinition().getPropertyDefinition(evt.getProperty());

         // If the property is not an embedded element type, we don't care about it
         if (prop_def.getType() != ConfigElement.class)
         {
            return;
         }

         // Remove the property from each element node in the tree
         for (Iterator itr = getNodesFor(elt).iterator(); itr.hasNext(); )
         {
            DefaultMutableTreeNode elt_node = (DefaultMutableTreeNode)itr.next();

            // Go through the children of this node looking for the property
            // that matches
            for (Enumeration e = elt_node.children(); e.hasMoreElements(); )
            {
               DefaultMutableTreeNode prop_node = (DefaultMutableTreeNode)e.nextElement();
               String property_token = ((PropertyDefinition)prop_node.getUserObject()).getToken();
               String changed_property = evt.getProperty();
               
               if (property_token.equals(changed_property))
               {
                  // Stop listening to the embedded element
                  ConfigElement removed_elt = (ConfigElement)evt.getValue();
                  removed_elt.removeConfigElementListener(mElementListener);

                  // Itereate over child nodes and remove the nodes for the given element.
                  for (Enumeration ve = prop_node.children(); ve.hasMoreElements(); )
                  {
                     DefaultMutableTreeNode node = (DefaultMutableTreeNode)ve.nextElement();
                     if( node.getUserObject() == evt.getValue() )
                     {
                        // Remove the node from the tree
                        removeNodeFromParent(node);
                     }
                  }
               }
            }
         }
      }
   }
}
