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
package org.vrjuggler.jccl.editorgui.configchunkui;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editorgui.*;
import org.vrjuggler.jccl.vjcontrol.Core;
import org.vrjuggler.jccl.vjcontrol.ui.widgets.*;

/**
 * VarValuePanel is just a little panel that we'll stick several of
 * in a PropertyPanel.
 *
 *     -----------------------------------------------------
 *     |                                                   |
 *     | Type(optional) valuegadget  removebutton(optional)|
 *     |                                                   |
 *     -----------------------------------------------------
 *
 * the value gadget is going to be either a TextArea or a multisetSelectedItem
 * with the acceptable values in it.  removebutton will be there if the
 * associated property has var numbers of values.
 */
public class VarValueStandardPanel extends VarValuePanel
                                   implements ActionListener
{

   java.util.List              action_listeners;
   PropertyDesc      desc;
   JTextField        text;
   JComboBox         choice;   // note that only one of text,choice will be used.
   JButton           removebutton;

   ConfigUIHelper    uihelper_module;

   public VarValueStandardPanel ()
   {
      super();
      desc = null;
      uihelper_module = null;
      action_listeners = new ArrayList();

      setLayout(new BorderLayout (1,1));
   }


   public void setConfigUIHelper (ConfigUIHelper helper)
   {
      if( uihelper_module == null )
      {
         uihelper_module = helper;
         if( (desc != null) && (desc.getValType() == ValType.CHUNK) )
         {
            choice.setModel (generateChunkSelectionList (desc.getEnumerations()));
            choice.setSelectedItem ("<No Selection>");
         }
      }
   }


   public void setPropertyDesc (PropertyDesc _desc)
   {
      if( desc == null )
      {
         desc = _desc;
         int i, j, k;

         if( desc.getValType() == ValType.CHUNK )
         {
            /* we build up a choice of all chunks in the db of allowed types */
            choice = new JComboBox ();
            choice.setEditable (true);
            add (choice, "Center");

            if( uihelper_module != null )
            {
               choice.setModel (generateChunkSelectionList (desc.getEnumerations()));
               choice.setSelectedItem ("<No Selection>");
            }
         }
         else if( desc.getEnumerationsSize() > 0 )
         {
            /* just present the enumeration names as choices */
            choice = new JComboBox();
            ListBoxModel bm = new ListBoxModel();
            //bm.addObject ("<No Selection>"); may not be safe for enums
            DescEnum[] e = desc.getEnumerations();

            for( i = 0; i < e.length; i++ )
            {
               bm.addElement(e[i].str);
            }

            choice.setModel(bm);
            choice.setSelectedIndex (0);
            add(choice, "Center");
         }
         else if( desc.getValType() == ValType.BOOL )
         {
            choice = new JComboBox();
            choice.addItem("1");
            choice.addItem("0");
            choice.setRenderer(new TrueFalseCellRenderer());
            add(choice,"East");
         }
         else
         {
            /* just add a type label & the text string */
            add (new JLabel ("(" + desc.getValType().toString() + ")"), "West");

            if ( desc.getValType() == ValType.INT )
            {
               text = new IntegerTextField (5);
            }
            else if ( desc.getValType() == ValType.FLOAT )
            {
               text = new FloatTextField (5);
            }
            else
            {
               text = new StringTextField (10);
            }

            add(text,"Center");
         }

         if( desc.hasVariableNumberOfValues() )
         {
            /* then it's a variable # of values */
            removebutton = new JButton("Remove");
            Insets in = new Insets (0,0,0,0);
            removebutton.setMargin (in);
            add (removebutton,"East");
            removebutton.addActionListener(this);
         }

      }
   }


   /** Utility for generateChunkSelectionList(). */
   private void addEmbeddedChunks (ListBoxModel bm, ConfigChunk parentChunk,
                                   DescEnum[] allowedChunkTypes)
   {
      java.util.List v = parentChunk.getEmbeddedChunks();
      ConfigChunk child_chunk;

      for( int i = 0; i < v.size(); i++ )
      {
         child_chunk = (ConfigChunk)v.get(i);
         if ( matchesTypes (child_chunk, allowedChunkTypes) )
         {
            bm.addElement(child_chunk.getFullName());
         }

         addEmbeddedChunks(bm, child_chunk, allowedChunkTypes);
      }
   }


   /**
    * Creates a list box model object containing all the available chunks
    * that match the given array of allowed types.  This method has got to be
    * really, really slow.
    */
   private ListBoxModel generateChunkSelectionList (DescEnum[] allowedChunkTypes)
   {
      Vector v;
      ListBoxModel bm = new ListBoxModel();
      ConfigChunkDB db;

      ConfigModule config_module = uihelper_module.getConfigModule();

      bm.addElement ("<No Selection>");

      // Search each of the currently loaded config files (chunk DB's).
      for ( int i = 0; i < config_module.chunkdbs.size(); i++ )
      {
         db = (ConfigChunkDB)config_module.chunkdbs.elementAt(i);

         for ( int j = 0; j < db.size(); j++ )
         {
            ConfigChunk current_chunk = db.get(j);

            // Compare the types of the current chunk with those in the
            // array of allowed types.
            if ( matchesTypes(current_chunk, allowedChunkTypes) )
            {
               bm.addElement(db.getName() + ": " + current_chunk.getName());
            }

            // Now we have to check for chunks that are children of
            // current_chunk.  They may also match.
            addEmbeddedChunks(bm, current_chunk, allowedChunkTypes);
         }
      }

      return bm;
   }


   /** Utility for generateChunkSelectionList(). */
   private boolean matchesTypes (ConfigChunk ch, DescEnum[] chunktypes)
   {
      int k;
      if ( chunktypes == null )
      {
         return true;
      }

      for( k = 0; k < chunktypes.length; k++ )
      {
         if ( ch.getDescToken().equals(chunktypes[k].str) )
         {
            return true;
         }
      }
      return false;
   }



   /** Set the VarValue to be displayed in this panel.
    *  @param v - a non-null VarValue.
    */
   public void setValue (VarValue v)
   {
      // sets the displayed value.
      if( choice != null )
      {
         choice.setSelectedItem(desc.getEnumString(v));
      }
      else
      {
         /* set text in textbox */
         text.setText(v.toString());
      }
   }



   public VarValue getValue ()
   {
      VarValue v = new VarValue(desc.getValType());
      if( choice != null )
      {
         /* enumeration */
         String item = (String)choice.getSelectedItem();
         if ( item.equals("<No Selection>") )
         {
            return v;  // return 'default' varvalue
         }

         if( desc.getValType() == ValType.CHUNK )
         {
            // remove the "filename: " prefix if it's there...
            int i = item.indexOf(": ");
            if( i != -1 )
            {
               v.set(item.substring(i+2));
            }
            else
            {
               v.set(item);
            }

            return v;
         }

         VarValue v2 = desc.getEnumValue(item);
         if( v2 != null )
         {
            v.set(v2);
            return v;
         }

         v.set(item);
         return v;
      }
      else
      {
         v.set (text.getText());
         return v;
      }
   }



   public void actionPerformed (ActionEvent e)
   {
      if( e.getSource() == removebutton )
      {
         notifyActionListenersRemove();
      }
   }


   //--------------------- ActionEvent Stuff ------------------------

   public void addActionListener (ActionListener l)
   {
      synchronized (action_listeners)
      {
         action_listeners.add (l);
      }
   }

   public void removeActionListener (ActionListener l)
   {
      synchronized (action_listeners)
      {
         action_listeners.remove (l);
      }
   }

   private void notifyActionListenersRemove ()
   {
      ActionEvent e = new ActionEvent (this, ActionEvent.ACTION_PERFORMED,
                                       "Remove");
      ActionListener l;
      int i, n;
      synchronized (action_listeners)
      {
         n = action_listeners.size();
         for( i = 0; i < n; i++ )
         {
            l = (ActionListener)action_listeners.get(i);
            l.actionPerformed (e);
         }
      }
   }
}
