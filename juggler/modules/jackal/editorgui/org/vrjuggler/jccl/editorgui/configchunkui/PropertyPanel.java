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
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editorgui.*;

/** Panel for displaying a single Property of a ConfigChunk.
 *  This panel will typical contain 0 or more VarValuePanels for the
 *  individual values of the property.
 *
 * @version $Revision$
 */
public class PropertyPanel extends JPanel implements ActionListener
{

   ConfigUIHelper uihelper_module;

   protected static AbstractBorder border =
      new CompoundBorder(new BevelBorder(BevelBorder.RAISED),
                         new EmptyBorder(5,5,5,5));

   private PropertyDesc mPropDesc;
   private String       mPropID;
   private ConfigChunk  mContainerChunk;

   private java.util.List valuepanels;  // vector of VarValuePanel
   private JPanel centerpanel, eastpanel;
   private JButton addbutton;
   protected static String valuelabelpad = "          ";

   protected GridBagLayout eastpanellayout;
   protected GridBagConstraints c;


   public PropertyPanel (PropertyDesc propDesc, ConfigChunk containerChunk,
                         ConfigUIHelper _uihelper_module)
   {
      super();
      uihelper_module = _uihelper_module;

      mPropDesc       = propDesc;
      mPropID         = propDesc.getToken();
      mContainerChunk = containerChunk;
      jbInit();
   }

   private void jbInit ()
   {
      String label;
      int newlen;

      setBorder (border);

      setLayout(new BorderLayout (1,1));
      valuepanels = new Vector();

      JPanel westpanel = new JPanel();
      westpanel.setLayout (new BoxLayout (westpanel, BoxLayout.Y_AXIS));

      JLabel l1 = new JLabel(mPropID, JLabel.LEFT);
      //l1.setFont (core.ui.windowfontbold);
      westpanel.add (l1);

      if( mPropDesc.hasVariableNumberOfValues() )
      {
         // For embedded chunks, we need to add a button that has the type of
         // the embedded chunk.
         if( mPropDesc.getValType() == ValType.EMBEDDEDCHUNK )
         {
            // Is this the best way to get the type of the embedded chunk?
            String chunk_type = mPropDesc.getEnumAtIndex(0).str;
            addbutton = new JButton ("Add " + chunk_type);
         }
         // For other property types, we just have a generic "Add Value"
         // button.
         else
         {
            addbutton = new JButton ("Add Value");
         }

         westpanel.add(addbutton);
         addbutton.addActionListener(this);
      }
      add (westpanel, "West");

      /* add help message, if there is one */
      String h = mPropDesc.getHelp();
      if( !h.equals("") )
      {
         JTextField helparea;
//          if (h.length() < 50)
//              helparea = new JTextField(h, 10);
//          else
         helparea = new JTextField(h,10);
         helparea.setEditable(false);
         add(helparea, "South");
      }

      /* east panel holds the individual PropertyPanels */
      eastpanel = new JPanel();

      eastpanellayout = new GridBagLayout();
      c = new GridBagConstraints();
      eastpanel.setLayout (eastpanellayout);
      c.fill = GridBagConstraints.BOTH;
      c.weightx = 1.0;
      JLabel lj;

      // Loop based on the current number of properties of this type.
      int prop_count = mContainerChunk.getPropertyCount(mPropID);

      for ( int i = 0; i < prop_count; ++i )
      {
         lj = new JLabel();

         if( mPropDesc.getValueLabelsSize() > i )
         {
            lj.setText(valuelabelpad + mPropDesc.getValueLabel(i));
         }
         else if( mPropDesc.getValType() == ValType.EMBEDDEDCHUNK )
         {
            ConfigChunk child_chunk =
               mContainerChunk.getProperty(mPropID, i).getEmbeddedChunk();

            if ( null != child_chunk )
            {
               lj.setText(valuelabelpad + child_chunk.getFullName());
            }
         }
         else
         {
            lj.setText(valuelabelpad);
         }

         c.gridwidth = GridBagConstraints.RELATIVE;
         c.weightx = 0;
         eastpanellayout.setConstraints (lj, c);
         eastpanel.add (lj);
         c.weightx = 1;
         c.gridwidth = GridBagConstraints.REMAINDER;

         VarValuePanel p;
         p = makeVarValuePanel(i);
         valuepanels.add(p);
         eastpanellayout.setConstraints(p, c);
         eastpanel.add (p);
         p.addActionListener (this);
      }

      add(eastpanel,"Center");
   }


