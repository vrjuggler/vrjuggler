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

package prefstestbean;

import java.awt.*;
import javax.swing.*;
import org.vrjuggler.tweek.beans.*;


/**
 * @version 1.0
 */
public class PrefsTestBean extends JPanel implements java.io.Serializable,
                                                     BeanPreferences,
                                                     PreferencesEditListener
{
   public PrefsTestBean ()
   {
      try
      {
         editor.load();
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      editor.addPreferencesEditListener(this);
   }

   public void initPrefsGUI()
   {
      editor.initGUI();
   }

   public JComponent getEditor ()
   {
      return editor;
   }

   public String getEditorName ()
   {
      return editor.getEditorName();
   }

   public void load () throws java.io.IOException
   {
      editor.load();
   }

   public boolean canCommit ()
   {
      return editor.canCommit();
   }

   public void save () throws java.io.IOException
   {
      editor.save();
   }

   public void preferencesEdited (PreferencesEditEvent e)
   {
      this.setBackground(editor.getCurrentBackgroundColor());
      mSampleTextField.setText(editor.getSampleFieldText());
   }

   private void jbInit() throws Exception
   {
      mSampleTextLabel.setText("Some text:");
      this.setBackground(editor.getCurrentBackgroundColor());
      mSampleTextField.setBackground(Color.lightGray);
      mSampleTextField.setBorder(BorderFactory.createLoweredBevelBorder());
      mSampleTextField.setMinimumSize(new Dimension(150, 17));
      mSampleTextField.setPreferredSize(new Dimension(200, 17));
      mSampleTextField.setEditable(false);
      mSampleTextField.setText(editor.getSampleFieldText());
      this.add(mSampleTextLabel, null);
      this.add(mSampleTextField, null);
   }

   private PrefsEditor editor = new PrefsEditor();

   private JLabel     mSampleTextLabel = new JLabel();
   private JTextField mSampleTextField = new JTextField();
}
