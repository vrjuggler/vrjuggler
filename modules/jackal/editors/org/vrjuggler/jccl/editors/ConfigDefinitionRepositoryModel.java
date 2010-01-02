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
import org.vrjuggler.jccl.config.event.*;

/**
 * A data model for a configuration definition repository.
 */
public class ConfigDefinitionRepositoryModel
   extends DefaultTreeModel
{
   /**
    * Creates a new, empty model.
    */
   public ConfigDefinitionRepositoryModel()
   {
      this(new ConfigDefinitionRepository());
   }

   /**
    * Creates a model for the given repository.
    *
    * @param repos         the repository to model
    */
   public ConfigDefinitionRepositoryModel(ConfigDefinitionRepository repos)
   {
      super(new DefaultMutableTreeNode(new Category("/")));
      setRepository(repos);
   }

   /**
    * Sets the repository this model should use.
    */
   public void setRepository(ConfigDefinitionRepository repos)
   {
      // Change the repository
      if (mRepos != null)
      {
         mRepos.removeConfigDefinitionRepositoryListener(mReposListener);
      }
      mRepos = repos;
      mRepos.addConfigDefinitionRepositoryListener(mReposListener);

      rebuildTree();
   }

   /**
    * Gets the repository this model is using.
    */
   public ConfigDefinitionRepository getRepository()
   {
      return mRepos;
   }

   /**
    * Clears out the current tree and rebuilds it based on the current
    * repository.
    */
   private void rebuildTree()
   {
      // First clear out the current tree
      clearTree();

      // Add all definitions in the repository
      addDefinitions(mRepos.getAllLatest());
   }

   /**
    * Adds all the definitions in the given list to this tree.
    */
   private void addDefinitions(List definitions)
   {
      for (Iterator itr = definitions.iterator(); itr.hasNext(); )
      {
         addDefinition((ConfigDefinition)itr.next());
      }
   }

   /**
    * Removes all definitions in the given list from this tree.
    */
   private void removeDefinitions(List definitions)
   {
      for (Iterator itr = definitions.iterator(); itr.hasNext(); )
      {
         removeDefinition((ConfigDefinition)itr.next());
      }
   }

   /**
    * Adds the given definition to the tree for each category that it belongs to.
    */
   private void addDefinition(ConfigDefinition def)
   {
      // Listen for changes to the definition and all of its property definitions
      addListenerTo(def);

      // If the definition does not specify a category, default to the root
      List categories = new ArrayList(def.getCategories());
      if (categories.size() == 0)
      {
         categories.add(new Category("/"));
      }

      // Add the definition as a child to each of its categories
      for (Iterator itr = categories.iterator(); itr.hasNext(); )
      {
         // Get the node for this category
         Category cat = (Category)itr.next();
         DefaultMutableTreeNode cat_node = getCategoryNode(cat);
         if (cat_node == null)
         {
            cat_node = addCategoryNode(cat);
         }
         // Add the definition to the category (sorted)
         addDefinition(def, cat_node);
      }
   }

   /**
    * Removes the given configuration definition from all of its locations in
    * the tree.
    */
   private void removeDefinition(ConfigDefinition def)
   {
      // Stop listening for changes to the definition and all of its property
      // definitions
      removeListenerFrom(def);

      // Remove the definition from all of its locations in the tree
      for (Iterator itr = getNodesFor(def).iterator(); itr.hasNext(); )
      {
         // Remove the definition node from its parent.
         DefaultMutableTreeNode def_node = (DefaultMutableTreeNode)itr.next();
         MutableTreeNode parent = (MutableTreeNode)def_node.getParent();
         removeNodeFromParent(def_node);

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
    * Adds the given definition to the tree as a child of the given parent node.
    */
   private void addDefinition(ConfigDefinition def,
                              DefaultMutableTreeNode parent)
   {
      // Add the node to the parent
      DefaultMutableTreeNode def_node;
      def_node = addSorted(def, parent);

      // Add the properties definitions to the definition
      List prop_defs = def.getPropertyDefinitions();
      for (Iterator prop_itr = prop_defs.iterator(); prop_itr.hasNext(); )
      {
         // Add a node for the property definition
         PropertyDefinition prop_def = (PropertyDefinition)prop_itr.next();
         DefaultMutableTreeNode prop_node =
            new DefaultMutableTreeNode(prop_def);
         insertNodeInto(prop_node, def_node, def_node.getChildCount());
      }
   }

   /**
    * Adds the given definition to the given parent node.
    */
   private DefaultMutableTreeNode addSorted(ConfigDefinition def,
                                            DefaultMutableTreeNode parent)
   {
      // Make sure the definition gets added in alphabetical order
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

         // If the child is a config definition, make sure we go alphabetically
         // after it.
         if (child instanceof ConfigDefinition)
         {
            ConfigDefinition child_def = (ConfigDefinition)child;

            // If we find an existing definition with a token matching ours,
            // then we have to replace the old definition with the new one.
            if ( def.getToken().equals(child_def.getToken()) )
            {
               child_node.setUserObject(def);

               // XXX: This has the unfortunate side effect of collapsing the
               // definition node if it is expanded.  I couldn't come up with
               // an alternative approach that avoided this.  -PH 11/12/2004
               child_node.removeAllChildren();
               nodeStructureChanged(child_node);
               return child_node;
            }
            else if (def.getName().compareTo(child_def.getName()) <= 0)
            {
               // Insert the element before the current node
               --insert_idx;
               break;
            }
         }
      }

      DefaultMutableTreeNode def_node = new DefaultMutableTreeNode(def);
      insertNodeInto(def_node, parent, insert_idx);
      return def_node;
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

         // If the child is a configuration definition, we go before it
         if (child instanceof ConfigDefinition)
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
    * Adds a listener to the given config definition and all of its property
    * definitions.
    */
   private void addListenerTo(ConfigDefinition def)
   {
      // Listen to the definition
      def.addConfigDefinitionListener(mConfigDefListener);

      // Listen to the definition's property definitions
      List prop_defs = def.getPropertyDefinitions();
      for (Iterator itr = prop_defs.iterator(); itr.hasNext(); )
      {
         PropertyDefinition prop_def = (PropertyDefinition)itr.next();
         prop_def.addPropertyDefinitionListener(mPropDefListener);
      }
   }

   /**
    * Removes a listener from the given config definition and all of its
    * property definitions.
    */
   private void removeListenerFrom(ConfigDefinition def)
   {
      // Don't listen to the definition or its property definitions anymore
      List prop_defs = def.getPropertyDefinitions();
      for (Iterator itr = prop_defs.iterator(); itr.hasNext(); )
      {
         PropertyDefinition prop_def = (PropertyDefinition)itr.next();
         prop_def.removePropertyDefinitionListener(mPropDefListener);
      }

      // Stop listening to the definition
      def.removeConfigDefinitionListener(mConfigDefListener);
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

   /** The repository of configuration definitions being modeled. */
   private ConfigDefinitionRepository mRepos;

   /** All listeners interested in this model. */
   private EventListenerList mListeners = new EventListenerList();

   /** The custom listener for changes to the repository. */
   private RepositoryChangeListener mReposListener = new RepositoryChangeListener();

   /** The custom listener for changes to the definitions in the repository. */
   private ConfigDefinitionChangeListener mConfigDefListener =
      new ConfigDefinitionChangeListener();

   /**
    * The custom listener for changes to the property definitions of the
    * config definitions in the repository.
    */
   private PropertyDefinitionChangeListener mPropDefListener =
      new PropertyDefinitionChangeListener();

   /**
    * Custom comparator for sorting configuration definitions by label.
    */
   private class ConfigDefinitionLabelComparator
      implements Comparator
   {
      public int compare(Object o1, Object o2)
      {
         ConfigDefinition d1 = (ConfigDefinition)o1;
         ConfigDefinition d2 = (ConfigDefinition)o2;
         return d1.getName().compareTo(d2.getName());
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
    * Custom listener for changes to the repository.
    */
   private class RepositoryChangeListener
      implements ConfigDefinitionRepositoryListener
   {
      public void configDefinitionAdded(ConfigDefinitionRepositoryEvent evt)
      {
         // TODO: add the definition
         addDefinition(evt.getDefinition());
      }

      public void configDefinitionRemoved(ConfigDefinitionRepositoryEvent evt)
      {
         // TODO: remove the definition
         removeDefinition(evt.getDefinition());
      }
   }

   /**
    * Custom listener for changes to configuration definitions.
    */
   private class ConfigDefinitionChangeListener
      extends ConfigDefinitionAdapter
   {
      public void nameChanged(ConfigDefinitionEvent evt)
      {
         // Let listeners of the model know that the representation of the nodes
         // in the tree for the definition that changed have changed.
         ConfigDefinition def = (ConfigDefinition)evt.getSource();
         for (Iterator itr = getNodesFor(def).iterator(); itr.hasNext(); )
         {
            nodeChanged((TreeNode)itr.next());
         }
      }

      public void categoryAdded(ConfigDefinitionEvent evt)
      {
         // Get the node for the new category
         Category category = (Category)evt.getValue();
         DefaultMutableTreeNode cat_node = getCategoryNode(category);
         if (cat_node == null)
         {
            cat_node = addCategoryNode(category);
         }

         // Add the definition to the category node
         ConfigDefinition def = (ConfigDefinition)evt.getSource();
         addDefinition(def, cat_node);
      }

      public void categoryRemoved(ConfigDefinitionEvent evt)
      {
         // Get the node for the category
         Category category = (Category)evt.getValue();
         DefaultMutableTreeNode cat_node = getCategoryNode(category);
         if (cat_node != null)
         {
            // Find the definition node that's a child of the category node
            for (Enumeration e = cat_node.children(); e.hasMoreElements(); )
            {
               DefaultMutableTreeNode child_node =
                  (DefaultMutableTreeNode)e.nextElement();
               // If this child is the definition, remove it from that category
               if (child_node.getUserObject().equals(evt.getSource()))
               {
                  removeNodeFromParent(child_node);

                  // Walk our way up the tree cleaning up empty parents
                  MutableTreeNode parent = cat_node;
                  while ((parent.getParent() != null) && (parent.getChildCount() == 0))
                  {
                     MutableTreeNode next_parent = (MutableTreeNode)parent.getParent();
                     removeNodeFromParent(parent);
                     parent = next_parent;
                  }
               }
            }
         }
      }

      public void propertyDefinitionAdded(ConfigDefinitionEvent evt)
      {
         ConfigDefinition def = (ConfigDefinition)evt.getSource();
         PropertyDefinition prop_def = evt.getPropertyDefinition();

         // Start listening to the definition
         prop_def.addPropertyDefinitionListener(mPropDefListener);

         // Get the nodes for the definition that changed
         for (Iterator itr = getNodesFor(def).iterator(); itr.hasNext(); )
         {
            DefaultMutableTreeNode def_node = (DefaultMutableTreeNode)itr.next();

            // Add a node for the property definition
            DefaultMutableTreeNode prop_node =
               new DefaultMutableTreeNode(evt.getPropertyDefinition());
            insertNodeInto(prop_node, def_node, def_node.getChildCount());
         }
      }

      public void propertyDefinitionRemoved(ConfigDefinitionEvent evt)
      {
         ConfigDefinition def = (ConfigDefinition)evt.getSource();
         PropertyDefinition prop_def = evt.getPropertyDefinition();

         // Stop listening to the definition
         prop_def.removePropertyDefinitionListener(mPropDefListener);

         // Get the nodes for property definition
         for (Iterator itr = getNodesFor(prop_def).iterator(); itr.hasNext(); )
         {
            DefaultMutableTreeNode prop_def_node = (DefaultMutableTreeNode)itr.next();
            removeNodeFromParent(prop_def_node);
         }
      }
   }

   /**
    * Custom listener for changes to property definitions of config definitions
    * in the repository.
    */
   private class PropertyDefinitionChangeListener
      extends PropertyDefinitionAdapter
   {
      public void tokenChanged(PropertyDefinitionEvent evt)
      {
         PropertyDefinition prop_def = (PropertyDefinition)evt.getSource();
         for (Iterator itr = getNodesFor(prop_def).iterator(); itr.hasNext(); )
         {
            nodeChanged((TreeNode)itr.next());
         }
      }
   }
}
