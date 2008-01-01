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

package prefstestbean;

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.FileWriter;
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;
import org.jdom.*;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;
import org.vrjuggler.tweek.beans.*;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;


/**
 * This is a simple class to demonstrate how a Bean can provide an editor for
 * its specific preferences.
 */
public class PrefsEditor extends JPanel
{
   public PrefsEditor ()
   {
      // Set the file to load to $HOME/.tweek_prefs_test.
      mPrefsFile = new File(getUserHome() + File.separator +
                            ".tweek_prefs_test");
   }

   public synchronized void addPreferencesEditListener (PreferencesEditListener l)
   {
      mListeners.add(l);
   }

   public synchronized void removePreferencesEditListener (PreferencesEditListener l)
   {
      mListeners.remove(l);
   }

   /**
    * Informs listeners that the preferences have been edited.
    */
   public void firePreferencesEditEvent (Object source)
   {
      PreferencesEditEvent e = new PreferencesEditEvent(source);

      Vector listeners = null;

      synchronized (mListeners)
      {
         listeners = (Vector) mListeners.clone();
      }

      java.util.Iterator i = listeners.iterator();

      while ( i.hasNext() )
      {
         ((PreferencesEditListener) i.next()).preferencesEdited(e);
      }
   }

   /**
    * Initializes the GUI for the preferences editor.
    */
   public void initGUI ()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   public String getEditorName ()
   {
      return "Prefs Test";
   }

   public void load () throws java.io.IOException
   {
      // If the preferences file exists, we read it into a DOM tree.
      if ( mPrefsFile.exists() )
      {
         org.jdom.input.SAXBuilder builder = new org.jdom.input.SAXBuilder();

         try
         {
            mPrefsDoc     = builder.build(mPrefsFile);
            mPrefsDocRoot = mPrefsDoc.getRootElement();

            // Handle the elements.
            Element bg_color_element = mPrefsDocRoot.getChild("bgcolor");
            int red   = bg_color_element.getAttribute("red").getIntValue();
            int green = bg_color_element.getAttribute("green").getIntValue();
            int blue  = bg_color_element.getAttribute("blue").getIntValue();
            currentBackgroundColor = new Color(red, green, blue);

            Element sample_field_element = mPrefsDocRoot.getChild("field");
            sampleFieldText = sample_field_element.getAttributeValue("text");
         }
         catch (JDOMException e)
         {
            e.printStackTrace();
         }
      }
      // Build the preferences document by hand using defaults.
      else
      {
         mPrefsDocRoot = new Element("PrefsTest");
         mPrefsDoc     = new Document(mPrefsDocRoot);

         Element bg_color_element = new Element("bgcolor");
         bg_color_element.setAttribute("red",
                                       String.valueOf(currentBackgroundColor.getRed()));
         bg_color_element.setAttribute("green",
                                       String.valueOf(currentBackgroundColor.getGreen()));
         bg_color_element.setAttribute("blue",
                                       String.valueOf(currentBackgroundColor.getBlue()));
         mPrefsDocRoot.addContent(bg_color_element);

         Element sample_field_element = new Element("field");
         sample_field_element.setAttribute("text", sampleFieldText);
         mPrefsDocRoot.addContent(sample_field_element);

         writePrefs(true);
      }
   }

   /**
    * For this simple example, committing of preferences can always happen.
    */
   public boolean canCommit ()
   {
      return true;
   }

   public void save () throws java.io.IOException
   {
      setSampleFieldText(mSampleTextField.getText());
      writePrefs(false);
      firePreferencesEditEvent(this);
   }

   /**
    * Sets the preferred background color.  The button that opens the color
    * chooser has its background color set to the given color too.  The element
    * in the DOM tree that stores the color preference is updated based on the
    * Color object.
    */
   public void setCurrentBackgroundColor (Color color)
   {
      Element e = mPrefsDocRoot.getChild("bgcolor");

      if ( e == null )
      {
         e = new Element("bgcolor");
         mPrefsDocRoot.addContent(e);
      }

      e.setAttribute("red", String.valueOf(color.getRed()));
      e.setAttribute("green", String.valueOf(color.getGreen()));
      e.setAttribute("blue", String.valueOf(color.getBlue()));
      currentBackgroundColor = color;
      mEditColorButton.setBackground(currentBackgroundColor);
   }

   /**
    * Returns the current preferred background color.
    */
   public Color getCurrentBackgroundColor ()
   {
      return currentBackgroundColor;
   }

