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
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editorgui.*;
import org.vrjuggler.jccl.vjcontrol.Core;

/** Frame for editing ValueLabels.
 *  Uses the List _data which is passed to it from the parent;
 *  when the frame's OK button is pressed it modifies the List object.
 *
 *  @version $Revision$
 */
public class ValueLabelFrame extends JDialog implements ActionListener,
                                                        WindowListener
{
   protected JPanel panel;
   protected JPanel sppanel;  /* sppanel is the panel inside the scrollpane...*/
   protected JButton insertbutton;
   protected JButton removebutton;
   protected JButton okbutton;
   protected JButton cancelbutton;
   protected java.util.List elempanels; // List of DescEnumElemPanels
   protected PropertyDesc mDesc;
   private   ValType mDescValType;
   public boolean closed;
   protected JPanel buttonspanel;


   /*
    * ARGS: _varnumbers - if true, show insert & remove buttons
    * ARGS: _numval - # of values to show (only matters if varnumbers false)
    */
   public ValueLabelFrame (PropertyDescPanel p, PropertyDesc propertyDesc,
                           ValType propDescValType, String pdn)
   {
      super(JOptionPane.getFrameForComponent(p), pdn);

      // We need this dialog box to be modal WRT p so that the user cannot
      // change the property type behind our back.
      // Essentially, this is hack on top of the hack that puts default value
      // editing at this level in the GUI.  A future version of the GUI could
      // eliminate the need for modality by editing property values in such a
      // way that the user-specified value would change appropriately with the
      // property's value type.
      this.setModal(true);

      closed = false;

      mDesc        = propertyDesc;
      mDescValType = propDescValType;

      buttonspanel = new JPanel();

      elempanels = new ArrayList();
      panel = new JPanel(new BorderLayout (10,2));
      panel.setBorder (new EmptyBorder (10, 5, 5, 5));
      getContentPane().add(panel);

      sppanel = new JPanel();
      sppanel.setLayout (new BoxLayout (sppanel, BoxLayout.Y_AXIS));

      JScrollPane sp = new JScrollPane(sppanel,
                                       JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                                       JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
      sp.getVerticalScrollBar().setUnitIncrement(40);
      sp.getHorizontalScrollBar().setUnitIncrement(40);

      panel.add (sp, "Center");

      if ( mDesc.hasVariableNumberOfValues() )
      {
         buttonspanel.setLayout (new GridLayout (1, 4));
         insertbutton = new JButton ("Insert");
         insertbutton.addActionListener(this);
         buttonspanel.add (insertbutton);
         removebutton = new JButton ("Remove");
         removebutton.addActionListener(this);
         buttonspanel.add (removebutton);
      }
      else
      {
         buttonspanel.setLayout (new GridLayout (1, 2));
         insertbutton = null;
         removebutton = null;
      }
      okbutton = new JButton ("OK");
      okbutton.addActionListener(this);
      buttonspanel.add (okbutton);
      cancelbutton = new JButton ("Cancel");
      cancelbutton.addActionListener(this);
      buttonspanel.add (cancelbutton);
      panel.add (buttonspanel, "South");

      makeItems();
      addWindowListener(this);

      setFrameSize();
      setVisible(true);
   }



   private void setFrameSize()
   {
      Dimension d = sppanel.getPreferredSize();
      Dimension d2 = buttonspanel.getPreferredSize();
      d.width = Math.max (d.width+42, d2.width +20);
      d.width = Math.min (d.width, Core.screenWidth);
      if ( mDesc.hasVariableNumberOfValues() )
      {
         d.height = Math.min (300, Core.screenHeight);
      }
      else
      {
         d.height = Math.min (d.height + d2.height + 45, Core.screenHeight);
      }
      setSize(d);
   }



   public void closeFrame()
   {
      /* closes this frame & informs it's parent - doesn't save any data or
       * anything.
       */
      closed = true;
      dispose();
   }



   private int makeItems()
   {
      /* fills up the scrollpane with all the items in enums,
       * which is a list of DescEnum
       */
      java.util.List items = mDesc.getItems();
      int n = items.size();
      if( ! mDesc.hasVariableNumberOfValues() )
      {
         n = Math.min(n, mDesc.getNumAllowed());
      }

      ValueLabelElemPanel p;
      int i;
      for ( i = 0; i < n; i++ )
      {
         p = new ValueLabelElemPanel((PropertyDesc.Item) items.get(i));
         elempanels.add (p);
         sppanel.add(p);
      }

      // If a finite number of values are available for mDesc, fill out the
      // remaining (n - i) panels with empty settings.
      if ( ! mDesc.hasVariableNumberOfValues() )
      {
         for ( ; i < mDesc.getNumAllowed(); i++ )
         {
            PropertyDesc.Item new_item =
               new PropertyDesc.Item(mDescValType);
            mDesc.addItem(new_item);
            p = new ValueLabelElemPanel(new_item);
            elempanels.add (p);
            sppanel.add(p);
         }
      }
      sppanel.validate();
      return items.size();
   }



   public void actionPerformed (ActionEvent e)
   {
      ValueLabelElemPanel p;
      int unused,j;
      float k;

      if( e.getSource() == insertbutton )
      {
         PropertyDesc.Item new_item = new PropertyDesc.Item(mDescValType);
         this.mDesc.addItem(new_item);
         p = new ValueLabelElemPanel(new_item);
         elempanels.add (p);
         sppanel.add(p);

         if( elempanels.size() == 1 )
         { // make sure panel wide enough
            setFrameSize();
            Dimension d = getSize();
            Dimension d2 = sppanel.getPreferredSize();
            d.width = Math.max (d.width, d2.width + 42);
            d.width = Math.min (d.width, Core.screenWidth);
            setSize (d);
         }
         validate();
      }
      else if( e.getSource() == removebutton )
      {
         for( int i = 0; i < elempanels.size(); )
         {
            p = (ValueLabelElemPanel)elempanels.get(i);
            if( p.getSelected() )
            {
               sppanel.remove(p);
               elempanels.remove(i);
            }
            else
            {
               i++;
            }
         }
         // validate & repaint both needed
         validate();
         sppanel.repaint (sppanel.getBounds());
      }
      else if( e.getSource() == cancelbutton )
      {
         closeFrame();
      }
      else if( e.getSource() == okbutton )
      {
         unused = 0;
         ArrayList new_labels = new ArrayList();
         VarValue temp_val;

         for( int i = 0; i < elempanels.size(); i++ )
         {
            ValueLabelElemPanel elem_panel =
               (ValueLabelElemPanel) elempanels.get(i);

            try
            {
               elem_panel.commit();
            }
            catch (ConfigUIException ui_ex)
            {
               ui_ex.printStackTrace();
            }
         }
         closeFrame();
      }
   }



   /* WindowListener Stuff */
   public void windowActivated(WindowEvent e)
   {
   }
   public void windowClosed(WindowEvent e)
   {
   }
   public void windowClosing(WindowEvent e)
   {
      closeFrame();
   }
   public void windowDeactivated(WindowEvent e)
   {
   }
   public void windowDeiconified(WindowEvent e)
   {
   }
   public void windowIconified(WindowEvent e)
   {
   }
   public void windowOpened(WindowEvent e)
   {
   }
}