   private VarValuePanel makeVarValuePanel (int valindex)
   {
      VarValuePanel retval;
      if( mPropDesc.getValType() == ValType.EMBEDDEDCHUNK )
      {
         VarValue emb_chunk_val =
            mContainerChunk.getProperty(mPropID, valindex);
         ConfigChunk ch = null;

         // XXX: This wouldn't be necessary if getProperty() returned a
         // default/empty value.  (valindex is valid, but there may not be a
         // property value at that index.)
         if ( null != emb_chunk_val )
         {
            ch = emb_chunk_val.getEmbeddedChunk();
         }

         // The container chunk does not have an embedded chunk at the given
         // index, so we must add one.
         if ( ch == null )
         {
            String child_desc = mPropDesc.getEnumAtIndex(0).str;
            ch = ChunkFactory.createChunk(ChunkFactory.getChunkDescByToken(child_desc));

            if( valindex < mPropDesc.getValueLabelsSize() )
            {
               ch.setName(mPropDesc.getValueLabel (valindex));
            }
            else
            {
               ch.setName(mPropDesc.getName() + " " + valindex);
            }

            // The new chunk must be added as a child of the appropriate
            // property of mContainerChunk.
            mContainerChunk.setProperty(mPropID, valindex, new VarValue(ch));
         }

         if( useMiniPanel (ch) )
         {
            retval = new VarValueMiniChunkPanel ();
         }
         else
         {
            retval = new VarValueBigChunkPanel ();
         }

         retval.setConfigUIHelper (uihelper_module);
         retval.setPropertyDesc(mPropDesc);
         retval.setValue (new VarValue (ch));
         return retval;
      }
      else
      {
         VarValue value = mContainerChunk.getProperty(mPropID, valindex);

         // If there is no value for the requested index, fall back on the
         // default.
         if ( null == value )
         {
            value = mPropDesc.getDefaultValue(valindex);
         }

         retval = new VarValueStandardPanel ();
         retval.setConfigUIHelper (uihelper_module);
         retval.setPropertyDesc(mPropDesc);
         retval.setValue(value);
         return retval;
      }
   }



   // helper function for makeVarValuePanel. returns true if we
   // should use the VarValueMiniPanel to handle this embedded
   // chunk, and false if we should give it a separate frame.
   private boolean useMiniPanel (ConfigChunk ch)
   {
      boolean useminipanel = true;
      int maxwidth = 4;

      Vector props = ch.getDesc().getPropertyDescs();
      if( props.size() > maxwidth )
      {
         return false;
      }

      int nvals = 0;
      for( int i = 0; i < props.size(); i++ )
      {
         PropertyDesc cur_prop = (PropertyDesc) props.elementAt(i);

         if ( cur_prop.hasVariableNumberOfValues() ||
              cur_prop.getValType() == ValType.EMBEDDEDCHUNK )
         {
            return false;
         }

         nvals += cur_prop.getNumAllowed();
      }
      return(nvals <= maxwidth);
   }



   public void commit ()
   {
      VarValuePanel panel;

      for ( int i = 0; i < valuepanels.size(); ++i )
      {
         panel = (VarValuePanel) valuepanels.get(i);

         if ( panel.isVisible() )
         {
            mContainerChunk.setProperty(mPropID, i, panel.getValue());
         }
         else
         {
            mContainerChunk.removeProperty(mPropID, i);
         }
      }
   }



   public void actionPerformed (ActionEvent e)
   {
      if( e.getSource() == addbutton )
      {
         int i;
         JLabel lj;

         i = valuepanels.size();
         VarValuePanel p = makeVarValuePanel(i);
         p.addActionListener (this);

         if( i < mPropDesc.getValueLabelsSize() )
         {
            lj = new JLabel (valuelabelpad + mPropDesc.getValueLabel(i));
         }
         else if( mPropDesc.getValType() == ValType.EMBEDDEDCHUNK )
         {
            ConfigChunk child_chunk =
               mContainerChunk.getProperty(mPropID).getEmbeddedChunk();
            lj = new JLabel(valuelabelpad + child_chunk.getFullName());
         }
         else
         {
            lj = new JLabel (valuelabelpad);
         }

         lj.setBackground (Color.green);
         c.gridwidth = GridBagConstraints.RELATIVE;
         c.weightx = 0;
         eastpanellayout.setConstraints (lj, c);
         eastpanel.add (lj);
         c.weightx = 1;
         c.gridwidth = GridBagConstraints.REMAINDER;

         valuepanels.add(p);
         //c.gridwidth = GridBagConstraints.REMAINDER;
         eastpanellayout.setConstraints(p,c);
         eastpanel.add (p);

         eastpanel.revalidate(); // yes, we do need this one :(
      }
      else if( e.getSource() instanceof VarValuePanel )
      {
         if( e.getActionCommand().equals ("Remove") )
         {
            VarValuePanel p = (VarValuePanel)e.getSource();

            // XXX: This is a hack to deal with removal of property value
            // settings.  Something better needs to be implemented.
            // (PH 5/13/2002)
            p.setVisible(false);

            // annoying but true: we need to remove p and the label component left of it
            Component comps[] = eastpanel.getComponents();
            for( int i = 0; i < comps.length; i++ )
            {
               if( comps[i] == p )
               {
                  eastpanel.remove (comps[i-1]);
                  eastpanel.remove(p);
                  eastpanel.revalidate(); // forces resize of container
                  break;
               }
            }
         }
         else if( e.getActionCommand().equals ("Rename") )
         {
            VarValuePanel p = (VarValuePanel)e.getSource();

            Component comps[] = eastpanel.getComponents();
            for( int i = 0; i < comps.length; i++ )
            {
               if( comps[i] == p )
               {
                  JLabel l = (JLabel)(comps[i-1]);
                  ConfigChunk child_chunk =
                     mContainerChunk.getProperty(mPropID).getEmbeddedChunk();

                  l.setText (valuelabelpad + child_chunk.getFullName());
                  //eastpanel.remove (comps[i-1]);
                  //eastpanel.remove(p);
                  eastpanel.revalidate(); // forces resize of container
                  break;
               }
            }
         }
      }
   } // actionPerformed()
}
