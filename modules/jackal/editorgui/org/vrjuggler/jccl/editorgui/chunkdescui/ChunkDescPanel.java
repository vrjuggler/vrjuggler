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
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editorgui.*;
import org.vrjuggler.jccl.editorgui.configchunkui.*;
import org.vrjuggler.jccl.vjcontrol.*;
import org.vrjuggler.jccl.vjcontrol.ui.EditorPanel;
import org.vrjuggler.jccl.vjcontrol.ui.widgets.*;

/** A panel for viewing/editing a single ChunkDesc
 *
 * @version $Revision$
 */
public class ChunkDescPanel extends JPanel implements EditorPanel,
                                                      ActionListener,
                                                      DescDBListener
{
   private String mOrigDescName = "";
   private ChunkDesc mDesc;
   ChunkDescDB descdb;
   Vector proppanels; // property description panels.
   JPanel properties;
   JTextField namefield;
   JTextField tokenfield;
   JTextField helpfield;
   JButton removebutton;
   JButton insertbutton;
   JButton edit_defaults_button;
   JButton clear_defaults_button;
   boolean editable;
   JPanel mainpanel, buttonspanel;
   ConfigUIHelper confighelper_module; //needed to edit default values
   GenericEditorFrame default_chunk_frame;

   public ChunkDescPanel (ChunkDesc _desc,
                          ChunkDescDB _descdb,
                          ConfigUIHelper _confighelper_module,
                          boolean _editable)
   {

      JScrollPane sp;
      PropertyDescPanel t;

      editable = _editable;
      mOrigDescName = _desc.getName();
      mDesc = _desc;
      descdb = _descdb;
      confighelper_module = _confighelper_module;
      proppanels = new Vector();
      default_chunk_frame = null;

      //setFont(core.ui.windowfont);

      /******************* North Panel **********************/

      setBorder (BorderFactory.createEmptyBorder (3,3,3,3));

      JLabel l;
      GridBagLayout gbl;
      GridBagConstraints gbc = new GridBagConstraints();
      setLayout (gbl = new GridBagLayout());

      gbc.fill = gbc.HORIZONTAL;
      gbc.insets = new Insets (2,4,2,4);
      gbc.gridwidth = 1;

      l = new JLabel ("ChunkDesc Name: ", JLabel.RIGHT);
      gbl.setConstraints (l, gbc);
      add (l);

      namefield = new StringTextField(mDesc.getName(), 15);
      gbc.weightx = 0.5;
      gbl.setConstraints (namefield, gbc);
      add (namefield);

      l = new JLabel ("Token: ", JLabel.RIGHT);
      gbc.weightx = 0;
      gbl.setConstraints (l, gbc);
      add (l);

      tokenfield = new NoSpaceTextField(mDesc.getToken(), 15);
      gbc.gridwidth = gbc.REMAINDER;
      gbc.weightx = 0.5;
      gbl.setConstraints (tokenfield, gbc);
      add (tokenfield);

      l = new JLabel ("Help String: ", JLabel.RIGHT);
      gbc.gridwidth = 1;
      gbc.weightx = 0;
      gbl.setConstraints (l, gbc);
      add (l);

      helpfield = new StringTextField(mDesc.getHelp(), 50);
      gbc.weightx = 1;
      gbc.gridwidth = gbc.REMAINDER;
      gbl.setConstraints (helpfield, gbc);
      add (helpfield);


      /******************* Center Panel ********************/
      properties = new JPanel ();
      sp = new JScrollPane(properties, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                           JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
      sp.getVerticalScrollBar().setUnitIncrement(40);
      sp.getHorizontalScrollBar().setUnitIncrement(40);

      gbc.weighty = 1;
      gbc.weightx = 1;
      gbc.fill = gbc.BOTH;
      gbc.gridheight = gbc.RELATIVE;
      gbl.setConstraints (sp, gbc);
      add(sp);

      properties.setLayout (new BoxLayout (properties, BoxLayout.Y_AXIS));

      // make property description panels
      Vector props = mDesc.getPropertyDescs();
      for (int i = 0; i < props.size(); i++)
      {
         PropertyDesc cur_desc = (PropertyDesc) props.elementAt(i);
         t = new PropertyDescPanel(this,
                                   !cur_desc.getName().equalsIgnoreCase("name") && editable,
                                   cur_desc);
         proppanels.add(t);
         properties.add(t);
      }

      /****************** South Panel ***********************/

      if (editable)
      {
         buttonspanel = new JPanel();
         insertbutton = new JButton ("Insert Property");
         insertbutton.addActionListener (this);
         buttonspanel.add (insertbutton);
         removebutton = new JButton ("Remove Property");
         removebutton.addActionListener (this);
         buttonspanel.add (removebutton);

         gbc.weighty = 0;
         gbc.fill = gbc.HORIZONTAL;
         gbc.gridwidth = 1;
         gbc.gridheight = gbc.REMAINDER;
         gbc.gridwidth = gbc.REMAINDER;
         gbl.setConstraints (buttonspanel, gbc);
         add (buttonspanel);
      }

      descdb.addDescDBListener (this);

   }

   public String getOrigDescName ()
   {
      return mOrigDescName;
   }

   public ChunkDesc getDesc ()
   {
      return mDesc;
   }

   public ChunkDescDB getDescDB()
   {
      return descdb;
   }



   public String getName ()
   {
      return mDesc.getName();
   }



   public void actionPerformed (ActionEvent e)
   {
      Object source = e.getSource();
      if (source == removebutton)
      {
         PropertyDescPanel p;

         for ( int i = 0; i < proppanels.size(); ++i )
         {
            p = (PropertyDescPanel)proppanels.elementAt(i);
            if (p.getSelected())
            {
               p.setVisible(false);
            }
         }
         // validate & repaint needed to get components to "shift up"
         // after removing one in the middle.
         validate();
         repaint();
      }
      else if (source == insertbutton)
      {
         PropertyDesc fresh_desc = new PropertyDesc();
         this.mDesc.addPropertyDesc(fresh_desc);
         PropertyDescPanel t = new PropertyDescPanel(this, true, fresh_desc);
         t.setItemEditorEnabled(false);
         proppanels.addElement(t);
         properties.add(t);
         //setReasonableSize();
         validate();
      }
   }

   public void commit () throws ConfigUIException
   {
      // returns the value currently shown in the panel...
      if (tokenfield.getText().equals(""))
      {
         String msg = "Token value must be set for chunk descriptions";
         JOptionPane.showMessageDialog(null, msg, "Invalid Chunk Description",
                                       JOptionPane.ERROR_MESSAGE);
         throw new ConfigUIException(msg);
      }

      mDesc.setToken(tokenfield.getText());
      mDesc.setHelp(helpfield.getText());
      mDesc.setName(namefield.getText());

      if ( mDesc.getName().equals("") )
      {
         mDesc.setName(mDesc.getToken());
      }

      java.util.Iterator i = proppanels.iterator();
      while ( i.hasNext() )
      {
         PropertyDescPanel desc_panel = (PropertyDescPanel) i.next();

         if ( desc_panel.isVisible() )
         {
            desc_panel.commit();
            desc_panel.setItemEditorEnabled(true);
         }
         else
         {
            mDesc.removePropertyDesc(desc_panel.getPropertyDesc());
         }
      }
   }

   //----------------------- EditorPanel Stuff ------------------------------

   public void showHelp ()
   {
   }


   public void setComponentName (String _name)
   {
      ; // determined by the description assigned to this panel.
   }


   public String getComponentName ()
   {
      return mDesc.getName();
   }


   public VjComponent addConfig (ConfigChunk ch) throws VjComponentException {
      throw new VjComponentException ("ChunkDescPanel does not support child component: " + ch.getName());
   }


   public boolean removeConfig (String name)
   {
      return false;
   }

   /* is this safe? */
   public ConfigChunk getConfiguration ()
   {
      return null;
   }


   public void setConfiguration (ConfigChunk ch) throws VjComponentException {
      ;
   }


   public void initialize () throws VjComponentException {
      ;
   }



   //------------------------ ChildFrame Stuff ------------------------------

   public void destroy ()
   {
      if(descdb != null)
      {
         descdb.removeDescDBListener (this);
      }

      int n = proppanels.size();
      for (int i = 0; i < n; i++)
      {
         ((PropertyDescPanel)proppanels.elementAt(i)).closeFrames();
      }
      if (default_chunk_frame != null)
      {
         default_chunk_frame.destroy();
      }
   }


   public boolean matches (String cl, Object db, Object o)
   {
      if(cl != null)
      {
         try
         {
            if (!(Class.forName(cl).isInstance(this)))
            {
               return false;
            }
         }
         catch(Exception e)
         {
            return false;
         }
      }

      if (db != descdb)
      {
         return false;
      }

      if (o == null)
      {
         return true;
      }
      else if (o instanceof ChunkDesc)
      {
         return ((ChunkDesc)o).getToken().equals(mDesc.getToken());
      }
      else
      {
         return false;
      }
   }


   //------------------- DescDBListener stuff ----------------------
   public void chunkDescAdded (DescDBEvent e)
   {
      ;
   }

   public void chunkDescRemoved (DescDBEvent e)
   {
      ;
   }

   public void chunkDescReplaced (DescDBEvent e)
   {
      // basically we're checking for when an apply comes across, so we
      // can correct exactly which chunk we're supposed to be editing.
      // at some point, we should add the ability to completely update the
      // displayed state; that'll be helpful when we get more "live"
      // information from juggler.  (it'd probably be helpful at that
      // point if we can query the ConfigModule about whether this is an
      // "active db" or not - if not, then we shouldn't have to do a full
      // update).
//      if (e.getOldDesc() == mDesc)
//      {
//         mDesc = e.getNewDesc();
//      }
   }

   public void chunkDescsCleared (DescDBEvent e)
   {
      ;
   }
}
