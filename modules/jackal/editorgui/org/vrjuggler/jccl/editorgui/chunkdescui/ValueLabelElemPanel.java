/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
package org.vrjuggler.jccl.editorgui.chunkdescui;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editorgui.*;
import org.vrjuggler.jccl.vjcontrol.Core;
import org.vrjuggler.jccl.vjcontrol.ui.widgets.*;

/** A small bordered panel for editing a single PropertyDesc ValueLabel.
 *
 *  @version $Revision$
 */
public class ValueLabelElemPanel extends JPanel implements MouseListener
{

   static AbstractBorder select_border =
      new CompoundBorder(new BevelBorder(BevelBorder.LOWERED),
                         new EmptyBorder(5,5,5,5));

   static AbstractBorder unselect_border =
      new CompoundBorder(new BevelBorder(BevelBorder.RAISED),
                         new EmptyBorder (5,5,5,5));

   private PropertyDesc.Item mItem = null;

   JTextField namef;
   JTextField mDefaultField;
   boolean selected = false;

   public ValueLabelElemPanel (PropertyDesc.Item item)
   {
      super();
      mItem = item;
      jbInit();
   }

   private void jbInit ()
   {
      setBorder(unselect_border);
      addMouseListener (this);

      JLabel name_label = new JLabel("Label Name:");
      name_label.addMouseListener(this);
      add(name_label);
      namef = new JTextField(mItem.getLabel(), 20);
      add (namef);
      JLabel default_label = new JLabel ("Default Value:");
      default_label.addMouseListener(this);
      add(default_label);
      mDefaultField = new JTextField(mItem.getDefaultValue().toString(), 20);
      add(mDefaultField);

      Dimension d3 = getPreferredSize();
      Dimension d4 = getMaximumSize();
      d4.height = d3.height;
      setMaximumSize (d4);
   }

   public void commit () throws ConfigUIException
   {
      mItem.setLabel(namef.getText());
      VarValue temp_val = new VarValue(mItem.getValType());
      temp_val.set(mDefaultField.getText());
      mItem.setDefaultValue(temp_val);
   }


   private void toggleSelected()
   {
      selected = !selected;
      super.setBorder(selected?select_border:unselect_border);
      repaint();
   }

   public boolean getSelected()
   {
      return selected;
   }


   /* MouseListener stuff */
   public void mouseClicked (MouseEvent e)
   {
      toggleSelected();
   }
   public void mouseEntered (MouseEvent e)
   {
   }
   public void mouseExited (MouseEvent e)
   {
   }
   public void mousePressed (MouseEvent e)
   {
   }
   public void mouseReleased (MouseEvent e)
   {
   }
}
