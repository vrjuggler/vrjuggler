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

import java.util.Vector;
import javax.swing.tree.*;


/**
 * @since 1.0
 */
public class BeanTreeModel extends DefaultTreeModel
{
   public BeanTreeModel (TreeNode root)
   {
      super(root);
   }

   public BeanTreeModel (TreeNode root, boolean askAllowsChildren)
   {
      super(root, askAllowsChildren);
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
