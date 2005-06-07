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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

package org.vrjuggler.tweek.iconviewer;

import javax.swing.*;
import java.awt.*;


/**
 * A subclass of JInternalFrame designed to simplify access to the contained
 * BeanIconPanel object.
 */
public class InternalBeanFrame extends JInternalFrame
{
   public InternalBeanFrame ()
   {
      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }
   }

   public void setIconPanel (BeanIconPanel newIconPanel)
   {
      if ( null != this.iconPanel )
      {
         this.getContentPane().remove(iconPanel.getComponent());
      }

      this.iconPanel = newIconPanel;
      mScrollPane.getViewport().add(new JScrollPane(iconPanel.getComponent()),
                                    null);
      this.setTitle(iconPanel.getName());

      this.pack();
   }

   public BeanIconPanel getIconPanel ()
   {
      return iconPanel;
   }

   private void jbInit () throws Exception
   {
      this.setRequestFocusEnabled(true);
      this.setClosable(true);
      this.setIconifiable(true);
      this.setResizable(true);
      this.setTitle("Icon Panel");
      this.setBackground(java.awt.Color.white);
      this.getContentPane().add(mScrollPane, BorderLayout.CENTER);
   }

   private BeanIconPanel iconPanel = null;

   private JScrollPane mScrollPane = new JScrollPane();
}
