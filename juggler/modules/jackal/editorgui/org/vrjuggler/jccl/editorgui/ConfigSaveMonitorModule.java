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
package org.vrjuggler.jccl.editorgui;

import javax.swing.*;
import java.io.File;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.vjcontrol.*;
import org.vrjuggler.jccl.vjcontrol.ui.ControlUIModule;
import org.vrjuggler.jccl.vjcontrol.ui.widgets.*;

/** A module that warns if you close a config or desc file without saving.
 *  This module monitors the need_to_save flag of ConfigChunkDB files and
 *  ChunkDescDB files belonging to a ConfigModule.  If a file is closed
 *  without saving changes, it shows a dialog asking if it should be
 *  saved.
 */
public class ConfigSaveMonitorModule
   extends DefaultCoreModule
   implements /*CoreModule,*/ ConfigModuleListener
{
   ConfigModule config_module;
   ControlUIModule ui_module;

   SuffixFilter chunkdb_filter;
   SuffixFilter descdb_filter;

   /** Constructor */
   public ConfigSaveMonitorModule ()
   {
      super();
      config_module = null;
      ui_module = null;
      component_name = "Unconfigured ConfigSaveMonitorModule";
   }



   public void setConfiguration (ConfigChunk ch) throws VjComponentException
   {
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
               if ( c instanceof ControlUIModule )
               {
                  ui_module = (ControlUIModule)c;
               }

               if ( c instanceof ConfigModule )
               {
                  config_module = (ConfigModule)c;
               }
            }
         }
      }
   }


   public void initialize () throws VjComponentException
   {
      if ( (ui_module == null) || (config_module == null) )
      {
         throw new VjComponentException (component_name + ": Initialized with unmet dependencies");
      }

      chunkdb_filter = new SuffixFilter ("Config Files (*.config, *.cfg)", ".config");
      chunkdb_filter.addSuffix(".cfg");
      chunkdb_filter = (SuffixFilter)ui_module.getEasyFileDialog().addFilter (chunkdb_filter, ConfigTokens.chunk_db_TOKEN);
      descdb_filter = new SuffixFilter("Description Files (*.desc, *.dsc)", ".desc");
      descdb_filter.addSuffix(".dsc");
      descdb_filter = (SuffixFilter)ui_module.getEasyFileDialog().addFilter (descdb_filter, ConfigTokens.chunk_desc_db_TOKEN);

      config_module.addConfigModuleListener (this);
   }


//      public boolean addConfig (ConfigChunk ch) {
//          return false;
//      }


//      public boolean removeConfig (String name) {
//          return false;
//      }


   /** We're going down; do what we can to get the job done before we die.
    */
   public void destroy ()
   {
      File f;

      // look for unsaved ConfigChunkDBs
      ConfigChunkDB[] chunkdbs = config_module.getChunkDBs();
      for( int i = 0; i < chunkdbs.length; i++ )
      {
         if( !config_module.isActive(chunkdbs[i]) &&
             chunkdbs[i].hasUnsavedChanges() )
         {

            int result =
            JOptionPane.showConfirmDialog (ui_module, "Save changes to " +
                                           chunkdbs[i].getName() + "?",
                                           "Save file?",
                                           JOptionPane.YES_NO_OPTION,
                                           JOptionPane.QUESTION_MESSAGE
                                           /*, Core.save_icn*/);
            if( result == JOptionPane.YES_OPTION )
            {
               //config_module.saveChunkDBFile (chunkdbs[i]);
               f = chunkdbs[i].getInputFile();
               f = ui_module.getEasyFileDialog().requestSaveFile (f, ui_module, chunkdb_filter);

               if( f != null )
               {
                  config_module.saveChunkDBFile (chunkdbs[i], f);
               }
            }
         }
      }

      // look for unsaved ChunkDescDBs
      ChunkDescDB[] descdbs = config_module.getDescDBs();
      for( int i = 0; i < descdbs.length; i++ )
      {
         if( descdbs[i].hasUnsavedChanges() &&
             !config_module.isActive(descdbs[i]) )
         {

            int result =
               JOptionPane.showConfirmDialog(ui_module,
                                             "Save changes to " +
                                             descdbs[i].getName() + "?",
                                             "Save file?",
                                             JOptionPane.YES_NO_OPTION,
                                             JOptionPane.QUESTION_MESSAGE
                                             /*, Core.save_icn*/);

            if ( result == JOptionPane.YES_OPTION )
            {
               //config_module.saveDescDBFile (descdbs[i]);
               f = descdbs[i].getInputFile();
               f = ui_module.getEasyFileDialog().requestSaveFile (f, ui_module, descdb_filter);
               if( f != null )
               {
                  config_module.saveDescDBFile (descdbs[i], f);
               }
            }
         }
      }
   }



   //----------------------- ConfigModule Listener Stuff -------------------


   public void chunkDBAdded (ConfigModuleEvent e)
   {
      ;
   }


   public void chunkDBRemoved (ConfigModuleEvent e)
   {
      ConfigChunkDB db = e.getChunkDB();
      if( db.hasUnsavedChanges()  && !config_module.isActive(db) )
      {
         int result =
            JOptionPane.showConfirmDialog(ui_module, "Save changes to " +
                                          db.getName() + "?", "Save file?",
                                          JOptionPane.YES_NO_OPTION,
                                          JOptionPane.QUESTION_MESSAGE
                                          /*, Core.save_icn*/);
         if( result == JOptionPane.YES_OPTION )
         {
            File f = db.getInputFile();
            f = ui_module.getEasyFileDialog().requestSaveFile (f, ui_module, chunkdb_filter);
            if( f != null )
            {
               config_module.saveChunkDBFile (db, f);
            }
         }
      }
   }


   public void descDBAdded (ConfigModuleEvent e)
   {
      ;
   }


   public void descDBRemoved (ConfigModuleEvent e)
   {
      ChunkDescDB db = e.getDescDB();
      if( db.hasUnsavedChanges() && !config_module.isActive(db) )
      {
         int result =
            JOptionPane.showConfirmDialog(ui_module,
                                          "Save changes to " + db.getName() + "?",
                                          "Save file?",
                                          JOptionPane.YES_NO_OPTION,
                                          JOptionPane.QUESTION_MESSAGE
                                          /*, Core.save_icn*/);

         if( result == JOptionPane.YES_OPTION )
         {
            File f = db.getInputFile();
            f = ui_module.getEasyFileDialog().requestSaveFile (f, ui_module, descdb_filter);

            if( f != null )
            {
               config_module.saveDescDBFile (db, f);
            }
         }
      }
   }
}
