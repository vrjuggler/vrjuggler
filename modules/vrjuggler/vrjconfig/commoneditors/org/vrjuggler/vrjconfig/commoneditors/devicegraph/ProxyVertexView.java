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
import java.awt.Frame;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.datatransfer.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.MouseEvent;
import java.util.Iterator;
import java.util.Map;
import javax.swing.*;
import javax.swing.border.Border;

import org.jgraph.JGraph;
import org.jgraph.graph.AttributeMap;
import org.jgraph.graph.CellView;
import org.jgraph.graph.CellViewRenderer;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.VertexView;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.DataFlavorRepository;
import org.vrjuggler.jccl.editors.TransferableConfigElement;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;


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

   private static class ProxyVertexRenderer
      extends JPanel
      implements CellViewRenderer
               , ClipboardOwner
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

      private static String IMG_ROOT =
         "org/vrjuggler/vrjconfig/commoneditors/images";

      public ProxyVertexRenderer()
      {
         mDefaultForeground = UIManager.getColor("Tree.textForeground");
         mDefaultBackground = UIManager.getColor("Tree.textBackground");

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
         mNameLabel.setText(" ");

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
                  Dimension row_size = addAliasRow(elt);

                  ProxyVertexRenderer r = ProxyVertexRenderer.this;
                  Dimension pref_size = r.getPreferredSize();

                  int width = Math.max(pref_size.width, row_size.width);
                  int height = pref_size.height + row_size.height;

                  r.setPreferredSize(new Dimension(width, height));
                  r.revalidate();
                  r.repaint();
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
                        Dimension row_size = addAliasRow(alias_elt);

                        ProxyVertexRenderer r = ProxyVertexRenderer.this;

                        Dimension pref_size = r.getPreferredSize();

                        int width = Math.max(pref_size.width, row_size.width);
                        int height = pref_size.height + row_size.height;
                        r.setPreferredSize(new Dimension(width, height));

                        // Manually disable all the "Paste" buttons in the
                        // proxy vertex renderers.
                        // XXX: It would be better if this would happen
                        // automatically upon reading the contents of the
                        // clipboard.
                        setAllPasteButtonsEnabled(r.graph, false);

                        r.revalidate();
                        r.repaint();
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

         this.add(mNameLabel,
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

               ConfigElement proxy_elt = mProxyInfo.getElement();

               int alias_width = 0, alias_height = 0;

               for ( Iterator i = mProxyInfo.getAliases().iterator();
                     i.hasNext(); )
               {
                  Dimension row_size = addAliasRow((ConfigElement) i.next());

                  alias_width   = Math.max(alias_width, row_size.width);
                  alias_height += row_size.height;
               }

               this.revalidate();

               Dimension label_size = mNameLabel.getPreferredSize();
               Dimension add_size   = mAddAliasButton.getPreferredSize();
               Dimension paste_size = mPasteAliasButton.getPreferredSize();
               int width = Math.max(alias_width,
                                    Math.max(label_size.width,
                                             Math.max(add_size.width,
                                                      paste_size.width)));
               int min_height = label_size.height + add_size.height +
                                   paste_size.height + 10;
               int pref_height = min_height + alias_height;
               setMinimumSize(new Dimension(width, min_height));
               setPreferredSize(new Dimension(width * 2, pref_height));
            }
            catch (Exception ex)
            {
               ex.printStackTrace();
            }
         }

         if ( graph.getEditingCell() != view.getCell() )
         {
            mNameLabel.setText(mProxyInfo.getElement().getName());
         }
         else
         {
            mNameLabel.setText(" ");
         }

         return this;
      }

      public void paint(Graphics g)
      {
         if ( gradientColor != null && ! preview )
         {
            setOpaque(false);
            Graphics2D graphics = (Graphics2D) g;
            graphics.setPaint(new GradientPaint(0, 0, getBackground(),
                                                getWidth(), getHeight(),
                                                gradientColor, true));
            graphics.fillRect(0, 0, getWidth(), getHeight());
         }

         super.paint(g);
         paintSelectionBorder(g);
      }

      private void paintSelectionBorder(Graphics g)
      {
         ((Graphics2D) g).setStroke(GraphConstants.SELECTION_STROKE);

         if ( hasFocus && selected )
         {
            g.setColor(graph.getLockedHandleColor());
         }
         else if ( selected )
         {
            g.setColor(graph.getHighlightColor());
         }

         if ( selected )
         {
            Dimension d = getSize();
            g.drawRect(0, 0, d.width - 1, d.height - 1);
         }
      }

      public void lostOwnership(Clipboard clipboard, Transferable contents)
      {
      }

      private void installAttributes(JGraph graph, Map attributes)
      {
         setOpaque(GraphConstants.isOpaque(attributes));

         mNameLabel.setIcon(GraphConstants.getIcon(attributes));
         mNameLabel.setVerticalAlignment(
            GraphConstants.getVerticalAlignment(attributes)
         );
         mNameLabel.setHorizontalAlignment(
            GraphConstants.getHorizontalAlignment(attributes)
         );
         mNameLabel.setVerticalTextPosition(
            GraphConstants.getVerticalTextPosition(attributes)
         );
         mNameLabel.setHorizontalTextPosition(
            GraphConstants.getHorizontalTextPosition(attributes)
         );

         Color foreground = GraphConstants.getForeground(attributes);

         if ( foreground == null )
         {
            setForeground(mDefaultForeground);
         }
         else
         {
            setForeground(foreground);
         }

         Color background = GraphConstants.getBackground(attributes);

         if ( background == null )
         {
            setBackground(mDefaultBackground);
         }
         else
         {
            setForeground(background);
         }

         java.awt.Font font = GraphConstants.getFont(attributes);

         if ( font == null )
         {
            mNameLabel.setFont(graph.getFont());
         }
         else
         {
            mNameLabel.setFont(font);
         }

         Border border      = GraphConstants.getBorder(attributes);
         Color border_color = GraphConstants.getBorderColor(attributes);

         if ( border != null )
         {
            setBorder(border);
         }
         else if ( border_color != null )
         {
            int border_width =
               Math.max(1, Math.round(GraphConstants.getLineWidth(attributes)));
            setBorder(BorderFactory.createLineBorder(border_color,
                                                     border_width));
         }

         gradientColor = GraphConstants.getGradientColor(attributes);
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
       * the proxy being rendered.
       *
       * @return The preferred size of the new row (as the sum of its
       *         components) is returned.
       */
      private Dimension addAliasRow(final ConfigElement aliasElt)
      {
         final ConfigContext ctx = mProxyInfo.getContext();

         final JLabel name_field = new JLabel(aliasElt.getName());
         name_field.setFont(new Font("Dialog", Font.ITALIC,
                                      mNameLabel.getFont().getSize()));
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

         final int row = mMainLayout.getNumRow();

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
                  Frame parent =
                     (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
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
                  removeAliasRow(components, row);
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
                  removeAliasRow(components, row);
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

         int width = name_field.getPreferredSize().width +
                        edit_btn.getPreferredSize().width +
                        cut_btn.getPreferredSize().width +
                        remove_btn.getPreferredSize().width;
         int height = name_field.getPreferredSize().height;
         return new Dimension(width, height);
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

      private void removeAliasRow(JComponent[] components, int row)
      {
         for ( int i = 0; i < components.length; ++i )
         {
            this.remove(components[i]);
         }

         mMainLayout.deleteRow(row);
         this.revalidate();
         this.repaint();
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

      private transient JGraph graph        = null;
      private transient Color gradientColor = null;
      private transient boolean hasFocus    = false;
      private transient boolean selected    = false;
      private transient boolean preview     = false;

      private transient ProxyInfo mProxyInfo = null;

      private Color mDefaultBackground;
      private Color mDefaultForeground;
      private TableLayout mMainLayout = null;
      private JLabel mNameLabel = new JLabel();
      private JButton mAddAliasButton = new JButton();
      private JButton mPasteAliasButton = new JButton();
   }
}
