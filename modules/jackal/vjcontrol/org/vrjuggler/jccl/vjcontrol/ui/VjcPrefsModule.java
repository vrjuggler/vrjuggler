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
package org.vrjuggler.jccl.vjcontrol.ui;

import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.JMenuItem;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.vjcontrol.*;
import org.vrjuggler.jccl.vjcontrol.ui.widgets.EasyMenuBar;
import org.vrjuggler.jccl.editorgui.ConfigUIHelper;

/** Provides facilities for editing VjControl's preferences.
 *  This module requires a ControlUI module and a ConfigUIHelper module,
 *  and is used to edit/save/etc. VjControl.Core's gui_chunkdb.
 *
 *  @version $Revision$
 */
public class VjcPrefsModule extends DefaultCoreModule implements ActionListener
{
   protected ControlUIModule ui_module;
   protected ConfigUIHelper confighelper_module;

   protected JMenuItem editgprefs_mi;
   protected JMenuItem saveprefs_mi;



   public VjcPrefsModule ()
   {
      super ();

      ui_module = null;
      confighelper_module = null;
      component_name = "Unconfigured VjcPrefsModule";
   }


   public ConfigChunk getConfiguration ()
   {
      return component_chunk;
   }


   public void setConfiguration (ConfigChunk ch) throws VjComponentException
   {
      component_chunk = ch;
      component_name = ch.getName();

      // get pointers to the modules we need.
      VarValue prop_val = ch.getProperty(VjComponentTokens.DEPENDENCIES);

      if ( null != prop_val )
      {
         int n = ch.getPropertyCount(VjComponentTokens.DEPENDENCIES);
         String s;
         VjComponent c;

         for ( int i = 0; i < n; i++ )
         {
            s = ch.getProperty(VjComponentTokens.DEPENDENCIES, i).toString();
            c = Core.getVjComponent (s);

            if( c != null )
            {
               if( c instanceof ControlUIModule )
               {
                  ui_module = (ControlUIModule)c;
               }

               if( c instanceof ConfigUIHelper )
               {
                  confighelper_module = (ConfigUIHelper)c;
               }
            }
         }
      }
   }


   public void initialize () throws VjComponentException
   {
      if( ui_module == null || confighelper_module == null )
      {
         throw new VjComponentException (component_name + ": Initialized with unmet dependencies.");
      }

      EasyMenuBar mb = ui_module.getEasyMenuBar();
      editgprefs_mi = mb.addMenuItem ("Options/Edit Global Preferences...");
      saveprefs_mi = mb.addMenuItem ("Options/Save Preferences");

      editgprefs_mi.addActionListener (this);
      saveprefs_mi.addActionListener (this);
   }



   public void destroy ()
   {
//         EasyMenuBar mb = ui_module.getEasyMenuBar();
//         mb.removeMenuItem ("");
//         mb.removeMenuItem ("");
   }



   public void actionPerformed (ActionEvent e)
   {
      Object o = e.getSource();
      if( o == editgprefs_mi )
      {
         List v = Core.vjcontrol_chunkdb.getOfDescToken ("vjcontrol");
         if( v.size() >= 1 )
         {
            ConfigChunk chunk = (ConfigChunk)v.get(0);
            confighelper_module.openChunkFrame (Core.vjcontrol_chunkdb, chunk);
         }
      }
      else if( o == saveprefs_mi )
      {
         try
         {
            Core.vjcontrol_chunkdb.write();
//            DataOutputStream out = new DataOutputStream(new FileOutputStream(Core.vjcontrol_chunkdb.getFile()));
//            out.writeBytes(Core.vjcontrol_chunkdb.fileRep());
            Core.consoleInfoMessage(component_name,
                                    "Saved VjControl preferences: " +
                                    Core.vjcontrol_chunkdb.getInputFile());
            Core.vjcontrol_chunkdb.setUnsavedChanges(false);
            /* do some fixing up if the name changed */
         }
         catch( IOException x )
         {
            Core.consoleErrorMessage (component_name, "Error saving VjControl preferences: " + x.toString());
         }
      }
   }
}
