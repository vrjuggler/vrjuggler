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
