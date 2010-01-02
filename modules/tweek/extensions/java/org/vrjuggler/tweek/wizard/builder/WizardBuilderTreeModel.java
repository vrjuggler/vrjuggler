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
package org.vrjuggler.tweek.wizard.builder;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.NoSuchElementException;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.*;

import org.vrjuggler.tweek.wizard.*;

/**
 * Tweak panel bean that allows for the construction of Wizards.
 */
public class WizardBuilderTreeModel
   implements TreeModel
{
   /**
    * Creates a new WizardBuilder TreeModel for the given wizard.
    *
    * @throw   IllegalArgumentException
    *             if wizard is null
    */
   public WizardBuilderTreeModel(Wizard wizard)
   {
      // Don't allow a null wizard
      if (wizard == null)
      {
         throw new IllegalArgumentException("wizard can't be null");
      }
      mListeners = new ArrayList();
      this.mWizard = wizard;
   }

   /**
    * Gets the root of the tree.
    */
   public Object getRoot()
   {
      return mWizard.getSequence();
   }

   /**
    * Gets the child of parent at index in the parent's child array.
    */
   public Object getChild(Object parent, int index)
   {
      try
      {
         return ((WizardSequence)parent).get(index);
      }
      catch (NoSuchElementException e)
      {
         return null;
      }
   }

   /**
    * Gets the number of children of parent.
    */
   public int getChildCount(Object parent)
   {
      return ((WizardSequence)parent).size();
   }

   /**
    * Gets the index of the child object within the parent.
    */
   public int getIndexOfChild(Object parent, Object child)
   {
      return ((WizardSequence)parent).getIndexOf((WizardStep)child);
   }

   /**
    * Checks if the given node is a leaf.
    */
   public boolean isLeaf(Object node)
   {
      return ! (node instanceof WizardSequence);
   }

   /**
    * Called when the user has altered the value for the item identified by path
    * to newValue.
    */
   public void valueForPathChanged(TreePath path, Object newValue)
   {
      WizardStep step = (WizardStep)path.getLastPathComponent();
   }

   /**
    * Adds the given child into the given parent at the end of the child list.
    * This is the preferred way to append a child since it will fire off the
    * appropriate event.
    */
   public void add(WizardStep newChild, WizardSequence parent)
   {
      insertInto(newChild, parent, parent.size());
   }

   /**
    * Adds the given child into the given parent. This is the preferred way
    * to add children since it will fire off the appropriate event.
    */
   public void insertInto(WizardStep newChild, WizardSequence parent, int idx)
   {
      parent.insert(newChild, idx);

      // fire off the event
      int[] newIndicies = new int[1];
      newIndicies[0] = idx;
      nodesWereInserted(parent, newIndicies);
   }

   /**
    * Removes the given child from the given parent. This is the preferred way
    * to remove children since it will fire off the appropriate event.
    */
   public void removeFrom(WizardStep child, WizardSequence parent)
   {
      int idx = parent.getIndexOf(child);
      if (idx != -1)
      {
         parent.remove(idx);

         // fire off the event
         int[] indicesRemoved = { idx };
         Object[] childrenRemoved = { child };
         nodesWereRemoved(parent, indicesRemoved, childrenRemoved);
      }
   }

   /**
    * Gets the wizard this model represents.
    */
   public Wizard getWizard()
   {
      return mWizard;
   }

   /**
    * Sets the wizard on this model.
    *
    * @throw   IllegalArgumentException
    *             if wizard is null
    */
   public void setWizard(Wizard wizard)
   {
      // Don't allow a null wizard.
      if (wizard == null)
      {
         throw new IllegalArgumentException(
                     "The wizard is not allowed to be null");
      }
      mWizard = wizard;
      nodeStructureChanged(wizard.getSequence());
   }

   /**
    * Invoke this if the children of the given node have been totally changed.
    * This will post a treeStructureChaged event.
    */
   public void nodeStructureChanged(WizardStep step)
   {
      if (step != null)
      {
         fireTreeStructureChanged(this, getPathToRoot(step), null, null);
      }
   }

   /**
    * Invoke this if nodes have been inserted into the given parent at the
    * given indices. This will post a treeNodesInserted event.
    */
   public void nodesWereInserted(WizardSequence sequence, int[] childIndicies)
   {
      int numNewChildren = childIndicies.length;
      Object[] newChildren = new Object[numNewChildren];
      for (int i=0; i<numNewChildren; ++i)
      {
         newChildren[i] = sequence.get(childIndicies[i]);
      }
      fireTreeNodesInserted(this, getPathToRoot(sequence), childIndicies,
                            newChildren);
   }

   /**
    * Invoke this if nodes have been removed from the given parent at the
    * given indicies. This will post a treeNodesRemoved event.
    */
   public void nodesWereRemoved(WizardSequence sequence, int[] childIndicies,
                                Object[] childrenRemoved)
   {
      if (childIndicies != null)
      {
         fireTreeNodesRemoved(this, getPathToRoot(sequence), childIndicies,
                              childrenRemoved);
      }
   }

   /**
    * Computes a list of the nodes extending from the root to the given node.
    */
   public WizardStep[] getPathToRoot(WizardStep step)
   {
      return getPathToRoot(step, 0);
   }

   /**
    * Helper function to getPathToRoot(WizardStep).
    */
   public WizardStep[] getPathToRoot(WizardStep step, int depth)
   {
      WizardStep[] result;

      // Safety precaution if we got passed a null step
      if (step == null)
      {
         if (depth == 0)
         {
            return null;
         }
         else
         {
            result = new WizardStep[depth];
         }
      }
      else
      {
         ++depth;
         if (step == getRoot())
         {
            // we reached the root, stop recursing
            result = new WizardStep[depth];
         }
         else
         {
            // get a filled in path from the current node to the dest node
            result = getPathToRoot(step.getParent(), depth);
         }

         // add in the current node to the path
         result[result.length - depth] = step;
      }
      return result;
   }

   /**
    * Adds the given listener to this model.
    */
   public void addTreeModelListener(TreeModelListener l)
   {
      mListeners.add(l);
   }

   /**
    * Removes the given listener to this model.
    */
   public void removeTreeModelListener(TreeModelListener l)
   {
      mListeners.remove(l);
   }

   /**
    * Notify all listeners that nodes have been inserted.
    */
   protected void fireTreeNodesChanged(Object source, Object[] path,
                                       int[] childIndices, Object[] children)
   {
      TreeModelEvent e = new TreeModelEvent(source, path,
                                            childIndices, children);
      for (Iterator itr = mListeners.iterator(); itr.hasNext(); )
      {
         ((TreeModelListener)itr.next()).treeNodesChanged(e);
      }
   }

   /**
    * Notify all listeners that nodes have been inserted.
    */
   protected void fireTreeNodesInserted(Object source, Object[] path,
                                       int[] childIndices, Object[] children)
   {
      TreeModelEvent e = new TreeModelEvent(source, path,
                                            childIndices, children);
      for (Iterator itr = mListeners.iterator(); itr.hasNext(); )
      {
         ((TreeModelListener)itr.next()).treeNodesInserted(e);
      }
   }

   /**
    * Notify all listeners that nodes have been deleted.
    */
   protected void fireTreeNodesRemoved(Object source, Object[] path,
                                       int[] childIndices, Object[] children)
   {
      TreeModelEvent e = new TreeModelEvent(source, path,
                                            childIndices, children);
      for (Iterator itr = mListeners.iterator(); itr.hasNext(); )
      {
         ((TreeModelListener)itr.next()).treeNodesRemoved(e);
      }
   }

   /**
    * Notify all listeners that the structure of the tree on the given path
    * down has changed.
    */
   protected void fireTreeStructureChanged(Object source, Object[] path,
                                       int[] childIndices, Object[] children)
   {
      TreeModelEvent e = new TreeModelEvent(source, path,
                                            childIndices, children);
      for (Iterator itr = mListeners.iterator(); itr.hasNext(); )
      {
         ((TreeModelListener)itr.next()).treeStructureChanged(e);
      }
   }

   /**
    * The wizard this tree model is representing.
    */
   Wizard mWizard;

   /**
    * List of all TreeModelListeners.
    */
   List mListeners;
}
