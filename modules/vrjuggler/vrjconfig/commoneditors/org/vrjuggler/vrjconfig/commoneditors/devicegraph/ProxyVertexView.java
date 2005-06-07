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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Window;
import java.awt.datatransfer.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
//import java.awt.event.MouseEvent;
import java.awt.Insets;
import java.util.Iterator;
import java.util.Map;
import javax.swing.*;

import org.jgraph.JGraph;
import org.jgraph.graph.AttributeMap;
import org.jgraph.graph.CellView;
import org.jgraph.graph.CellViewRenderer;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.VertexView;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;
import org.vrjuggler.jccl.editors.DataFlavorRepository;
import org.vrjuggler.jccl.editors.TransferableConfigElement;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;


/**
 * A custom vertex view for graph cells representing device proxy config
 * elements.  This view makes use of heavy-weight renderers so that users may
 * interact with components in the vertex view.
 */
public class ProxyVertexView
   extends VertexView
{
   private transient ProxyVertexRenderer renderer = new ProxyVertexRenderer();

   public ProxyVertexView()
   {
      super();
   }

   public ProxyVertexView(Object cell)
   {
      super(cell);
   }

   public CellViewRenderer getRenderer()
   {
      return renderer;
   }

   /**
    * A heavy-weight renderer for use by ProxyVertexView.
    */
   private static class ProxyVertexRenderer
      extends AbstractCustomVertexRenderer
      implements ClipboardOwner
               , ConfigElementListener
   {
      private static final int LABEL_START_COLUMN   = 1;
      private static final int LABEL_END_COLUMN     = 1;
      private static final int BUTTON0_START_COLUMN = 3;
      private static final int BUTTON0_END_COLUMN   = 3;
      private static final int BUTTON1_START_COLUMN = 5;
      private static final int BUTTON1_END_COLUMN   = 5;
      private static final int BUTTON2_START_COLUMN = 7;
      private static final int BUTTON2_END_COLUMN   = 7;

      private static final int SPAN_START_COLUMN = LABEL_START_COLUMN;
      private static final int SPAN_END_COLUMN   = BUTTON2_END_COLUMN;

      private static final int BUTTON_MIN_WIDTH   = 30;
      private static final int BUTTON_MIN_HEIGHT  = 16;
      private static final int BUTTON_PREF_WIDTH  = 30;
      private static final int BUTTON_PREF_HEIGHT = 16;

      private static final int NAME_ROW         = 1;
      private static final int ADD_BUTTON_ROW   = 2;
      private static final int PASTE_BUTTON_ROW = 3;
      private static final int FIRST_ALIAS_ROW  = PASTE_BUTTON_ROW + 1;

      private static String IMG_ROOT = EditorConstants.COMMON_IMG_ROOT;

      public ProxyVertexRenderer()
      {
         super();

         double[][] sizes =
            {
               {5, TableLayout.FILL, 5, TableLayout.MINIMUM, 5,
                TableLayout.MINIMUM, 5, TableLayout.MINIMUM, 5},
               {5, TableLayout.PREFERRED, TableLayout.MINIMUM,
                TableLayout.MINIMUM}
            };
         mMainLayout = new TableLayout(sizes);
         this.setLayout(mMainLayout);

         // Set some text in the name label so that it has a preferred size
         // that can be used for later calculations.
         nameLabel.setText(" ");

         mAddAliasButton.setEnabled(false);
         mAddAliasButton.setToolTipText("Add a new alias for this proxy");
         mAddAliasButton.setText("Create Alias");
         mAddAliasButton.setMargin(new Insets(3, 0, 0, 0));
         mAddAliasButton.setBorderPainted(false);
         mAddAliasButton.setFont(new Font("Dialog", Font.PLAIN, 10));
         mAddAliasButton.setContentAreaFilled(false);

         try
         {
            ClassLoader loader = getClass().getClassLoader();
            mAddAliasButton.setIcon(
               new ImageIcon(loader.getResource(IMG_ROOT + "/New16.gif"))
            );
         }
         catch (Exception ex)
         {
         }

         mAddAliasButton.addActionListener(
            new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  ConfigElement elt = createAlias();
                  addAliasRow(elt, true);
               }
            }
         );

         mPasteAliasButton.setEnabled(false);
         mPasteAliasButton.setToolTipText("Paste alias from the clipboard");
         mPasteAliasButton.setText("Paste Alias");
         mPasteAliasButton.setMargin(new Insets(0, 0, 3, 0));
         mPasteAliasButton.setBorderPainted(false);
         mPasteAliasButton.setFont(new Font("Dialog", Font.PLAIN, 10));
         mPasteAliasButton.setContentAreaFilled(false);

         try
         {
            ClassLoader loader = getClass().getClassLoader();
            mPasteAliasButton.setIcon(
               new ImageIcon(loader.getResource(IMG_ROOT + "/Paste16.gif"))
            );
         }
         catch (Exception ex)
         {
         }

         mPasteAliasButton.addActionListener(
            new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  // Get the alias pasted on the clipboard.
                  Clipboard clipboard = getToolkit().getSystemClipboard();
                  Transferable contents = clipboard.getContents(this);

                  DataFlavor flavor =
                     DataFlavorRepository.getConfigElementFlavor();

                  // If the clipboard contents are what we expect, then proceed
                  // with the pasting operation.
                  if ( contents != null &&
                       contents.isDataFlavorSupported(flavor) )
                  {
                     try
                     {
                        ConfigElement alias_elt =
                           (ConfigElement) contents.getTransferData(flavor);
                        addAlias(alias_elt);
                        addAliasRow(alias_elt, true);

                        // Manually disable all the "Paste" buttons in the
                        // proxy vertex renderers.
                        // XXX: It would be better if this would happen
                        // automatically upon reading the contents of the
                        // clipboard.
                        setAllPasteButtonsEnabled(
                           ProxyVertexRenderer.this.graph, false
                        );
                     }
                     // The condition above that tests the transferable object
                     // to see if our data flavor is supported makes it so
                     // that we do not have to worry about this exxception
                     // getting thrown.
                     catch (UnsupportedFlavorException ex)
                     {
                        /* Do nothing. */ ;
                     }
                     catch (java.io.IOException ex)
                     {
                        ex.printStackTrace();
                     }
                     catch (Exception ex)
                     {
                        ex.printStackTrace();
                     }
                  }
               }
            }
         );

         this.add(nameLabel,
                  new TableLayoutConstraints(SPAN_START_COLUMN, NAME_ROW,
                                             SPAN_END_COLUMN, NAME_ROW,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.CENTER));
         this.add(mAddAliasButton,
                  new TableLayoutConstraints(SPAN_START_COLUMN, ADD_BUTTON_ROW,
                                             SPAN_END_COLUMN, ADD_BUTTON_ROW,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.CENTER));
         this.add(mPasteAliasButton,
                  new TableLayoutConstraints(SPAN_START_COLUMN,
                                             PASTE_BUTTON_ROW, SPAN_END_COLUMN,
                                             PASTE_BUTTON_ROW,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.CENTER));
      }

      /**
       * Sets up the renderer component using the given information and then
       * returns it.
       */
      public Component getRendererComponent(JGraph graph, CellView view,
                                            boolean selected, boolean hasFocus,
                                            boolean preview)
      {
         mView = view;

         this.graph    = graph;
         this.selected = selected;
         this.preview  = preview;
         this.hasFocus = hasFocus;

         Map attributes = view.getAllAttributes();
         installAttributes(graph, attributes);

         if ( mProxyInfo == null )
         {
            try
            {
               DefaultGraphCell cell = (DefaultGraphCell) view.getCell();
               mProxyInfo = (ProxyInfo) cell.getUserObject();

               mAddAliasButton.setEnabled(true);

               Dimension label_size = nameLabel.getPreferredSize();
               Dimension add_size   = mAddAliasButton.getPreferredSize();
               Dimension paste_size = mPasteAliasButton.getPreferredSize();

               int min_width = Math.max(label_size.width,
                                        Math.max(add_size.width,
                                                 paste_size.width));

               // The extra 15 units of height is to allow some vertical
               // breathing room.
               int min_height = label_size.height + add_size.height +
                                   paste_size.height + 15;

               setMinimumSize(new Dimension(min_width, min_height));
               // XXX: Doubling the minimum width is a hack to deal with the
               // default preferred alias row sizes not being wide enough.
               setPreferredSize(new Dimension(min_width * 2, min_height));

               for ( Iterator i = mProxyInfo.getAliases().iterator();
                     i.hasNext(); )
               {
                  // Do not revalidate the layout and resize the cell view for
                  // each new row.  We do what is necessary one time after all
                  // the rows are added.
                  addAliasRow((ConfigElement) i.next(), false);
               }

               // At this point, we only revalidate the layout.  There is no
               // need to change the cell view size because it is still being
               // set up by JGraph at this stage of execution.
               this.revalidate();

               mProxyInfo.getElement().addConfigElementListener(this);
            }
            catch (Exception ex)
            {
               ex.printStackTrace();
            }
         }

         if ( graph.getEditingCell() != view.getCell() )
         {
            nameLabel.setText(mProxyInfo.getElement().getName());
         }
         else
         {
            nameLabel.setText(" ");
         }

         return this;
      }

      public void lostOwnership(Clipboard clipboard, Transferable contents)
      {
      }

      public void nameChanged(ConfigElementEvent evt)
      {
         nameLabel.setText(mProxyInfo.getElement().getName());
      }

      public void propertyValueAdded(ConfigElementEvent evt)
      {
      }

      public void propertyValueChanged(ConfigElementEvent evt)
      {
      }

      public void propertyValueOrderChanged(ConfigElementEvent evt)
      {
      }

      public void propertyValueRemoved(ConfigElementEvent evt)
      {
      }

      private ConfigElement createAlias()
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition alias_def =
            broker.getRepository().get(EditorConstants.ALIAS_TYPE);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());

         ConfigContext context = mProxyInfo.getContext();
         ConfigElement alias_elt = factory.createUnique(alias_def, context);
         alias_elt.setProperty(EditorConstants.PROXY_PROPERTY, 0,
                               mProxyInfo.getElement().getName());

         addAlias(alias_elt);

         return alias_elt;
      }

      private void addAlias(ConfigElement aliasElt)
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigContext context    = mProxyInfo.getContext();

         for ( Iterator i = broker.getResourceNames().iterator();
               i.hasNext(); )
         {
            DataSource ds = broker.get((String) i.next());
            if ( ds.contains(mProxyInfo.getElement()) )
            {
               broker.add(context, aliasElt, ds);
               break;
            }
         }

         mProxyInfo.addAlias(aliasElt);
      }

      /**
       * Adds a new row for the given config element that is an alias for
       * the proxy being rendered.  If the caller chooses not to resize the
       * cell view after adding the new row to this renderer, it is the
       * responsibility of the caller to perform the layout validation and
       * the cell view resizing.
       *
       * @param aliasElt        the config element for the alias that will be
       *                        represented by the newly added row
       * @param resizeCellView  a flag indicating whether or not to revalidate
       *                        the renderer layout and to resize the cell
       *                        view associated with this renderer
       *
       * @see #revalidate()
       * @see GraphHelpers#autoSizeCellView(JGraph,Dimension,CellView)
       */
      private void addAliasRow(final ConfigElement aliasElt,
                               boolean resizeCellView)
      {
         final ConfigContext ctx = mProxyInfo.getContext();

         final JLabel name_field = new JLabel(aliasElt.getName());
         name_field.setFont(new Font("Dialog", Font.ITALIC,
                                     nameLabel.getFont().getSize()));
         name_field.setBorder(BorderFactory.createLineBorder(Color.black));
         name_field.setBackground(
            gradientColor != null ? gradientColor.brighter().brighter()
                                  : this.getBackground().brighter().brighter()
         );
/*
         name_field.setTransferHandler(new TransferHandler("text"));
         name_field.addMouseListener(
            new java.awt.event.MouseAdapter()
            {
               public void mousePressed(MouseEvent e)
               {
                  System.out.println("Mouse pressed on JLabel");
                  JComponent c = (JComponent) e.getSource();
                  TransferHandler handler = c.getTransferHandler();
                  handler.exportAsDrag(c, e, TransferHandler.COPY);
               }
            }
         );
*/

         final JButton edit_btn = new JButton();
         final JButton cut_btn = new JButton();
         final JButton remove_btn = new JButton();

         edit_btn.setEnabled(true);
         edit_btn.setToolTipText("Edit the name of this proxy alias");

         try
         {
            ClassLoader loader = getClass().getClassLoader();

            edit_btn.setIcon(
               new ImageIcon(loader.getResource(IMG_ROOT + "/Edit16.png"))
            );
            edit_btn.setMargin(new Insets(0, 0, 0, 0));
            edit_btn.setBorderPainted(false);
            edit_btn.setFocusPainted(false);
            edit_btn.setContentAreaFilled(false);
         }
         catch (Exception ex)
         {
            edit_btn.setMinimumSize(new Dimension(BUTTON_MIN_WIDTH,
                                                  BUTTON_MIN_HEIGHT));
            edit_btn.setPreferredSize(new Dimension(BUTTON_PREF_WIDTH,
                                                    BUTTON_PREF_HEIGHT));
            edit_btn.setFont(new Font("Dialog", Font.PLAIN, 9));
            edit_btn.setText("Edit");
         }

         edit_btn.addActionListener(
            new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  Window parent =
                     (Window) SwingUtilities.getAncestorOfClass(Window.class,
                                                                ProxyVertexRenderer.this);
                  // Open up an input dialog to modify the alias' name.
                  String new_name =
                     JOptionPane.showInputDialog(parent,
                                                 "Modify the alias name",
                                                 aliasElt.getName());

                  // If the user selected a valid name using the dialog box,
                  // change the config element name.
                  if ( new_name != null && ! new_name.equals("") &&
                       ! aliasElt.getName().equals(new_name) )
                  {
                     try
                     {
                        aliasElt.setName(new_name, mProxyInfo.getContext());
                        name_field.setText(new_name);
                     }
                     catch (Exception ex)
                     {
                        String message =
                           "Name change failed:\n" + ex.getMessage();
                        JOptionPane.showMessageDialog(
                           parent, message,
                           "Alias Name Change Failed",
                           JOptionPane.ERROR_MESSAGE
                        );
                     }
                  }
               }
            }
         );

         cut_btn.setEnabled(true);
         cut_btn.setToolTipText(
            "Remove this proxy alias and copy it to the clipboard"
         );

         try
         {
            ClassLoader loader = getClass().getClassLoader();

            cut_btn.setIcon(
               new ImageIcon(loader.getResource(IMG_ROOT + "/Cut16.gif"))
            );
            cut_btn.setMargin(new Insets(0, 0, 0, 0));
            cut_btn.setBorderPainted(false);
            cut_btn.setFocusPainted(false);
            cut_btn.setContentAreaFilled(false);
         }
         catch (Exception ex)
         {
            cut_btn.setMinimumSize(new Dimension(BUTTON_MIN_WIDTH,
                                                 BUTTON_MIN_HEIGHT));
            cut_btn.setPreferredSize(new Dimension(BUTTON_PREF_WIDTH,
                                                   BUTTON_PREF_HEIGHT));
            cut_btn.setFont(new Font("Dialog", Font.PLAIN, 9));
            cut_btn.setText("Cut");
         }

         cut_btn.addActionListener(
            new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  Clipboard clipboard = getToolkit().getSystemClipboard();
                  clipboard.setContents(new TransferableConfigElement(aliasElt),
                                        ProxyVertexRenderer.this);

                  // Manually enable all the "Paste" buttons in the proxy
                  // vertex renderers.
                  // XXX: It would be better if this would happen automatically
                  // upon setting the contents of the clipboard.
                  setAllPasteButtonsEnabled(ProxyVertexRenderer.this.graph,
                                            true);

                  JComponent[] components = new JComponent[]{name_field,
                                                             edit_btn, cut_btn,
                                                             remove_btn};
                  removeAliasRow(components);
                  removeAlias(aliasElt);
               }
            }
         );

         remove_btn.setEnabled(true);
         remove_btn.setToolTipText("Delete this proxy alias");

         try
         {
            ClassLoader loader = getClass().getClassLoader();

            remove_btn.setIcon(
               new ImageIcon(loader.getResource(IMG_ROOT + "/Delete16.gif"))
            );
            remove_btn.setMargin(new Insets(0, 0, 0, 0));
            remove_btn.setBorderPainted(false);
            remove_btn.setFocusPainted(false);
            remove_btn.setContentAreaFilled(false);
         }
         catch (Exception ex)
         {
            remove_btn.setMinimumSize(new Dimension(BUTTON_MIN_WIDTH,
                                                    BUTTON_MIN_HEIGHT));
            remove_btn.setPreferredSize(new Dimension(BUTTON_PREF_WIDTH,
                                                      BUTTON_PREF_HEIGHT));
            remove_btn.setFont(new Font("Dialog", Font.PLAIN, 9));
            remove_btn.setText("Delete");
         }

         remove_btn.addActionListener(
            new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  JComponent[] components = new JComponent[]{name_field,
                                                             edit_btn, cut_btn,
                                                             remove_btn};
                  removeAliasRow(components);
                  removeAlias(aliasElt);
               }
            }
         );

         // Define button columns for the layout.
         int edit_btn_start   = BUTTON0_START_COLUMN,
             edit_btn_end     = BUTTON0_END_COLUMN,
             cut_btn_start    = BUTTON1_START_COLUMN,
             cut_btn_end      = BUTTON1_END_COLUMN,
             remove_btn_start = BUTTON2_START_COLUMN,
             remove_btn_end   = BUTTON2_END_COLUMN;

         int row = mMainLayout.getNumRow();
         mMainLayout.insertRow(row, TableLayoutConstraints.PREFERRED);
         this.add(name_field,
                  new TableLayoutConstraints(LABEL_START_COLUMN, row,
                                             LABEL_END_COLUMN, row,
                                             TableLayoutConstraints.FULL,
                                             TableLayoutConstraints.CENTER));
         this.add(edit_btn,
                  new TableLayoutConstraints(edit_btn_start, row,
                                             edit_btn_end, row,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.CENTER));
         this.add(cut_btn,
                  new TableLayoutConstraints(cut_btn_start, row,
                                             cut_btn_end, row,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.CENTER));
         this.add(remove_btn,
                  new TableLayoutConstraints(remove_btn_start, row,
                                             remove_btn_end, row,
                                             TableLayoutConstraints.CENTER,
                                             TableLayoutConstraints.CENTER));

         // Update the preferred size of the renderer based on the
         // dimensions of the newly added row.
         int width = name_field.getPreferredSize().width +
                        edit_btn.getPreferredSize().width +
                        cut_btn.getPreferredSize().width +
                        remove_btn.getPreferredSize().width;
         int height = name_field.getPreferredSize().height;

         Dimension pref_size = this.getPreferredSize();
         int new_width  = Math.max(width, pref_size.width);
         int new_height = height + pref_size.height;
         pref_size = new Dimension(new_width, new_height);
         this.setPreferredSize(pref_size);

         // Only resize the cell view associated with this renderer if we are
         // told to do so.  This is a bit of an optimization.
         if ( resizeCellView )
         {
            this.revalidate();
            GraphHelpers.autoSizeCellView(this.graph, pref_size, mView);
         }
      }

      private static void setAllPasteButtonsEnabled(JGraph graph,
                                                    boolean enabled)
      {
         CellView[] views = graph.getGraphLayoutCache().getCellViews();
         for ( int i = 0; i < views.length; ++i )
         {
            Component c = views[i].getRendererComponent(graph, false,
                                                        false, false);
            if ( c instanceof ProxyVertexRenderer )
            {
               ((ProxyVertexRenderer) c).mPasteAliasButton.setEnabled(enabled);
            }
         }
      }

      private void removeAliasRow(JComponent[] components)
      {
         // Get the preferred size of the renderer so that we can modify it
         // after the row is removed.
         Dimension pref_size = this.getPreferredSize();

         // Set up the new preferred width and height based on the row that
         // is being removed.
         int width = pref_size.width;
         int height =
            pref_size.height - components[0].getPreferredSize().height;

         // Find the row containing the components being removed.  We expect
         // that all the components in the given array are in the same row.
         // XXX: Is there a more robust way to do this?
         TableLayoutConstraints tlc = mMainLayout.getConstraints(components[0]);
         int row = tlc.row1;

         // Remove all the components in this row (at least as far as we know).
         for ( int i = 0; i < components.length; ++i )
         {
            this.remove(components[i]);
         }

         mMainLayout.deleteRow(row);

         // Change the preferred size of the renderer to account for the row
         // removal.
         pref_size = new Dimension(width, height);
         this.setPreferredSize(pref_size);

         this.revalidate();

         // Resize the cell view now that the renderer has changed.
         GraphHelpers.autoSizeCellView(this.graph, pref_size, mView);
      }

      private void removeAlias(ConfigElement aliasElt)
      {
         mProxyInfo.getAliases().remove(aliasElt);

         ConfigBroker broker = new ConfigBrokerProxy();

         for ( Iterator i = broker.getResourceNames().iterator();
               i.hasNext(); )
         {
            DataSource ds = broker.get((String) i.next());

            if ( ds.contains(aliasElt) )
            {
               broker.remove(mProxyInfo.getContext(), aliasElt, ds);
               return;
            }
         }

         System.err.println("ERROR: Failed to find the data source that " +
                            "contains " + aliasElt.getName());
      }

      private transient CellView mView = null;
      private transient ProxyInfo mProxyInfo = null;

      private TableLayout mMainLayout = null;
      private JButton mAddAliasButton = new JButton();
      private JButton mPasteAliasButton = new JButton();
   }
}
