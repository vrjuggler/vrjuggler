/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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


/**
 * The basic interface used by all viewers of Bean data models.  In particular,
 * this is the interface that must be implemented by Viewer Beans.
 *
 * @see ViewerBean
 */
public interface BeanModelViewer
{
   /**
    * Returns the viewer component.
    */
   public javax.swing.JComponent getViewer();

   /**
    * Initializes the model viewer.
    *
    * @param treeModel  the Bean tree model that this viewer will display
    */
   public void init(BeanTreeModel treeModel);

   /**
    * Changes the model referenced by this viewer.
    *
    * @param treeModel  the Bean tree model that this viewer will display
    */
   public void setModel(BeanTreeModel treeModel);

   /**
    * Returns the model used by this viewer.
    */
   public BeanTreeModel getModel();

   /**
    * Initializes the user interface for this viewer.
    */
   public void initGUI();

   /**
    * Programmatically focuses the given panel bean in this viewer.
    *
    * @param bean       the bean to focus
    */
   public void focusBean(PanelBean bean);

   public void addBeanFocusChangeListener(BeanFocusChangeListener l);

   public void removeBeanFocusChangeListener(BeanFocusChangeListener l);
}
