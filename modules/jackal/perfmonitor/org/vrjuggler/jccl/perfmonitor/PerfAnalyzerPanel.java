/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
package org.vrjuggler.jccl.perfmonitor;

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.util.*;
import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.tree.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.vjcontrol.*;
import org.vrjuggler.jccl.vjcontrol.ui.*;

/** PlugPanel for displaying data from PerformanceModule
 *  This panel is connected to a PerfDataCollection object.
 *  It will include a chooser to select which data set in the collection
 *  to show, a scrollpane for displaying text or graphics, and some other
 *  gadgest to control what gets displayed.
 *
 *  @see PerformanceModule
 *
 *  @version $Revision$
 */
public class PerfAnalyzerPanel
   extends JPanel
   implements PlugPanel,
               ActionListener,
PerformanceModuleListener
{

   protected interface DataPanelElem
      extends ActionListener
   {

      public void destroy ();

      public void update();

      public PerfDataCollector getPerfDataCollector();
   }


   protected class LabeledDataPanelElem
      implements DataPanelElem
   {

      protected LabeledPerfDataCollector col;
      protected DefaultMutableTreeNode col_root;
      protected HashMap node_map; // maps IndexInfos to TreeNodes


      public LabeledDataPanelElem (LabeledPerfDataCollector _col, MutableTreeNode global_root)
      {
         col = _col;
         col_root = new DefaultMutableTreeNode( new PerfTreeNodeInfo (col.getName(), null, col));
         tree_model.insertNodeInto (col_root, global_root, global_root.getChildCount());
         node_map = new HashMap();
      }


      public void initialize ()
      {
         Iterator i = col.indexIterator();
         while (i.hasNext())
         {
            LabeledPerfDataCollector.IndexInfo ii = (LabeledPerfDataCollector.IndexInfo)i.next();
            addToTree (ii);
         }

         col.addActionListener (this);
      }


      public void destroy ()
      {
         col.removeActionListener (this);
      }


      protected void addToTree (LabeledPerfDataCollector.IndexInfo ii)
      {
         MutableTreeNode node = col_root;
         MutableTreeNode new_node;
         int i, j, n;  // i = index into ii.label_components
         i = 0;
         // first, find the parent
         //System.out.println ("add to tree: " + ii.label_components);
         for (i = 0; i < ii.label_components.size() - 1; i++)
         {
            new_node = null;
            for (j = 0; j < node.getChildCount(); j++)
            {
               DefaultMutableTreeNode mtn = (DefaultMutableTreeNode)node.getChildAt(j);
               PerfTreeNodeInfo te = (PerfTreeNodeInfo)mtn.getUserObject();
               if (te.sublabel.equals((String)ii.label_components.get(i)))
               {
                  new_node = mtn;
               }
            }
            if (new_node == null)
            {
               // didn't find it, create folder node
               PerfTreeNodeInfo ni = new PerfTreeNodeInfo ((String)ii.label_components.get(i), null, col);
               new_node = new DefaultMutableTreeNode(ni);
               tree_model.insertNodeInto (new_node, node, node.getChildCount());
               ni.getGraphButton().addActionListener (PerfAnalyzerPanel.this);
            }
            node = new_node;
         }
         // add ii as a child node of node.
         PerfTreeNodeInfo ni = new PerfTreeNodeInfo ((String)ii.label_components.get(ii.label_components.size()-1), ii, col);
         new_node = new DefaultMutableTreeNode(ni);
         tree_model.insertNodeInto (new_node, node, node.getChildCount());
         ni.getGraphButton().addActionListener (PerfAnalyzerPanel.this);

         node_map.put (ii, new_node);

         //System.out.println ("tree is: \n" + col_root);
      }


      public void update()
      {
         Iterator i = col.indexIterator();
         int j = 0;

         // update indices
         while (i.hasNext())
         {
            LabeledPerfDataCollector.IndexInfo ii = (LabeledPerfDataCollector.IndexInfo)i.next();
            //System.out.println ("updating " + ii.index);
            DefaultMutableTreeNode tn = (DefaultMutableTreeNode)node_map.get(ii);
            if (tn == null)
               addToTree (ii);
            else
            {
               PerfTreeNodeInfo ni = (PerfTreeNodeInfo)tn.getUserObject();
               ni.update();
            }
         }

         // traverse tree & calc new averages.
         doUpdateTreeAverages (col_root);

         tree_model.nodeChanged (col_root);
      }


      /** Utility for update.
       *  Does a preorder traversal of this' subtree.
       */
      protected void doUpdateTreeAverages (DefaultMutableTreeNode root)
      {
         double sum = 0.0;
         int i, count;
         DefaultMutableTreeNode child;
         PerfTreeNodeInfo ni, child_ni;
         ni = (PerfTreeNodeInfo)root.getUserObject();
         if (ni.getIndexInfo() == null)
         {
            // it's a folder... have to sum up its children.
            count = root.getChildCount();
            for (i = 0; i < count; i++)
            {
               child = (DefaultMutableTreeNode)root.getChildAt(i);
               child_ni = (PerfTreeNodeInfo)child.getUserObject();
               doUpdateTreeAverages(child);
               sum += child_ni.getAverage();
            }
            ni.setAverage (sum);
            ni.update();
            //System.out.println ("set a folder average to: " + sum);
         }
         else
         {
            // it was an actual index & already knows its average
            return;
         }
      }


      public PerfDataCollector getPerfDataCollector()
      {
         return col;
      }


      public void actionPerformed (ActionEvent e)
      {
         if (e.getActionCommand().equals ("Update"))
         {
            update();
         }
      }

   }



   JComboBox  display_choice;
   JComboBox  max_samples_box;
   JTextField preskip_box;
   JTextField postskip_box;
   JButton savecontents_button;
   JButton load_button;
   JButton print_all_button;
   JButton graph_button;
   JPanel data_panel;
   GridBagLayout gblayout;
   GridBagConstraints gbc;
   boolean ui_initialized;

   JScrollPane display_pane;
   JTextArea text_area;
   DefaultMutableTreeNode root;
   JTree tree;
   DefaultTreeModel tree_model;
   PanelTree panel_tree;

   PerformanceModule perf_module;
   protected String component_name;
   protected ConfigChunk component_chunk;

   ControlUIModule ui_module;
   protected SuffixFilter perf_filter;

   Vector child_frames;  // vector of GenericGraphFrames...
   Vector datapanel_elems;

   int preskip, postskip;
   float anomalycutoff;
   boolean doanomaly;


   public PerfAnalyzerPanel ()
   {
      super();

      component_name = "Unconfigured PerfAnalyzerPanel";
      component_chunk = null;

      child_frames = new Vector();
      datapanel_elems = new Vector();
      perf_module = null;
      ui_module = null;
      text_area = null;
      root = new DefaultMutableTreeNode();
      tree_model = new DefaultTreeModel(root);

      preskip = 20;
      postskip = 20;
      doanomaly = true;
      anomalycutoff = 1.0f;

      ui_initialized = false;

   }


   /** Utility method for various printing routines. */
   private String padFloat (double f)
   {
      // reformats f to a string w/ 3 places after decimal
      String s = Double.toString(f);
      if (s.indexOf('E') != -1)
      {
         return s;
      }
      int i = s.lastIndexOf('.');
      if ((i >= 0) && (i+5 < s.length()))
      {
         s = s.substring (0, i + 5);
      }
      return s;
   }


   public DataPanelElem getDataPanelElem (PerfDataCollector col)
   {
      DataPanelElem dpe;
      for (int i = 0; i < datapanel_elems.size(); i++)
      {
         dpe = (DataPanelElem)datapanel_elems.elementAt(i);
         if (col == dpe.getPerfDataCollector())
         {
            return dpe;
         }
      }
      return null;
   }


   public void addDataPanelElem (PerfDataCollector col)
   {
      DataPanelElem dpe;
//          if (col instanceof NumberedPerfDataCollector) {
//              dpe = new NumberedDataPanelElem ((NumberedPerfDataCollector)col);
//  	    if (ui_initialized)
//  		((NumberedDataPanelElem)dpe).initialize (data_panel, gblayout, gbc);
//  	}
//          else {
      dpe = new LabeledDataPanelElem ((LabeledPerfDataCollector)col, root);
      if (ui_initialized)
         ((LabeledDataPanelElem)dpe).initialize ();
//  	}
      datapanel_elems.add(dpe);
   }


   public void actionPerformed (ActionEvent e)
   {
      Object source = e.getSource();

      if (source instanceof GenericGraphFrame)
      {
         GenericGraphFrame f = (GenericGraphFrame)source;
         child_frames.removeElement(f);
         f.destroy();
      }
      else if (source instanceof PerfTreeNodeInfo.LabeledPanelButton)
      {
         PerfTreeNodeInfo.LabeledPanelButton b = (PerfTreeNodeInfo.LabeledPanelButton)e.getSource();
         if (e.getActionCommand().equals ("Graph"))
         {
            GenericGraphPanel gp = new LabeledSummaryGraphPanel (b.collector, b.index_info);
            String title = "Graph of " + b.collector.getName();
            if (b.index_info != null)
            {
               title = title + " label = " + b.index_info.index;
            }
            GenericGraphFrame gf = new GenericGraphFrame (gp, title);
            gf.addActionListener (this);
            child_frames.addElement (gf);
            gf.show();
         }
      }
      else if (source == max_samples_box)
      {
         int numsamps;
         String s = (String)max_samples_box.getSelectedItem();
         if (s.equalsIgnoreCase("<Infinite>"))
         {
            numsamps = -1;
         }
         else
         {
            numsamps = Integer.parseInt(s);
         }

         //System.out.println ("setting max samples to " + numsamps);
         perf_module.setMaxSamples(numsamps);
      }
      else if (source == display_choice)
      {
         refreshDisplay();
      }
      else if (source == preskip_box)
      {
         preskip = Integer.parseInt (preskip_box.getText().trim());
         refreshDisplay();
      }
      else if (source == postskip_box)
      {
         postskip = Integer.parseInt (postskip_box.getText().trim());
         refreshDisplay();
      }
      else if (source == savecontents_button)
      {
         //collection.savePerfDataFile ();
         File f = ui_module.getEasyFileDialog().requestSaveFile(perf_module.getFile(), ui_module, perf_filter);
         perf_module.savePerfDataFile (f);
      }
      else if (source == load_button)
      {
         File f = ui_module.getEasyFileDialog().requestOpenFile (perf_module.getFile().getParentFile(), ui_module, perf_filter);
         if (f != null)
         {
            String name = perf_module.loadNewPerfDataFile (f);
            if (name != null)
            {
               refreshDisplay();
            }
         }
      }
      else if (source == print_all_button)
      {
         String s = "";
         PerfDataCollector col;
         for (int i = 0; i < perf_module.getSize(); i++)
         {
            col = perf_module.getCollector(i);
            s += col.dumpAverages (preskip, postskip, false, anomalycutoff);
         }
         System.out.println (s);
      }
      else if (source == graph_button)
      {
         TreePath[] selections = tree.getSelectionPaths();
         for (int i = 0; i < selections.length; i++)
         {
            DefaultMutableTreeNode node;
            node = (DefaultMutableTreeNode)selections[i].getLastPathComponent();
            PerfTreeNodeInfo ni = (PerfTreeNodeInfo)node.getUserObject();
            if (ni != null)
            {
               GenericGraphPanel gp = new LabeledSummaryGraphPanel (ni.getCollector(), ni.getIndexInfo());
               String title = "Graph of " + ni.getCollector().getName();
               if (ni.getIndexInfo() != null)
               {
                  title = title + " label = " + ni.getIndexInfo().index;
               }
               GenericGraphFrame gf = new GenericGraphFrame (gp, title);
               gf.addActionListener (this);
               child_frames.addElement (gf);
               gf.show();
            }
         }
      }
   }


   public void refreshDisplay()
   {
      if (ui_initialized)
      {
         DataPanelElem dpe;
         int n = datapanel_elems.size();
         for (int i = 0; i < n; i++)
         {
            dpe = (DataPanelElem)datapanel_elems.elementAt(i);
            dpe.update();
         }
      }
   }


   public void removeAllData()
   {
      if (ui_initialized)
      {
         int i, n;

         for (i = 0; i < child_frames.size(); i++)
         {
            GenericGraphFrame f = (GenericGraphFrame)child_frames.get(i);
            f.destroy();
         }
         child_frames.removeAllElements();

         DataPanelElem dpe;
         n = datapanel_elems.size();
         for (i = 0; i < n; i++)
         {
            dpe = (DataPanelElem)datapanel_elems.elementAt(i);
            dpe.destroy();
         }
         datapanel_elems.clear();
      }
   }


   /********************** PlugPanel Stuff **************************/

   public String getComponentName ()
   {
      return component_name;
   }


   public void setComponentName (String _name)
   {
      component_name = _name;
   }


   public ImageIcon getComponentIcon ()
   {
      return null;
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
         int i;
         int n = ch.getPropertyCount(VjComponentTokens.DEPENDENCIES);
         String s;
         VjComponent c;
         for (i = 0; i < n; i++)
         {
            s = ch.getProperty(VjComponentTokens.DEPENDENCIES, i).toString();
            c = Core.getVjComponent (s);
            if (c != null)
            {
               if (c instanceof ControlUIModule)
               {
                  ui_module = (ControlUIModule)c;
               }
               if (c instanceof PerformanceModule)
               {
                  perf_module = (PerformanceModule)c;
               }
            }
         }
      }
   }


   public void initialize () throws VjComponentException
   {
      if (perf_module == null || ui_module == null)
      {
         throw new VjComponentException (component_name + ": Instantiated with unmet dependencies.");
      }

      perf_module.addPerformanceModuleListener (this);
      // get all the alread-added perf modules...
      for (int i = 0; i < perf_module.getSize(); i++)
      {
         addDataPanelElem (perf_module.getCollector(i));
      }

      perf_filter = new SuffixFilter ("Perf Data Files (*.perf)", ".perf");
      ui_module.getEasyFileDialog().addFilter (perf_filter, "PerfData");

      refreshDisplay();
   }


   public VjComponent addConfig (ConfigChunk ch) throws VjComponentException
   {
      throw new VjComponentException (component_name + " does not support child component: " + ch.getName());
   }


   public boolean removeConfig (String name)
   {
      return false;
   }


   public JComponent getUIComponent ()
   {
      return this;
   }


   public boolean initUIComponent()
   {
      if (!ui_initialized)
      {
         setLayout (new BorderLayout (5, 5));
         setBorder (new EmptyBorder (5,5,5,5));

         data_panel = new JPanel();
         data_panel.setLayout (gblayout = new GridBagLayout());
         gbc = new GridBagConstraints();
         gbc.insets = new Insets (0, 4, 0, 4);
         gbc.fill = gbc.HORIZONTAL;


         JPanel epanel = new JPanel ();
         epanel.setLayout (new GridLayout (10, 1, 2, 0));
         add (epanel, "East");

         load_button = new JButton ("Load");
         epanel.add (load_button);
         load_button.addActionListener (this);

         epanel.add (savecontents_button = new JButton ("Save Data"));
         savecontents_button.addActionListener (this);

         print_all_button = new JButton ("Print");
         print_all_button.addActionListener (this);
         epanel.add (print_all_button);

         graph_button = new JButton ("Graph");
         graph_button.addActionListener (this);
         epanel.add (graph_button);

         JPanel npanel = new JPanel ();
         npanel.setLayout (new BoxLayout (npanel, BoxLayout.Y_AXIS));
         add (npanel, "North");
         JPanel ntoppanel = new JPanel();
         ntoppanel.setLayout (new BoxLayout (ntoppanel, BoxLayout.X_AXIS));
         npanel.add (ntoppanel);

         ntoppanel.add (new JLabel ("Maximum stored samples"));
         ntoppanel.add (max_samples_box = new JComboBox());
         max_samples_box.addItem ("100");
         max_samples_box.addItem ("500");
         max_samples_box.addItem ("1000");
         max_samples_box.addItem ("5000");
         max_samples_box.addItem ("<Infinite>");
         max_samples_box.setSelectedIndex(1);
         max_samples_box.addActionListener (this);

         //            data_panel.setLayout (new BoxLayout (data_panel, BoxLayout.Y_AXIS));
         GridBagLayout gbl = new GridBagLayout();
         data_panel.setLayout (gbl);
         //data_panel.setBackground (Color.WHITE);

         tree = new JTree (tree_model);
         tree.setCellRenderer (new LabeledPerfTreeCellRenderer(tree.getCellRenderer()));
         tree.setRootVisible (true);
         display_pane = new JScrollPane (tree, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

//              display_pane = new JScrollPane (data_panel, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

         add (display_pane, "Center");

//              for (int i = 0; i < datapanel_elems.size(); i++) {
//                  DataPanelElem dpe = (DataPanelElem)datapanel_elems.get(i);
//  		if (dpe instanceof LabeledDataPanelElem)
//  		    ((LabeledDataPanelElem)dpe).initialize();
//  		else
//  		    ((NumberedDataPanelElem)dpe).initialize (data_panel, gblayout, gbc);
//              }
//              GridBagConstraints gbc = new GridBagConstraints();
//              gbc.anchor = GridBagConstraints.NORTHWEST;
//              gbc.fill = GridBagConstraints.NONE;
//              gbc.gridwidth = GridBagConstraints.RELATIVE;
//              gbc.gridheight = GridBagConstraints.RELATIVE;
//  	    panel_tree = new PanelTree (tree_model);
//              gbl.setConstraints (panel_tree, gbc);
//  	    data_panel.add (panel_tree);
//              gbc.fill = GridBagConstraints.BOTH;
//              gbc.gridwidth = GridBagConstraints.REMAINDER;
//              Component comp = Box.createHorizontalGlue();
//              gbl.setConstraints (comp, gbc);
//              data_panel.add (comp);
//              gbc.gridheight = GridBagConstraints.REMAINDER;
//              comp = Box.createVerticalGlue();
//              gbl.setConstraints (comp, gbc);
//              data_panel.add (comp);
         //data_panel.add (Box.createVerticalGlue());
//              display_pane = new JScrollPane (panel_tree, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

//              add (display_pane, "Center");

         ui_initialized = true;
      }
      return true;
   }


   public void destroy ()
   {
      removeAllData ();
   }


   public void rebuildDisplay ()
   {
   }


   //------------------ PerformanceModuleListener Stuff ---------------------

   public void perfDataCollectorAdded (PerformanceModuleEvent e)
   {
      addDataPanelElem (e.getPerfDataCollector());
   }

   public void perfDataCollectorRemoved (PerformanceModuleEvent e)
   {
      // not implemented
   }

   public void perfDataCollectorsCleared (PerformanceModuleEvent e)
   {
      removeAllData ();
   }

}
