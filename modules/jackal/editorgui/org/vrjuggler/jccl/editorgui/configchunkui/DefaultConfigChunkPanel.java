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
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editorgui.*;
import org.vrjuggler.jccl.vjcontrol.*;
import org.vrjuggler.jccl.vjcontrol.ui.widgets.*;


/** Default implementation of ConfigChunkPanel.
 *  DO NOT INSTANTIATE THIS YOURSELF.  ConfigChunkPanels should be allocated
 *  via ConfigUIHelperModule (and its ConfigChunkPanelFactory).
 */
public class DefaultConfigChunkPanel extends JPanel
                                     implements ConfigChunkPanel, ChunkDBListener
{
   private String        mOrigChunkName = "";
   private ConfigChunk   mChunk         = null;
   private ConfigChunkDB mChunkDB       = null;

   Vector mPropertyPanels = new Vector(); /**< property description panels. */
   Box properties_panel;
   JButton cancelbutton;
   JButton okbutton;
   JButton applybutton;
   JButton helpbutton;
   JTextField namef;
   JTextField helpfield;
   JScrollPane sp;

   GridBagLayout playout;
   GridBagConstraints pconstraints;

   protected ConfigChunk component_chunk = null;
   protected String component_name = "Unconfigured DefaultConfigChunkPanel";

   ConfigUIHelper mUIHelperModule = null;

   public DefaultConfigChunkPanel ()
   {
      super();

      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }
   }

   private void jbInit () throws Exception
   {
      JPanel northpanel, centerpanel;

      //setBorder (new EmptyBorder (5, 5, 0, 5));
      setLayout (new BorderLayout (5,5));

      JPanel p1;
      northpanel = new JPanel();
      northpanel.setLayout (new BoxLayout (northpanel, BoxLayout.Y_AXIS));
      p1 = new JPanel();
      p1.setLayout (new BoxLayout (p1, BoxLayout.X_AXIS));
      p1.add (new JLabel ("Instance Name: "));
      p1.add (namef = new StringTextField ("", 25));
      northpanel.add (p1);

      helpfield = new JTextField ("");
      helpfield.setEditable (false);
      northpanel.add (helpfield);

      add(northpanel, "North");

      properties_panel = Box.createVerticalBox();

      sp = new JScrollPane(properties_panel,
                           JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                           JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
      sp.getVerticalScrollBar().setUnitIncrement(40);
      sp.getHorizontalScrollBar().setUnitIncrement(40);

      add(sp,"Center");
   }


   /* note: _chunkdb is specifically allowed to be null; this is often
    * the case w/ embedded chunks.  if chunkdb is non-null we'll listen
    * to it for changes to _chunk.  otherwise, whoever called setChunk
    * on us is responsible for keeping us up-to-date.
    */
   public void setChunk (ConfigChunk _chunk, ConfigChunkDB _chunkdb)
   {
      if ( mChunk == _chunk )
      {
         return;
      }

      mOrigChunkName = _chunk.getName();
      mChunk         = _chunk;
      mChunkDB       = _chunkdb;

      //System.out.println ("editing chunk: " + _chunk.toString());

      // double check this to make sure i'm not leaving anything
      // dangling.
      mPropertyPanels.clear();
      properties_panel.removeAll();

      if ( null != mChunk )
      {
         namef.setText(mChunk.getName());
         helpfield.setText(mChunk.getDescHelp());

         // make property panels
         PropertyPanel t;
         PropertyDesc prop_desc;
         java.util.Iterator i = mChunk.getDesc().getPropertyDescs().iterator();

         while ( i.hasNext() )
         {
            prop_desc = (PropertyDesc) i.next();

            if ( Core.user_profile.accepts(prop_desc.getUserLevel()) )
            {
               t = new PropertyPanel(prop_desc, mChunk, mUIHelperModule);
               mPropertyPanels.add(t);
               properties_panel.add(t);
            }
         }
      }

      if( mChunkDB != null )
      {
         mChunkDB.addChunkDBListener(this);
      }
   }


   public String getName ()
   {
      return mChunk.getName();
   }


   public String getComponentName ()
   {
      return mChunk.getName();
   }


   public void setComponentName (String _name)
   {
      ; // determined by our chunk...
   }


//      public void setConfigUIHelper (ConfigUIHelper h) {
//          mUIHelperModule = h;
//      }

   public void setConfiguration (ConfigChunk ch) throws VjComponentException {
      component_chunk = ch;
      component_name = ch.getName();

      // get pointers to the modules we need.
      VarValue prop_val = ch.getProperty(VjComponentTokens.DEPENDENCIES);

      if ( null != prop_val)
      {
         int n = ch.getPropertyCount(VjComponentTokens.DEPENDENCIES);
         String s;
         VjComponent c;

         for ( int i = 0; i < n; ++i )
         {
            s = ch.getProperty(VjComponentTokens.DEPENDENCIES, i).toString();
            c = Core.getVjComponent (s);
            if( c != null )
            {
               if( c instanceof ConfigUIHelper )
               {
                  mUIHelperModule = (ConfigUIHelper)c;
               }
            }
         }
      }
   }


   public void initialize () throws VjComponentException {
      if ( null == mUIHelperModule )
      {
         throw new VjComponentException (component_name + ": Initialized with unmet dependencies.");
      }
   }


   public ConfigChunk getConfiguration ()
   {
      return component_chunk;
   }


   public VjComponent addConfig (ConfigChunk ch) throws VjComponentException {
      throw new VjComponentException (component_name + " does not support child component: " + ch.getName());
   }


   public boolean removeConfig (String name)
   {
      return false;
   }


   public void destroy ()
   {
      if ( mChunkDB != null )
      {
         mChunkDB.removeChunkDBListener (this);
      }
   }


   public ConfigChunkDB getChunkDB ()
   {
      return mChunkDB;
   }

   public ConfigChunk getChunk ()
   {
      return mChunk;
   }

   // XXX: This is here so that it is possible to look up mChunk by its
   // original name.
   public String getOrigChunkName ()
   {
      return mOrigChunkName;
   }

   /**
    * Commits the changes the user made to this chunk via the GUI.
    */
   public void commit () throws ConfigUIException
   {
      mChunk.setName(namef.getText());

      int n = mPropertyPanels.size();

      for ( int i = 0; i < n; ++i )
      {
         PropertyPanel p = (PropertyPanel) mPropertyPanels.get(i);
         p.commit();
      }
   }



   public boolean matches (String cl, Object db, Object o)
   {
      try
      {
         if( cl != null )
         {
            if( !(Class.forName(cl).isInstance(this)) )
            {
               return false;
            }
         }

         if ( mChunkDB != db )
         {
            return false;
         }
         ConfigChunk ch = (ConfigChunk)o;
         return(ch == null) || (ch.getName().equals(mChunk.getName()));
      }
      catch( Exception e )
      {
         return false;
      }
   }


   public void showHelp ()
   {
      mUIHelperModule.loadDescHelp(mChunk.getDescToken());
   }


   //------------------- ChunkDBListener stuff ----------------------
   public void configChunkAdded (ChunkDBEvent e)
   {
      ;
   }
   public void configChunkRemoved (ChunkDBEvent e)
   {
      ;
   }
   public void configChunkReplaced (ChunkDBEvent e)
   {
      // basically we're checking for when an apply comes across, so we
      // can correct exactly which chunk we're supposed to be editing.
      // at some point, we should add the ability to completely update the
      // displayed state; that'll be helpful when we get more "live"
      // information from juggler.  (it'd probably be helpful at that
      // point if we can query the ConfigModule about whether this is an
      // "active db" or not - if not, then we shouldn't have to do a full
      // update).
//      if ( e.getOldChunk() == chunk )
//      {
//         chunk = e.getNewChunk();
//      }
   }
   public void configChunksCleared (ChunkDBEvent e)
   {
      ;
   }
}
