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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.*;
import java.awt.event.*;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import javax.swing.*;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;
import info.clearthought.layout.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.ConfigBrokerProxy;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.ConfigElementFactory;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;

public class ViewEditorPanel
   extends JPanel
{
   private static int VP_ELT_COUNT = 0;
   private CaveModel mCaveModel = null;

   public ViewEditorPanel(CaveModel cm, Dimension resolution, String title)
      throws HeadlessException
   {
      mResolution = resolution;
      mCaveModel = cm;

      try
      {
         jbInit();
         mScreenModel = new ScreenComboBoxModel( cm );
         mScreenRenderer = new ScreenComboBoxRenderer( cm );
         mScreenCB.setModel( mScreenModel );
         mScreenCB.setRenderer( mScreenRenderer );
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
      
      mMainBorder =
         new TitledBorder(BorderFactory.createEtchedBorder(Color.white,
                                                           new Color(161, 161, 161)),
                                                           title);
      this.setBorder(mMainBorder);
   }
   
   public void updatePosSize(PropertyChangeEvent evt)
   {
      ConfigElement screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mView);
      int val = Integer.parseInt(evt.getNewValue().toString());
      int screen_size_x  = ((Integer) screen.getProperty("size", 0)).intValue();
      int screen_size_y  = ((Integer) screen.getProperty("size", 1)).intValue();
         
      if (evt.getSource() == mPositionXField)
      {
         float pos_x = (float)val/(float)screen_size_x;
         mView.setProperty("origin", 0, new Float(pos_x));
      }
      else if (evt.getSource() == mPositionYField)
      {
         float pos_y = (float)val/(float)screen_size_y;
         mView.setProperty("origin", 1, new Float(pos_y));
      }
      else if (evt.getSource() == mWidthField)
      {
         float size_x = (float)val/(float)screen_size_x;
         mView.setProperty("size", 0, new Float(size_x));
      }
      else if (evt.getSource() == mHeightField)
      {
         float size_y = (float)val/(float)screen_size_y;
         mView.setProperty("size", 1, new Float(size_y));
      }
   }

   class ChangeListener implements PropertyChangeListener
   {
      public void propertyChange(PropertyChangeEvent evt) 
      {
         if (evt.getPropertyName().equals("value"))
         {
            updatePosSize( evt );
         }
      }
   }

   private ChangeListener mChangeListener = new ChangeListener();
   
   public void setConfig(ConfigElement view)
   {
      mView = view;
      ConfigElement screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mView);
      
      loadPosSizeFromElement();
      mScreenModel.setSelectedScreen( screen );


      mPositionXField.addPropertyChangeListener(mChangeListener);
      mPositionYField.addPropertyChangeListener(mChangeListener);
      mWidthField.addPropertyChangeListener(mChangeListener);
      mHeightField.addPropertyChangeListener(mChangeListener);
      
      // Validate the default values for the various text fields.
      validateUserInput();

      mView.addConfigElementListener( new ConfigElementAdapter()
         {
            public void propertyValueChanged(ConfigElementEvent event)
            {
               if ( event.getProperty().equals("size") ||
                    event.getProperty().equals("origin") )
               {
                  loadPosSizeFromElement();
               }
            }
         });
   }

   private void loadPosSizeFromElement()
   {
      ConfigElement screen = (ConfigElement)mCaveModel.getViewToScreenMap().get(mView);

      float v_origin_x  = ((Number) mView.getProperty("origin", 0)).floatValue();
      float v_origin_y  = ((Number) mView.getProperty("origin", 1)).floatValue();
      float v_size_x  = ((Number) mView.getProperty("size", 0)).floatValue();
      float v_size_y  = ((Number) mView.getProperty("size", 1)).floatValue();
      
      int s_origin_x  = ((Integer) screen.getProperty("origin", 0)).intValue();
      int s_origin_y  = ((Integer) screen.getProperty("origin", 1)).intValue();
      int s_size_x  = ((Integer) screen.getProperty("size", 0)).intValue();
      int s_size_y  = ((Integer) screen.getProperty("size", 1)).intValue();
         
      mPositionXField.setValue( new Integer((int)(v_origin_x * s_origin_x)) );
      mPositionYField.setValue( new Integer((int)(v_origin_y * s_origin_y)) );
      mWidthField.setValue( new Integer((int)(v_size_x * s_size_x)) );
      mHeightField.setValue( new Integer((int)(v_size_y * s_size_y)) );
   }
   
   public Object getViewpoint()
   {
      return mView.getProperty("view", 0);
   }

   public Object getUser()
   {
      return mView.getProperty("user", 0);
   }
   
   public Rectangle getDisplayWindowBounds()
   {
      int x = ((Integer) mPositionXField.getValue()).intValue();
      int y = ((Integer) mPositionYField.getValue()).intValue();
      int width = ((Integer) mWidthField.getValue()).intValue();
      int height = ((Integer) mHeightField.getValue()).intValue();
      return new Rectangle(x, y, width, height);
   }

   private void jbInit() throws Exception
   {
      double main_size[][] =
         {{TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.PREFERRED},
          {TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.PREFERRED}};
      mMainLayout = new TableLayout(main_size);


      mScreenCB.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mScreenCB_selectionChanged(e);
         }
      });

      
      this.setLayout(mMainLayout);
      mSizeLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mSizeLabel.setLabelFor(mSizePanel);
      mSizeLabel.setText("Size");
      mPositionLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mPositionLabel.setLabelFor(mPositionPanel);
      mPositionLabel.setText("Position");
      mWidthField.setMinimumSize(new Dimension(40, 21));
      mWidthField.setPreferredSize(new Dimension(40, 21));
      mWidthField.setValue(new Integer(200));
      mWidthField.setHorizontalAlignment(SwingConstants.TRAILING);
      mSizeXLabel.setText("\u00D7");
      mHeightField.setMinimumSize(new Dimension(40, 21));
      mHeightField.setPreferredSize(new Dimension(40, 21));
      mHeightField.setValue(new Integer(200));
      mPositionXField.setMinimumSize(new Dimension(40, 21));
      mPositionXField.setPreferredSize(new Dimension(40, 21));
      mPositionXField.setValue(new Integer(0));
      mPositionXField.setHorizontalAlignment(SwingConstants.TRAILING);
      mPositionXLabel.setText(",");
      mPositionYField.setMinimumSize(new Dimension(40, 21));
      mPositionYField.setPreferredSize(new Dimension(40, 21));
      mPositionYField.setValue(new Integer(0));
      mPositionYField.setHorizontalAlignment(SwingConstants.TRAILING);
      
      this.add(mSizeLabel, new TableLayoutConstraints(0, 0, 0, 0,
                                                            TableLayout.RIGHT,
                                                            TableLayout.CENTER));
      this.add(mSizePanel, new TableLayoutConstraints(1, 0, 1, 0,
                                                            TableLayout.LEFT,
                                                            TableLayout.CENTER));
      this.add(mPositionLabel,
                       new TableLayoutConstraints(0, 1, 0, 1,
                                                  TableLayout.RIGHT,
                                                  TableLayout.CENTER));
      this.add(mPositionPanel,
                       new TableLayoutConstraints(1, 1, 1, 1,
                                                  TableLayout.LEFT,
                                                  TableLayout.CENTER));
      this.add(mScreenCB, new TableLayoutConstraints(0, 2, 1, 2,
                                         TableLayout.FULL,
                                         TableLayout.FULL));
      mPositionPanel.add(mPositionXField, null);
      mPositionPanel.add(mPositionXLabel, null);
      mPositionPanel.add(mPositionYField, null);

      mSizePanel.add(mWidthField, null);
      mSizePanel.add(mSizeXLabel, null);
      mSizePanel.add(mHeightField, null);
   }

   private ConfigElement mView = null;
   private Dimension mResolution = null;

   private TableLayout mMainLayout = null;
   
   private TitledBorder mMainBorder;
   
   private JLabel mSizeLabel = new JLabel();
   private JPanel mSizePanel = new JPanel();
   private JLabel mPositionLabel = new JLabel();
   private JPanel mPositionPanel = new JPanel();
   private JFormattedTextField mWidthField = new JFormattedTextField();
   private JLabel mSizeXLabel = new JLabel();
   private JFormattedTextField mHeightField = new JFormattedTextField();
   private JFormattedTextField mPositionXField = new JFormattedTextField();
   private JLabel mPositionXLabel = new JLabel();
   private JFormattedTextField mPositionYField = new JFormattedTextField();
   private ScreenComboBoxModel mScreenModel = null;
   private ScreenComboBoxRenderer mScreenRenderer = null;
   private JComboBox mScreenCB = new JComboBox();

   public void setEnabled(boolean enabled)
   {
      mSizeLabel.setEnabled(enabled);
      mPositionLabel.setEnabled(enabled);
      mWidthField.setEnabled(enabled);
      mSizeXLabel.setEnabled(enabled);
      mHeightField.setEnabled(enabled);
      mPositionXField.setEnabled(enabled);
      mPositionYField.setEnabled(enabled);
      mScreenCB.setEnabled(enabled);
      //mMainBorder.setEnabled(enabled);
   }

   public void setTitle(String title)
   {
      mMainBorder.setTitle(title);
   }
         
   void mScreenCB_selectionChanged(ActionEvent e)
   {
      JComboBox cb = (JComboBox)e.getSource();
      ConfigElement new_screen = mScreenModel.getSelectedScreen();

      mCaveModel.changeScreenForView(mView, new_screen);
   }
   
   private void validateUserInput()
   {
      boolean width_valid, height_valid, x_pos_valid, y_pos_valid;

      // The name cannot be an empty string.
      //width_valid  = ((Integer) mWidthField.getValue()).intValue() > 0;
      //height_valid = ((Integer) mHeightField.getValue()).intValue() > 0;
      //x_pos_valid  = ((Integer) mPositionXField.getValue()).intValue() >= 0;
      //y_pos_valid  = ((Integer) mPositionYField.getValue()).intValue() >= 0;
      
      /*
      if ( width_valid && height_valid && x_pos_valid &&
           y_pos_valid )
      {
         validateWindowBounds();
      }
      else
      {
      }
      */
   }

   /**
    * Ensures that the bounds of the display window are within the managed
    * area of the desktop.  If this is not the case, user confirmation is
    * requested.
    *
    * @param e DocumentEvent
    */
   private void validateWindowBounds(/*DocumentEvent e*/)
   {
      /*
      int window_width  = Integer.parseInt(mWidthField.getText());
      int window_height = Integer.parseInt(mHeightField.getText());
      int window_pos_x  = Integer.parseInt(mPositionXField.getText());
      int window_pos_y  = Integer.parseInt(mPositionYField.getText());

      if ( window_pos_x < 0 || window_pos_y < 0 ||
           window_width + window_pos_x > mResolution.width ||
           window_height + window_pos_y > mResolution.height )
      {
         int answer =
            JOptionPane.showConfirmDialog(this,
                                          "Do you want the window to open outside the managed area?",
                                          "Display Window Outside Managed Area",
                                          JOptionPane.YES_NO_OPTION,
                                          JOptionPane.WARNING_MESSAGE);

         if ( answer == JOptionPane.YES_OPTION )
         {
         }
         else
         {
         }
      }
      */
   }

   class ScreenComboBoxRenderer extends JLabel implements ListCellRenderer
   {
      private CaveModel mCaveModel = null;
      
      public ScreenComboBoxRenderer(CaveModel cm)
      {
         mCaveModel = cm;

         setOpaque(true);
         setHorizontalAlignment(CENTER);
         setVerticalAlignment(CENTER);
      }

      public Component getListCellRendererComponent( JList list,
                                                     Object value,
                                                     int index,
                                                     boolean isSelected,
                                                     boolean cellHasFocus)
      {
         if (isSelected)
         {
            setBackground(list.getSelectionBackground());
            setForeground(list.getSelectionForeground());
         }
         else
         {
            setBackground(list.getBackground());
            setForeground(list.getForeground());
         }
         
         ConfigElement screen = (ConfigElement)value;
         ConfigElement node = (ConfigElement)mCaveModel.getScreenToNodeMap().get( screen );

         if (null != node)
         {
            setText( node.getName() + ":" + screen.getName() );
         }
         else
         {
            setText( "local:" + screen.getName() );
         }
            
         return this;
      }
   }
   
   public class ScreenComboBoxModel
      extends AbstractListModel
      implements ComboBoxModel//, ConfigListener, ConfigElementListener
   {
      private CaveModel mCaveModel = null;
      private Object mSelectedObject;
       
      public void setSelectedItem(Object obj)
      {
         if ((mSelectedObject != null && !mSelectedObject.equals( obj )) ||
              mSelectedObject == null && obj != null)
         {
            mSelectedObject = obj;
            fireContentsChanged(this, -1, -1);
         }
      }

      public Object getSelectedItem()
      {
         return mSelectedObject;
      }
      
      public Object getElementAt(int index)
      {
         if ( index >= 0 && index < mCaveModel.getScreens().size() )
         {
            return mCaveModel.getScreens().elementAt(index);
         }
         else
         {
            return null;
         }
      }

      public int getSize()
      {
         return mCaveModel.getScreens().size();
      }
      
      public int getIndexOf(Object obj)
      {
         return mCaveModel.getScreens().indexOf( obj );
      }
      
      public void addElement(Object obj)
      {
         mCaveModel.getScreens().addElement( obj );
         fireIntervalAdded( this, mCaveModel.getScreens().size() - 1, mCaveModel.getScreens().size() - 1 );
         if ( mCaveModel.getScreens().size() == 1 && mSelectedObject == null && obj != null )
         {
            setSelectedItem( obj );
         }
      }

      public ScreenComboBoxModel( CaveModel cm )
      {
         mCaveModel = cm;
      }

      public ConfigElement getSelectedScreen()
      {
         Object obj = getSelectedItem();
         if (null != obj)
         {
            return( (ConfigElement)obj );
         }
         else
         {
            return null;
         }
      }
      
      public void setSelectedScreen( ConfigElement elm )
      {
         mSelectedObject = elm;
      }

      /*
      
      public void configElementAdded(ConfigEvent evt)
      {
         if (null != mConfigContext && mConfigContext.contains(evt.getResource()))
         {
            ConfigElement elm = evt.getElement();
            screenAdded( elm );
         
            if ( elm.getDefinition().getToken().equals(EditorConstants.cluster_node_type) )
            {
               elm.addConfigElementListener(this);
            }
         }
      }

      public void configElementRemoved(ConfigEvent evt)
      {
         if (null != mConfigContext && mConfigContext.contains(evt.getResource()))
         {
            ConfigElement elm = evt.getElement();
            screenRemoved( elm );

            if ( elm.getDefinition().getToken().equals(EditorConstants.cluster_node_type) )
            {
               elm.removeConfigElementListener(this);
            }
         }
      }

      public void nameChanged(ConfigElementEvent evt)
      {;}
      
      public void propertyValueChanged(ConfigElementEvent evt)
      {;}
      
      public void propertyValueAdded(ConfigElementEvent evt)
      {
         if ( evt.getProperty().equals( EditorConstants.display_window_type ) )
         {
            ConfigElement elm = (ConfigElement)evt.getValue();
            screenAdded( elm );
         }
      }
      
      public void propertyValueRemoved(ConfigElementEvent evt)
      {
         if ( evt.getProperty().equals( EditorConstants.display_window_type ) )
         {
            ConfigElement elm = (ConfigElement)evt.getValue();
            screenRemoved( elm );
         }
      }
      
      public void screenAdded(ConfigElement elm)
      {
         //System.out.println(elm.getDefinition().getName());
         if ( elm.getDefinition().getToken().equals(EditorConstants.display_window_type) )
         {
            addElement(elm));
         }
      }
     
      public void screenRemoved(ConfigElement elm)
      {
         if ( elm.getDefinition().getToken().equals(EditorConstants.display_window_type) )
         {
            mCaveModel.getScreens().removeElement(elm);
         }
      }
   */
   }
}
