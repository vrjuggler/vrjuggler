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

package org.vrjuggler.jccl.editorgui;

import java.awt.Component;
import javax.swing.*;


/**
 * This is a handy little class for rendering VarValues of type
 * ValType.BOOL.  It translates the "0" for false to the string "false"
 * and simiilarly for "true".
 */
public class TrueFalseCellRenderer extends JLabel
                                   implements ListCellRenderer
{
   public TrueFalseCellRenderer ()
   {
      setOpaque(true);
   }

   public Component getListCellRendererComponent (JList list, Object value,
                                                  int index,
                                                  boolean isSelected,
                                                  boolean cellHasFocus)
   {
      if ( isSelected )
      {
         setBackground(list.getSelectionBackground());
         setForeground(list.getSelectionForeground());
      }
      else
      {
         setBackground(list.getBackground());
         setForeground(list.getForeground());
      }

      if ( value.toString().equals("1") )
      {
         setText("true");
      }
      else
      {
         setText("false");
      }

      return this;
   }
}
