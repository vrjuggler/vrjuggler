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
import javax.swing.tree.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import java.io.File;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.vjcontrol.*;
import org.vrjuggler.jccl.vjcontrol.ui.*;
import org.vrjuggler.jccl.vjcontrol.ui.widgets.*;

/** An individual subpanel of ConfigurePane.
 *  ChunkDBPanel views and edits a single ChunkDB (at a time).  It has
 *  methods to copy chunks to another ChunkDBPanel.
 *
 *  @version $Revision$
 */
public class ChunkDBPanel
   extends JPanel
   implements PlugPanel,
              ActionListener,
              MouseListener,
              ConfigModuleListener
{
   protected ConfigModule mConfigModule;
   protected ConfigUIHelper mConfigHelperModule;
   protected ControlUIModule mUIModule;
   protected String mComponentName;
   protected ConfigChunk mComponentChunk;

   protected SuffixFilter mChunkDBFilter;

   protected int mControlsOnSide; // 0 for left, 1 for right;
   public static final int CONTROLS_ON_RIGHT = 0;
   public static final int CONTROLS_ON_LEFT = 1;

   protected ChunkDBTreeModel mTreeModel;
   protected ConfigChunkDB mCurrentDB;
   protected ChunkDBPanel mSendTarget; // target for the >>, >all> buttons

   protected JButton mLoadButton;
   protected JButton mSaveButton;
   protected JButton mInsertButton;
   protected JButton mRemoveButton;
   protected JButton mSendButton;
   protected JButton mSendAllButton;
   protected JButton mNewButton;
   protected JButton mCloseButton;
   protected JButton mDuplicateButton;
   protected JButton mChunkHelpButton;
   protected JButton mCheckDependenciesButton;
   protected JComboBox mSelectDBBox;
   protected JComboBox mInsertTypeBox;
   protected JScrollPane mScrollPane;
   protected Font mCurrentFont;  // used to make sure when we add a new tree
                                // that it's using the correct font.

   protected TreePath mTreeItemMenuPath;
   protected JPopupMenu mDescTreeItemMenu;
   protected JPopupMenu mChunkTreeItemMenu;
   protected JMenuItem mHelp1MI, mHelp2MI, mRemoveMI, mInsertMI;

   protected boolean mUiInitialized;

   public ChunkDBPanel ()
   {
      super();

      mComponentName = "Unconfigured ChunkDBPanel";
      mComponentChunk = null;

      mConfigModule = null;
      mUIModule = null;
      mConfigHelperModule = null;

      mUiInitialized = false;

      mCurrentFont = null;
      mCurrentDB = null;
      mTreeModel = null;
      mSendTarget = null;

      mControlsOnSide = CONTROLS_ON_RIGHT;
   }

   public void updateUI ()
   {
      super.updateUI();
      if (mUiInitialized)
      {
         if (mTreeModel != null)
            mTreeModel.updateUI();
      }
   }

   public void setControlsOnSide (int _mControlsOnSide)
   {
      mControlsOnSide = _mControlsOnSide;
   }

   // sets the target for >> and >all> buttons
   public void setSendAcrossTarget (ChunkDBPanel _target)
   {
      mSendTarget = _target;
   }

   public void setFont (Font f)
   {
      super.setFont(f);
      mCurrentFont = f;
   }

   protected void buildDBList ()
   {
      int i;
      mSelectDBBox.addItem ("No Selection");
      String[] names = mConfigModule.getChunkDBNames();
      for (i = 0; i < names.length; i++)
      {
         mSelectDBBox.addItem (names[i]);
      }
   }

   public void selectDB (String name)
   {
      /* changes the currently displayed db in the panel to name... */
      ConfigChunkDB newChunkDB = mConfigModule.getChunkDB (name);
      if (newChunkDB == mCurrentDB)
      {
         return;
      }
      mCurrentDB = newChunkDB;

      if (mTreeModel != null)
      {
         mTreeModel.tree.removeMouseListener (this);
         mConfigHelperModule.releaseChunkDBTreeModel (mTreeModel);
      }

      mTreeModel = mConfigHelperModule.getChunkDBTreeModel (mCurrentDB);
      if (mCurrentFont != null)
      {
         mTreeModel.tree.setFont(mCurrentFont);
      }
      mScrollPane.setViewportView (mTreeModel.tree);
      mTreeModel.tree.addMouseListener(this);
      mSelectDBBox.setSelectedItem (mTreeModel.getName());
      validate();
      mScrollPane.repaint();

      if (mTreeModel == null)
      {
         System.out.println ("mTreeModel is null & shouldn't be");
      }
      if (mTreeModel.tree == null)
      {
         System.out.println ("mTreeModel tree is null & shouldn't be");
      }

      setButtonsEnabled (mCurrentDB != null);

   }

   protected String getDBName()
   {
      return (mCurrentDB != null)? mCurrentDB.getName(): null;
   }

   // typically, one ChunkDBpanel will call this on another to perform a
   // sendacross action
   protected void addAll (ConfigChunkDB newdb)
   {
      if (mCurrentDB == null)
      {
         return;
      }
      else
      {
         mConfigModule.addChunks (mCurrentDB, newdb);
      }
   }

   // updates the list of chunk types which can be inserted.
   protected void updateInsertTypes ()
   {
      int i, j;
      if (mInsertTypeBox == null)
      {
         return;
      }
      // as of swing 1.2, there seems to be a bug in removeAllItems...
      //mInsertTypeBox.removeAllItems();
      j = mInsertTypeBox.getItemCount();
      for (i = 0; i < j; i++)
      {
         mInsertTypeBox.removeItemAt (0);
      }

      String[] desc_names = mConfigModule.getDescNames();
      for (i = 0; i < desc_names.length; i++)
      {
         mInsertTypeBox.addItem (desc_names[i]);
      }
   }

   // enables/disables some of the side panel buttons. Used when there
   // is no loaded db.
   protected void setButtonsEnabled (boolean b)
   {
      mSaveButton.setEnabled(b);
      mInsertButton.setEnabled(b);
      mRemoveButton.setEnabled(b);
      mSendButton.setEnabled(b);
      mSendAllButton.setEnabled(b);
      mCloseButton.setEnabled(b);
      mDuplicateButton.setEnabled(b);
      mCheckDependenciesButton.setEnabled(b);
   }

   /******************** ActionListener Stuff ***********************/

   // public, as the saying goes, as an implementation detail
   public void actionPerformed (ActionEvent e)
   {
      int i,j;
      String s;
      ChunkTreeNodeInfo ni;
      TreePath[] tp;
      ConfigChunkDB v;
      Object source = e.getSource();
      ConfigChunk ch;

      if (source == mSendButton)
      {
         tp = mTreeModel.tree.getSelectionPaths();
         if (tp == null)
            return;
         v = new ConfigChunkDB();
         if (tp != null)
         {
            for (i = 0; i < tp.length; i++)
            {
               ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
               if (ni.isChunkNode())
               {
                  v.add(ni.getChunk());
               }
            }
         }
         if (mSendTarget != null)
         {
            mSendTarget.addAll (v);
         }
      }
      else if (source == mSendAllButton)
      {
         if (mSendTarget != null)
         {
            mSendTarget.addAll (mCurrentDB);
         }
      }
      else if (source == mNewButton)
      {
         selectDB (mConfigModule.addChunkDB (new ConfigChunkDB()));
      }
      else if (source == mCloseButton)
      {
         mConfigModule.closeChunkDB (mCurrentDB);
      }
      else if (source == mCheckDependenciesButton)
      {
         java.util.List vec = mCurrentDB.getDependencies();
         for (i = 0; i < mConfigModule.chunkdbs.size(); i++)
         {
            ConfigChunkDB db = (ConfigChunkDB)mConfigModule.chunkdbs.elementAt(i);
            if (db != mCurrentDB)
            {
               db.searchDependencies (vec);
            }
         }
         DependencyFrame dependency_frame = (DependencyFrame)mUIModule.getChildFrameMatching ("org.vrjuggler.jccl.editorgui.DependencyFrame", mCurrentDB, null);
         if (dependency_frame != null)
         {
            dependency_frame.refreshData (vec);
         }
         else
         {
            dependency_frame = new DependencyFrame(this, mCurrentDB, vec);
            mUIModule.addChildFrame (dependency_frame);
         }
         dependency_frame.show();
      }
      else if (source == mRemoveButton)
      {
         tp = mTreeModel.tree.getSelectionPaths();
         if (tp == null)
         {
            return;
         }
         ConfigChunkDB db = new ConfigChunkDB ();
         for (i = 0; i < tp.length; i++)
         {
            ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
            if (ni.isChunkNode())
            {
               db.add(ni.getChunk());
            }
         }
         mConfigModule.removeChunks (mCurrentDB, db);
      }
      else if (source == mInsertButton)
      {
         ch = ChunkFactory.createChunkWithDescName ((String)mInsertTypeBox.getSelectedItem());
         if (ch != null)
         {
            ch.setName(mCurrentDB.getNewName(ch.getDescName()));
            mCurrentDB.add(ch);
         }
      }
      else if (source == mChunkHelpButton)
      {
         String token = mConfigModule.getDescTokenFromName ((String)mInsertTypeBox.getSelectedItem());
         mConfigHelperModule.loadDescHelp (token);
      }
      else if (source == mDuplicateButton)
      {
         tp = mTreeModel.tree.getSelectionPaths();
         if (tp == null)
            return;
         for (i = 0; i < tp.length; i++)
         {
            ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
            if (ni.isChunkNode())
            {
               // create a copy of this node...
               ch = new ConfigChunk (ni.getChunk());
               ch.setName (mCurrentDB.getNewName("copy of " + ch.getName()));
               mCurrentDB.add(ch);
            }
         }
      }
      else if (source == mLoadButton)
      {
         File default_dir = (mCurrentDB == null)?null:mCurrentDB.getInputFile().getParentFile();
         File f = mUIModule.getEasyFileDialog().requestOpenFile (default_dir, mUIModule, mChunkDBFilter);
         if (f != null)
         {
            s = mConfigModule.loadNewChunkDBFile(f);
            if (s != null)
            {
               selectDB(s);
            }
         }
      }
      else if (source == mSaveButton)
      {
         if (mCurrentDB != null)
         {
            File f = mCurrentDB.getInputFile();
            f = mUIModule.getEasyFileDialog().requestSaveFile (f, mUIModule, mChunkDBFilter);
            if (f != null)
            {
               s = mConfigModule.saveChunkDBFile (mCurrentDB, f);
               selectDB(s);
            }
         }
      }
      else if (source == mSelectDBBox)
      {
         selectDB ((String)mSelectDBBox.getSelectedItem());
      }
      else if (source == mHelp1MI || source == mHelp2MI)
      {
         ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)mTreeItemMenuPath.getLastPathComponent()).getUserObject());
         if (ni.isDescNode())
         {
            mConfigHelperModule.loadDescHelp (mConfigModule.getDescTokenFromName (ni.toString()));
         }
         else if (ni.isChunkNode())
         {
            mConfigHelperModule.loadDescHelp (ni.getChunk().getDescToken());
         }
      }
      else if (source == mRemoveMI)
      {
         ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)mTreeItemMenuPath.getLastPathComponent()).getUserObject());
         if (ni.isChunkNode())
         {
            mConfigModule.removeChunk (mCurrentDB, ni.getChunk());
         }
      }
      else if (source == mInsertMI)
      {
         ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)mTreeItemMenuPath.getLastPathComponent()).getUserObject());
         if (ni.isDescNode())
         {
            ch = ChunkFactory.createChunkWithDescName (ni.toString());
            if (ch != null)
            {
               ch.setName (mCurrentDB.getNewName(ni.toString()));
               mCurrentDB.add(ch);
            }
            else
            {
               Core.consoleErrorMessage (mComponentName, "Failed to create chunk: Can't find ChunkDesc for '" + ni.toString() + "'.");
            }
         }
         mTreeModel.tree.expandPath(mTreeItemMenuPath);
      }
   }

   public void mouseClicked(MouseEvent e)
   {
      if (mTreeModel == null)
      {
         return;
      }

      int selRow = mTreeModel.tree.getRowForLocation(e.getX(), e.getY());
      if (selRow == -1)
      {
         return;
      }
      mTreeItemMenuPath = mTreeModel.tree.getPathForLocation(e.getX(), e.getY());
      ChunkTreeNodeInfo ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)mTreeItemMenuPath.getLastPathComponent()).getUserObject());

      int mod = e.getModifiers();
      if (e.getClickCount() == 1)
      {
         if (ni.isChunkNode())
         {
            ConfigChunk ch = ni.getChunk();
            String h = ch.getDescHelp();
            if (h.equals(""))
            {
               h = "No help available";
            }
            Core.consoleTempMessage (ch.getDescName(), h);
         }
         else if (ni.isDescNode())
         {
            ChunkDesc d = Core.descdb.getByName (ni.toString());
            String h = ((d == null) || d.getHelp().equals(""))?"No help available":d.getHelp();
            String n = (d == null)?"Undefined ChunkDesc":d.getName();
            Core.consoleTempMessage (n, h);
         }
      }
      else if ((mod == MouseEvent.BUTTON1_MASK) &&
               (e.getClickCount() == 2))
      {
         openChunkFrame (ni.getChunk());
      }
   }

   protected void openChunkFrame (String name)
   {
      ConfigChunk ch = mCurrentDB.get (name);
      openChunkFrame (ch);
   }

   protected void openChunkFrame (ConfigChunk ch)
   {
      if (ch != null)
      {
         mConfigHelperModule.openChunkFrame (mCurrentDB, ch);
      }
   }

   /******************* MouseListener Stuff ************************/

   public void mouseEntered(MouseEvent e) {}
   public void mouseReleased(MouseEvent e) {}
   public void mouseExited(MouseEvent e) {}

   public void mousePressed(MouseEvent e)
   {
      if (mTreeModel == null)
      {
         return;
      }

      int selRow = mTreeModel.tree.getRowForLocation(e.getX(), e.getY());
      if (selRow == -1)
      {
         return;
      }
      mTreeItemMenuPath = mTreeModel.tree.getPathForLocation(e.getX(), e.getY());
      ChunkTreeNodeInfo ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)mTreeItemMenuPath.getLastPathComponent()).getUserObject());

      int mod = e.getModifiers();
      if ((mod == MouseEvent.BUTTON2_MASK) || (mod == MouseEvent.BUTTON3_MASK))
      {
         if (mTreeItemMenuPath != null)
         {
            DefaultMutableTreeNode n = (DefaultMutableTreeNode)mTreeItemMenuPath.getLastPathComponent();
            ChunkTreeNodeInfo info = (ChunkTreeNodeInfo)n.getUserObject();
            if (info.isDescNode())
            {
               mDescTreeItemMenu.show (mTreeModel.tree, e.getX(), e.getY());
            }
            if (info.isChunkNode())
            {
               mChunkTreeItemMenu.show (mTreeModel.tree, e.getX(), e.getY());
            }
         }
      }
   }

   /****************** ConfigModuleListener stuff *********************/
   public void chunkDBAdded (ConfigModuleEvent e)
   {
      mSelectDBBox.addItem (e.getChunkDB().getName());
   }

   public void chunkDBRemoved (ConfigModuleEvent e)
   {
      if (mCurrentDB == e.getChunkDB())
      {
         selectDB ("");
      }
      mSelectDBBox.removeItem (e.getChunkDB().getName());
   }

   public void descDBAdded (ConfigModuleEvent e)
   {
      updateInsertTypes();
   }

   public void descDBRemoved (ConfigModuleEvent e)
   {
      updateInsertTypes();
   }


   /*********************** PlugPanel Stuff ***************************/

   public String getComponentName ()
   {
      return mComponentName;
   }

   public void setComponentName (String _name)
   {
      mComponentName = _name;
   }

   public ImageIcon getComponentIcon ()
   {
      return null;
   }

   public void setConfiguration (ConfigChunk ch) throws VjComponentException
   {
      mComponentChunk = ch;
      mComponentName = ch.getName();

      // get pointers to the modules we need.
      String prop_val = ch.getProperty(VjComponentTokens.DEPENDENCIES).toString();
      PropertyDesc prop_desc = ch.getPropertyDesc(VjComponentTokens.DEPENDENCIES);

      if ( null != prop_val )
      {
         VjComponent c;

         for ( int i = 0; i < prop_desc.getNumAllowed(); ++i )
         {
            c = Core.getVjComponent(ch.getProperty(VjComponentTokens.DEPENDENCIES, i).toString());

            if (c != null)
            {
               if (c instanceof ControlUIModule)
               {
                  mUIModule = (ControlUIModule)c;
               }
               if (c instanceof ConfigModule)
               {
                  mConfigModule = (ConfigModule)c;
               }
               if (c instanceof ConfigUIHelper)
               {
                  mConfigHelperModule = (ConfigUIHelper)c;
               }
            }
         }
      }
   }

   public void setConfigModule(ConfigModule m)
   {
      mConfigModule = m;
   }

   public void setControlUIModule (ControlUIModule m)
   {
      mUIModule = m;
   }

   public void setConfigUIHelper (ConfigUIHelper m)
   {
      mConfigHelperModule = m;
   }

   public void initialize () throws VjComponentException
   {
      if (mUIModule == null || mConfigModule == null || mConfigHelperModule == null)
      {
         throw new VjComponentException (mComponentName + ": Initialize called with unmet dependences.");
      }
      mChunkDBFilter = new SuffixFilter ("Config Files (*.config, *.cfg)", ".config");
      mChunkDBFilter.addSuffix(".cfg");
      mChunkDBFilter = (SuffixFilter)mUIModule.getEasyFileDialog().addFilter (mChunkDBFilter, ConfigTokens.chunk_db_TOKEN);

   }

   public ConfigChunk getConfiguration ()
   {
      return mComponentChunk;
   }

   public VjComponent addConfig (ConfigChunk ch) throws VjComponentException
   {
      throw new VjComponentException (mComponentName + " does not support child component: " + ch.getName());
   }

   public boolean removeConfig (String s)
   {
      return false;
   }

   public JComponent getUIComponent ()
   {
      return this;
   }

   public boolean initUIComponent ()
   {
      if (!mUiInitialized)
      {
         setBorder (BorderFactory.createEmptyBorder (5,5,5,5));
         setLayout (new BorderLayout (5, 5));

         // side panel - add/load etc. buttons

         mLoadButton = new JButton ("Load",
                                    mUIModule.getIcon ("open file", 0));
         mSaveButton = new JButton ("Save",
                                    mUIModule.getIcon ("save file", 0));
         mNewButton = new JButton ("New",
                                   mUIModule.getIcon ("new file", 0));
         mCloseButton = new JButton ("Close",
                                     mUIModule.getIcon ("close file", 0));

         Insets ins = new Insets (0,0,0,0);
         mLoadButton.setMargin (ins);
         mSaveButton.setMargin (ins);
         mNewButton.setMargin (ins);
         mCloseButton.setMargin (ins);

         JPanel side_panel = new JPanel();
         side_panel.setLayout (new GridLayout (12, 1));

         side_panel.add (new JLabel ("File"));

         side_panel.add (mNewButton);
         side_panel.add (mLoadButton);
         side_panel.add (mSaveButton);
         side_panel.add (mCloseButton);

         side_panel.add (mCheckDependenciesButton = new JButton ("Verify"));

         side_panel.add (new JLabel ("Chunk"));
         if (mControlsOnSide == 0)
         {
            side_panel.add (mSendButton = new JButton (">>"));
            side_panel.add (mSendAllButton = new JButton (">All>"));
            add (side_panel, "East");
         }
         else
         {
            side_panel.add (mSendButton = new JButton ("<<"));
            side_panel.add (mSendAllButton = new JButton ("<All<"));
            add (side_panel, "West");
         }

         side_panel.add (mRemoveButton = new JButton ("Remove"));
         side_panel.add (mDuplicateButton = new JButton ("Duplicate"));
         side_panel.add (mInsertButton = new JButton ("Insert"));

         // south: insert types selector
         JPanel south_panel = new JPanel();
         south_panel.setLayout (new BoxLayout (south_panel, BoxLayout.X_AXIS));
         south_panel.add (new JLabel ("Insert type:"));
         mInsertTypeBox = new JComboBox();
         south_panel.add (mInsertTypeBox);
         mChunkHelpButton = new JButton ("Help");
         mChunkHelpButton.setMargin (new Insets (0,0,0,0));
         south_panel.add (mChunkHelpButton);

         add (south_panel, "South");

         // center: scrolled pane w/ tree
         //Box center_panel = new Box (BoxLayout.Y_AXIS);
         JPanel center_panel = new JPanel();
         GridBagLayout gbl = new GridBagLayout();
         GridBagConstraints gbc = new GridBagConstraints();
         center_panel.setLayout (gbl);

         gbc.gridwidth = gbc.REMAINDER;
         gbc.anchor = gbc.EAST;
         gbc.weightx = 1;
         gbc.fill = gbc.BOTH;
         gbc.weighty = 0;

         mSelectDBBox = new JComboBox();
         gbl.setConstraints (mSelectDBBox, gbc);
         center_panel.add (mSelectDBBox);

         mScrollPane = new JScrollPane (JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                                        JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

         gbc.weighty = 1;
         gbl.setConstraints (mScrollPane, gbc);
         center_panel.add (mScrollPane);

         add (center_panel, "Center");

         // setup event handling
         mSelectDBBox.addActionListener (this);
         mLoadButton.addActionListener (this);
         mSaveButton.addActionListener (this);
         mCheckDependenciesButton.addActionListener (this);
         mInsertButton.addActionListener (this);
         mDuplicateButton.addActionListener (this);
         mRemoveButton.addActionListener (this);
         mSendButton.addActionListener (this);
         mSendAllButton.addActionListener (this);
         mNewButton.addActionListener (this);
         mCloseButton.addActionListener (this);
         mChunkHelpButton.addActionListener (this);

         // tooltips
         mNewButton.setToolTipText ("Create a new Config file");
         mLoadButton.setToolTipText ("Load another Config file");
         mSaveButton.setToolTipText ("Save this Config file");
         mCloseButton.setToolTipText ("Close this file (doesn't save)");
         mCheckDependenciesButton.setToolTipText ("Check for missing dependencies");
         mSendButton.setToolTipText ("Copy selected chunks to the other panel");
         mSendAllButton.setToolTipText ("Copy all chunks over to the other panel");
         mInsertButton.setToolTipText ("Inserts new chunk (select type below)");
         mDuplicateButton.setToolTipText ("Insert copies of all selected chunks");
         mRemoveButton.setToolTipText ("Remove all selected chunks");
         mChunkHelpButton.setToolTipText ("Information about the insert selection");

         // initialize popup menus
         mTreeItemMenuPath = null;
         mDescTreeItemMenu = new JPopupMenu ();
         mDescTreeItemMenu.add (mHelp1MI = new JMenuItem ("Chunk Help"));
         mDescTreeItemMenu.add (mInsertMI = new JMenuItem ("Insert"));
         mChunkTreeItemMenu = new JPopupMenu ();
         mChunkTreeItemMenu.add (mHelp2MI = new JMenuItem ("Chunk Help"));
         mChunkTreeItemMenu.add (mRemoveMI = new JMenuItem ("Remove"));

         mHelp1MI.addActionListener (this);
         mHelp2MI.addActionListener (this);
         mRemoveMI.addActionListener (this);
         mInsertMI.addActionListener (this);


         // set up initial tree viewing stuff. can't use selectDB() for this
         // because the mCurrentDB is already null and it decides not to do
         // anything.
         mTreeModel = mConfigHelperModule.getChunkDBTreeModel (mCurrentDB);
         mScrollPane.setViewportView (mTreeModel.tree);
         mTreeModel.tree.addMouseListener(this);
         mSelectDBBox.setSelectedItem (mTreeModel.getName());
         setButtonsEnabled (false);

         buildDBList();

         // listen for new/delete mCurrentDB events from Core
         mConfigModule.addConfigModuleListener (this);

         // get initial set of chunkdesc names for the type selector box.
         updateInsertTypes();

         mUiInitialized = true;
      }
      return mUiInitialized;
   }

   public void destroy ()
   {
      if (mUiInitialized)
      {
         mConfigModule.removeConfigModuleListener (this);
      }
   }

   public void rebuildDisplay ()
   {
      if (mUiInitialized)
      {
         String s = getDBName();
         selectDB (null);
         selectDB (s);
      }
   }
}
