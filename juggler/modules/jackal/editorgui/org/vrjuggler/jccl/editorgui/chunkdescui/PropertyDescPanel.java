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
import org.vrjuggler.jccl.editorgui.ConfigUIException;
import org.vrjuggler.jccl.vjcontrol.ui.widgets.*;


/** Panel for displaying a single Property of a ConfigChunk.
 *
 * @version $Revision$
 */
public class PropertyDescPanel extends JPanel
implements ItemListener, MouseListener, ActionListener
{

   Vector                newenum;
   ChunkDescPanel        parent;
   PropertyDesc          mDesc;
   private ValType       mCurDescValType = null;
   JTextField             name_field;
   JTextField             help_field;
   JTextField             token_field;
   JCheckBox              varbox;
   JTextField             num_field;
   JComboBox                typechoice;
   JButton                enumsbutton;
   JButton                valuelabelsbutton;
   GridBagLayout         gblayout;
   DescEnumFrame         enumsframe;
   ValueLabelFrame         valuesframe;
   boolean               selected;
   boolean mEditable;

   static AbstractBorder select_border=null;
   static AbstractBorder unselect_border=null;

   /**
    * Returns the PropertyDesc object being edited by this panel.  This is
    * package visible since nothing outside this class' package should need
    * access to this method.
    *
    * @note The fact that this method exists is a hack to allow removal of
    *       properties.
    */
   PropertyDesc getPropertyDesc ()
   {
      return mDesc;
   }

   /**
    * Commits the changes made to mDesc in this panel.
    */
   public void commit () throws ConfigUIException
   {
      mDesc.setToken(token_field.getText());

      if ( mDesc.getToken().equals("") )
      {
         throw new ConfigUIException("No property description token specified");
      }

      mDesc.setName(name_field.getText());
      mDesc.setHelp(help_field.getText());
      mDesc.setValType(mCurDescValType);

      if ( varbox.isSelected() )
      {
         mDesc.setHasVariableNumberOfValues(true);
      }
      else
      {
         mDesc.setNumAllowed(Integer.parseInt(num_field.getText()));
      }

      mDesc.setEnumerations(newenum);

      if ( mDesc.getName().equals("") )
      {
         mDesc.setName(mDesc.getToken());
      }
   }

   public PropertyDescPanel (ChunkDescPanel p,
                             boolean editable,
                             PropertyDesc d )
   {
      /* Creates a new PropertyDescPanel with its components
       * initialized with the values in d
       */
      super();

      mDesc = d;
      mCurDescValType = d.getValType();

      if ( ValType.INVALID == mCurDescValType )
      {
         mCurDescValType = ValType.INT;
      }

      parent = p;
      mEditable = editable;
      jbInit();
      // for safety's sake we copy all the descenums, just so that we
      // don't accidentally modify the old PropertyDesc.
      DescEnum[] e = d.getEnumerations();
      for ( int i = 0; i < e.length; i++ )
      {
         newenum.add(new DescEnum(e[i]));
      }
   }



   private void addLabel (String s)
   {
      /* just a convenience used below */
      JLabel l = new JLabel (s);
      l.addMouseListener(this);
      add (l);
   }



   private void jbInit ()
   {
      /* Does common initialization used by all constructors */

      if( select_border == null )
      {
         select_border = new CompoundBorder ( new BevelBorder (BevelBorder.LOWERED),
                                              new EmptyBorder (5,5,5,5));
      }
      if( unselect_border == null )
      {
         unselect_border = new CompoundBorder ( new BevelBorder (BevelBorder.RAISED),
                                                new EmptyBorder (5,5,5,5));
      }

      setBorder (unselect_border);

      newenum = new Vector();

      GridBagConstraints gbc = new GridBagConstraints();
      selected = false;
      addMouseListener(this);
      enumsframe = null;
      addLabel ("Name");
      gblayout = new GridBagLayout();
      setLayout (gblayout);

      name_field = new StringTextField(mDesc.getName(), 15);
      gbc.gridwidth = 2;
      gblayout.setConstraints(name_field,gbc);
      add (name_field);
      addLabel ("Token");
      token_field = new NoSpaceTextField(mDesc.getToken(), 15);
      gbc.gridwidth = GridBagConstraints.RELATIVE;
      gblayout.setConstraints(token_field,gbc);
      add (token_field);

      gbc.gridwidth = GridBagConstraints.REMAINDER;
      valuelabelsbutton = new JButton ("Edit Item Properties");
      gblayout.setConstraints (valuelabelsbutton, gbc);
      valuelabelsbutton.addActionListener(this);
      add (valuelabelsbutton);

      varbox = new JCheckBox("Var Arguments", (mDesc.hasVariableNumberOfValues()));
      add (varbox);
      varbox.addItemListener(this);
      addLabel ("Number");
      num_field = new IntegerTextField(Integer.toString(mDesc.getNumAllowed()), 3);
      if( mDesc.hasVariableNumberOfValues() )
      {
         num_field.setEnabled(false);
      }

      add (num_field);
      addLabel ("Type");
      typechoice = new JComboBox();
      typechoice.addItem(ConfigTokens.int_TOKEN);
      typechoice.addItem(ConfigTokens.float_TOKEN);
      typechoice.addItem(ConfigTokens.bool_TOKEN);
      typechoice.addItem(ConfigTokens.string_TOKEN);
      typechoice.addItem(ConfigTokens.chunk_TOKEN);
      typechoice.addItem(ConfigTokens.embeddedchunk_TOKEN);
      typechoice.setSelectedItem(mCurDescValType.toString());
      add (typechoice);
      typechoice.addActionListener (this);

      enumsbutton = new JButton ("Edit Enumerations");
      setEnumsButton ((String)typechoice.getSelectedItem());
      enumsbutton.addActionListener(this);
      gblayout.setConstraints(enumsbutton,gbc);
      add(enumsbutton);

      addLabel ("Help Text");
      help_field = new StringTextField(mDesc.getHelp(), 45);
      gbc.gridwidth = GridBagConstraints.REMAINDER;
      gbc.fill = GridBagConstraints.HORIZONTAL;
      gblayout.setConstraints(help_field,gbc);
      add (help_field);

      if( mEditable == false )
      {
         name_field.setEditable(false);
         token_field.setEditable(false);
         num_field.setEditable(false);
         help_field.setEditable(false);
         varbox.setEnabled(false);
         typechoice.setEnabled(false);
         enumsbutton.setEnabled(false);
         valuelabelsbutton.setEnabled(false);
      }

      Dimension d1 = getPreferredSize();
      Dimension d2 = getMaximumSize();
      d2.height = d1.height;
      setMaximumSize(d2);

   }



   public void setSelected(boolean v)
   {
      selected = v;
      if( selected )
      {
         setBorder(select_border);
      }
      else
      {
         setBorder(unselect_border);
      }
      repaint();
   }



   public void toggleSelected()
   {
      setSelected(!selected);
   }



   public boolean getSelected()
   {
      return selected;
   }



   public void itemStateChanged (ItemEvent e)
   {
      if( e.getSource() == varbox )
      {
         num_field.setEnabled (!varbox.isSelected());
      }
   }



   private void setEnumsButton (String s)
   {
      if( s.equals(ConfigTokens.embeddedchunk_TOKEN) )
      {
         enumsbutton.setText ("Inner Chunk Type");
      }
      else if( s.equals(ConfigTokens.chunk_TOKEN) )
      {
         enumsbutton.setText ("Set Allowed Types");
      }
      else
      {
         enumsbutton.setText ("Edit Enumerations");
      }
   }



   public void actionPerformed (ActionEvent e)
   {
      if( e.getSource() == typechoice )
      {
         String val_type_str = (String) typechoice.getSelectedItem();

         mCurDescValType = ValType.getValType(val_type_str);
         setEnumsButton(val_type_str);
      }

      if( e.getSource() == valuelabelsbutton )
      {
         if( (valuesframe == null) || valuesframe.closed )
         {
            valuesframe = new ValueLabelFrame(this, mDesc, mCurDescValType,
                                              name_field.getText());
         }
      }
      else if( e.getSource() == enumsbutton )
      {
         if( (enumsframe == null) || enumsframe.closed )
         {
            if ( ((String)typechoice.getSelectedItem()).equals(ConfigTokens.embeddedchunk_TOKEN) )
            {
               enumsframe = new DescEnumFrame (this, newenum,
                                               name_field.getText(),
                                               ValType.getValType((String)typechoice.getSelectedItem()),
                                               false, 1);
            }
            else
            {
               enumsframe = new DescEnumFrame (this, newenum,
                                               name_field.getText(),
                                               ValType.getValType((String)typechoice.getSelectedItem()),
                                               true, 0);
            }
         }
      }
   }



   public void closeFrames ()
   {
      if( enumsframe != null )
      {
         enumsframe.closeFrame();
         enumsframe = null;
      }
      if( valuesframe != null )
      {
         valuesframe.closeFrame();
         valuesframe = null;
      }
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

   // XXX: This method is part of a hack to deal with my inability to come up
   // with a good way to edit default values for a newly created property
   // description prior to committing the property descsription additon.  That
   // is to say that I could not come up with a viable solution that did not
   // require drastic changes to the way that propery descriptions within a
   // given chunk description are edited.
   // (PH 5/15/2002)
   void setItemEditorEnabled (boolean enableVal)
   {
      valuelabelsbutton.setEnabled(enableVal);
      enumsbutton.setEnabled(enableVal);
   }
}
