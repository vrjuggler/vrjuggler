/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileFilter;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.event.EventListenerList;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;


/**
 * A Swing component that can be used for editors that need to provide users
 * with a list of serial ports.  The list of serial ports is composed of the
 * locally available serial ports (determined using platform-specific
 * conventions) and a list of "template" serial port names.  The list can be
 * modified by the user to allow run-time extension as necessary.
 */
public class SerialPortChooser
   extends JPanel
{
   public SerialPortChooser()
   {
      super();

      mPortComboBox = new JComboBox();
      mPortComboBox.setEditable(true);

      // Build up the two arrays of String objects that will be used to
      // populate mPortComboBox.
      String[] local_ports = getLocalPorts();
      String[] base_ports = { "/dev/ttySX", "/dev/ttydX", "/dev/ttyX", "COMX" };

      // Insert the locally available ports (if any) first.
      for ( int i = 0; i < local_ports.length; ++i )
      {
         mPortComboBox.addItem(local_ports[i]);
      }

      // Then insert the "template" port names at the end.
      for ( int i = 0; i < base_ports.length; ++i )
      {
         mPortComboBox.addItem(base_ports[i]);
      }

      mPortComboBox.addActionListener(
         new SerialPortChooser_chooserActionAdapter(this)
      );

      this.setLayout(new BorderLayout());
      this.add(mPortComboBox, BorderLayout.CENTER);
   }

   public void addActionListener(ActionListener l)
   {
      mListeners.add(ActionListener.class, l);
   }

   public void removeActionListener(ActionListener l)
   {
      mListeners.remove(ActionListener.class, l);
   }

   public void setPort(String portName)
   {
      mPortComboBox.setSelectedItem(portName);
   }

   public String getPort()
   {
      return (String) mPortComboBox.getSelectedItem();
   }

   /**
    * Finds the locally available serial port device names using
    * platform-specific rules.
    */
   private String[] getLocalPorts()
   {
      String os_name = System.getProperty("os.name");
      String[] ports;
      EnvironmentService env_svc = new EnvironmentServiceProxy();

      if ( env_svc.getOS() == EnvironmentService.Windows )
      {
         ports = new String[2];
         ports[0] = "COM1";
         ports[1] = "COM2";
      }
      else
      {
         File dev_dir = new File("/dev");
         FileFilter f = null;

         if ( os_name.toLowerCase().startsWith("mac os x") ||
              os_name.toLowerCase().startsWith("freebsd") ||
              os_name.toLowerCase().startsWith("irix") )
         {
            f = new MatchFilter("ttyd\\d+");
         }
         else if ( os_name.toLowerCase().startsWith("linux") )
         {
            f = new MatchFilter("ttyS\\d+");
         }
         else if ( os_name.toLowerCase().startsWith("solaris") )
         {
            f = new MatchFilter("tty[a-z]");
         }

         File[] port_devs = dev_dir.listFiles(f);

         ports = new String[port_devs.length];

         for ( int i = 0; i < port_devs.length; ++i )
         {
            ports[i] = port_devs[i].getAbsolutePath();
         }
      }

      return ports;
   }

   private EventListenerList mListeners = new EventListenerList();

   private JComboBox mPortComboBox;

   /**
    * A simple java.io.FileFilter implementation that uses regular expressions
    * to filter java.io.File objects.
    */
   public static class MatchFilter
      implements FileFilter
   {
      /**
       * Constructs a new MatchFilter that will use the given regular
       * expression for matching against java.io.File objects.
       *
       * @param devRegExp The regular expression that will be used for matching
       *                  against java.io.File objects.
       *
       * @see #accept(java.io.File)
       */
      public MatchFilter(String devRegExp)
      {
         this.devRegExp = devRegExp;
      }

      /**
       * Performs a regular expression match using the String returned by
       * calling the getPath() object on the given File object.
       */
      public boolean accept(File pathname)
      {
         return pathname.getPath().matches(devRegExp);
      }

      private String devRegExp;
   }

   protected void fireActionEvent(ActionEvent origActionEvent)
   {
      ActionEvent evt = null;
      Object[] listeners = mListeners.getListenerList();

      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == ActionListener.class )
         {
            if ( evt == null )
            {
               evt = new ActionEvent(this, origActionEvent.getID(),
                                     origActionEvent.getActionCommand());
            }

            ((ActionListener) listeners[i + 1]).actionPerformed(evt);
         }
      }
   }

   void chooserActionPerformed(ActionEvent actionEvent)
   {
      String port = (String) mPortComboBox.getSelectedItem();

      boolean found = false;

      for ( int i = 0; i < mPortComboBox.getItemCount(); ++i )
      {
         if ( mPortComboBox.getItemAt(i).equals(port) )
         {
            found = true;
            break;
         }
      }

      if ( ! found )
      {
         mPortComboBox.addItem(port);
      }

      fireActionEvent(actionEvent);
   }
}

class SerialPortChooser_chooserActionAdapter
   implements ActionListener
{
   private SerialPortChooser adaptee;
   SerialPortChooser_chooserActionAdapter(SerialPortChooser adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.chooserActionPerformed(actionEvent);
   }
}