   public void setSampleFieldText (String text)
   {
      Element e = mPrefsDocRoot.getChild("field");

      if ( e == null )
      {
         e = new Element("field");
         mPrefsDocRoot.addContent(e);
      }

      e.setAttribute("text", text);
      sampleFieldText = text;
   }

   public String getSampleFieldText ()
   {
      return sampleFieldText;
   }

   // =========================================================================
   // Private methods.
   // =========================================================================

   private void jbInit() throws Exception
   {
      mBgColorBorder = new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Background Color Editor");
      mSampleFieldBorder = new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Sample Field Editor");
      mMainLayout.setColumns(1);
      mMainLayout.setRows(0);
      this.setLayout(mMainLayout);
      mBgColorPanel.setBorder(mBgColorBorder);
      mBgColorPanel.setLayout(flowLayout1);
      mSampleFieldPanel.setBorder(mSampleFieldBorder);
      mSampleFieldPanel.setLayout(mSampleFieldPanelLayout);
      mEditColorButton.setBackground(currentBackgroundColor);
      mEditColorButton.setMaximumSize(new Dimension(45, 13));
      mEditColorButton.setMinimumSize(new Dimension(45, 13));
      mEditColorButton.setPreferredSize(new Dimension(75, 23));
      mEditColorButton.setText("Edit ...");
      mEditColorButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            colorEditButtonClicked(e);
         }
      });
      mCurrentColorLabel.setMaximumSize(new Dimension(65, 12));
      mCurrentColorLabel.setMinimumSize(new Dimension(65, 12));
      mCurrentColorLabel.setPreferredSize(new Dimension(115, 12));
      mCurrentColorLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mCurrentColorLabel.setText("Current color:");
      mSampleTextField.setText(sampleFieldText);
      this.setMinimumSize(new Dimension(310, 160));
      this.setPreferredSize(new Dimension(310, 160));
      this.add(mBgColorPanel, null);
      mBgColorPanel.add(mCurrentColorLabel, null);
      mBgColorPanel.add(mEditColorButton, null);
      this.add(mSampleFieldPanel, null);
      mSampleFieldPanel.add(mSampleTextField,      new GridBagConstraints(0, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 5, 0, 5), 0, 0));
   }

   /**
    * Uses the Tweek Environment Service to look up the user's home directory.
    * If the lookup fails, the current working directory is returned.
    */
   private String getUserHome()
   {
      String path = null;

      try
      {
         EnvironmentService env_service = new EnvironmentServiceProxy();
         path = env_service.getUserHome();
      }
      catch(RuntimeException e)
      {
         /* Oh well... */ ;
      }

      return path;
   }

   private void colorEditButtonClicked (ActionEvent e)
   {
      JDialog dialog = JColorChooser.createDialog(this,
                                                  "Background Color Chooser",
                                                  true, mBgColorChooser, null,
                                                  null);
      dialog.setVisible(true);
      setCurrentBackgroundColor(mBgColorChooser.getColor());
   }

   /**
    * Saves the current prefernces document.
    */
   private synchronized void writePrefs (boolean add_newlines)
      throws java.io.IOException
   {
      Format f = Format.getPrettyFormat();
      f.setLineSeparator(System.getProperty("line.separator"));
      XMLOutputter outputter = new XMLOutputter(f);
      FileWriter writer = new FileWriter(mPrefsFile);
      outputter.output(mPrefsDoc, writer);
      writer.close();
   }

   // =========================================================================
   // Private data members.
   // =========================================================================

   private Vector mListeners = new Vector();

   // Preferences file and DOM tree stuff.
   private File     mPrefsFile    = null;
   private Document mPrefsDoc     = null;
   private Element  mPrefsDocRoot = null;

   // Preferences.
   private Color currentBackgroundColor = Color.white;
   private String sampleFieldText       = "Just a sample";

   // GUI elements.
   private GridLayout mMainLayout = new GridLayout();
   private JPanel mBgColorPanel = new JPanel();
   private JPanel mSampleFieldPanel = new JPanel();
   private TitledBorder mBgColorBorder;
   private TitledBorder mSampleFieldBorder;
   private JButton mEditColorButton = new JButton();
   private JLabel mCurrentColorLabel = new JLabel();
   private JColorChooser mBgColorChooser = new JColorChooser();
   private JTextField mSampleTextField = new JTextField();
   private GridBagLayout mSampleFieldPanelLayout = new GridBagLayout();
   private FlowLayout flowLayout1 = new FlowLayout();
}
