/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import info.clearthought.layout.*;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;


public abstract class ViewportCreateDialog
   extends JDialog
   implements ConfigElementListener
{
   private static int VP_ELT_COUNT = 0;

   public ViewportCreateDialog(String title, ConfigContext ctx,
                               ConfigElement elt, String elementType)
   {
      super();
      this.setTitle(title);
      this.setModal(true);
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      if ( elt == null )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition vp_def = broker.getRepository().get(elementType);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());
         elt = factory.create("ViewportCreateDialog Element " + VP_ELT_COUNT,
                              vp_def);
      }

      mViewportElement = elt;
      mViewportElement.addConfigElementListener(this);
      mBoundsPanel = new ViewportBoundsEditorPanel(elt);
      mUserPanel = new ViewportUserEditorPanel(ctx, elt);
      this.setResizable(false);

      this.getRootPane().setDefaultButton(mOkButton);
   }

   public void nameChanged(ConfigElementEvent e)
   {
   }

   public void propertyValueChanged(ConfigElementEvent e)
   {
      validateUserInput();
   }

   public void propertyValueOrderChanged(ConfigElementEvent e)
   {
   }

   public void propertyValueAdded(ConfigElementEvent e)
   {
   }

   public void propertyValueRemoved(ConfigElementEvent e)
   {
   }

   public static final int OK_OPTION     = JOptionPane.OK_OPTION;
   public static final int CANCEL_OPTION = JOptionPane.CANCEL_OPTION;
   public static final int CLOSED_OPTION = JOptionPane.CLOSED_OPTION;

   public int showDialog()
   {
      setVisible(true);
      return status;
   }

   public Rectangle getViewportBounds()
   {
      return mBoundsPanel.getViewportBounds();
   }

   public Object getUser()
   {
      return mUserPanel.getUser();
   }

   public Object getViewpoint()
   {
      return mUserPanel.getViewpoint();
   }

   protected ConfigElement mViewportElement = null;

   protected void processWindowEvent(WindowEvent e)
   {
      if (e.getID() == WindowEvent.WINDOW_CLOSING)
      {
         status = CLOSED_OPTION;
         dispose();
      }

      super.processWindowEvent(e);
   }

   protected final void validateUserInput()
   {
      boolean user_set, size_set, custom_valid;
      boolean enabled;

      Rectangle bounds = mBoundsPanel.getViewportBounds();

      user_set = (((ConfigElementPointer) mUserPanel.getUser()).getTarget() != null);
      size_set = bounds.width > 0 && bounds.height > 0;
      custom_valid = validateCustomInput();

      // All of the above conditions must be true in order for the OK button
      // to be enabled.
      enabled = user_set && size_set && custom_valid;
      mOkButton.setEnabled(enabled);
   }

   protected abstract boolean validateCustomInput();

   protected void initUI()
   {
      mButtonPanel.setLayout(mButtonPanelLayout);
      mOkButton.setEnabled(false);
      mOkButton.setText("OK");
      mOkButton.addActionListener(new ViewportCreateDialog_mOkButton_actionAdapter(this));
      mCancelButton.setText("Cancel");
      mCancelButton.addActionListener(new ViewportCreateDialog_mCancelButton_actionAdapter(this));
      mHelpButton.setEnabled(false);
      mHelpButton.setDoubleBuffered(false);
      mHelpButton.setText("Help");
      mHelpButton.addActionListener(new ViewportCreateDialog_mHelpButton_actionAdapter(this));
      this.getContentPane().add(mMainPanel, BorderLayout.CENTER);
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);

      if ( (new EnvironmentServiceProxy()).getOS() == EnvironmentService.Windows )
      {
         mButtonPanelLayout.setAlignment(FlowLayout.CENTER);
         mButtonPanel.add(mOkButton, null);
         mButtonPanel.add(mCancelButton, null);
         mButtonPanel.add(mHelpButton, null);
      }
      else
      {
         mButtonPanelLayout.setAlignment(FlowLayout.RIGHT);
         mButtonPanel.add(mHelpButton, null);
         mButtonPanel.add(mCancelButton, null);
         mButtonPanel.add(mOkButton, null);
      }
   }

   private int status = CANCEL_OPTION;

   protected JPanel mMainPanel = new JPanel();
   protected TableLayout mMainPanelLayout = null;
   protected ViewportBoundsEditorPanel mBoundsPanel = null;
   protected ViewportUserEditorPanel mUserPanel = null;

   private JPanel mButtonPanel = new JPanel();
   private FlowLayout mButtonPanelLayout = new FlowLayout();
   private JButton mOkButton = new JButton();
   private JButton mCancelButton = new JButton();
   private JButton mHelpButton = new JButton();

   void okClicked(ActionEvent e)
   {
      status = OK_OPTION;
      dispose();
   }

   void cancelClicked(ActionEvent e)
   {
      status = CANCEL_OPTION;
      dispose();
   }

   void helpClicked(ActionEvent e)
   {

   }

}

class ViewportCreateDialog_mOkButton_actionAdapter implements ActionListener
{
   private ViewportCreateDialog adaptee;

   ViewportCreateDialog_mOkButton_actionAdapter(ViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.okClicked(e);
   }
}

class ViewportCreateDialog_mCancelButton_actionAdapter
   implements ActionListener
{
   private ViewportCreateDialog adaptee;

   ViewportCreateDialog_mCancelButton_actionAdapter(ViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.cancelClicked(e);
   }
}

class ViewportCreateDialog_mHelpButton_actionAdapter
   implements ActionListener
{
   private ViewportCreateDialog adaptee;

   ViewportCreateDialog_mHelpButton_actionAdapter(ViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.helpClicked(e);
   }
}
