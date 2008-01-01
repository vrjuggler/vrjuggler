/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import org.vrjuggler.jccl.config.*;


/**
 * A simple component for managing the editing of device proxies.  It does not
 * provide a means to change the relationships of proxies to devices and hence
 * should not be confused with <code>DeviceProxyGraphEditor</code> or
 * <code>ProxyEditorUI</code>, both of which are based on
 * <a href="http://www.jgraph.com/">JGraph</a>.  This editor uses standard
 * Swing components and provides an interface for editing the properties of
 * proxy config elements.
 */
public class SimpleProxyEditor
   extends JSplitPane
   implements EditorConstants
{
   public SimpleProxyEditor()
   {
      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
   }

   public void setConfig(ConfigContext ctx, ConfigElement deviceElt,
                         ConfigDefinition proxyType)
   {
      mContext   = ctx;
      mDeviceElt = deviceElt;
      mProxyDef  = proxyType;

      ConfigBroker broker = new ConfigBrokerProxy();
      List elts = broker.getElements(ctx);
      List proxy_elts = new ArrayList();

      Class editor_class = pickProxyTypeEditor();
      DefaultListModel model = new DefaultListModel();

      for ( Iterator i = elts.iterator(); i.hasNext(); )
      {
         ConfigElement e = (ConfigElement) i.next();

         if ( e.getDefinition().equals(proxyType) )
         {
            ConfigElementPointer proxied_dev =
               (ConfigElementPointer) e.getProperty("device", 0);

            if ( proxied_dev.getTarget().equals(deviceElt.getName()) )
            {
               ProxyElement proxy_elt = new ProxyElement(e);

               try
               {
                  ProxyTypeEditor editor =
                     (ProxyTypeEditor) editor_class.newInstance();

                  editor.setConfig(ctx, e);
                  proxy_elt.setPanel(editor);

                  // This proxy config element will only be used by the editor
                  // if its editor was successfully created and configured.
                  // Otherwise, there is no point in holding onto it.
                  proxy_elts.add(proxy_elt);
                  model.addElement(proxy_elt);
               }
               catch(Exception ex)
               {
                  ex.printStackTrace();
               }
            }
         }
      }

      for ( Iterator i = proxy_elts.iterator(); i.hasNext(); )
      {
         ProxyElement e = (ProxyElement) i.next();
         e.findAliases(elts);
      }

      mProxyList.setModel(model);
   }

   private void jbInit()
      throws Exception
   {
      mProxyListPanel.setLayout(mProxyListPanelLayout);
      mProxyAddButton.setText("Add");
      mProxyAddButton.addActionListener(
         new SimpleProxyEditor_mProxyAddButton_actionAdapter(this)
      );
      mProxyRemoveButton.setEnabled(false);
      mProxyRemoveButton.setText("Remove");
      mProxyRemoveButton.addActionListener(
         new SimpleProxyEditor_mProxyRemoveButton_actionAdapter(this)
      );
      mProxyList.addListSelectionListener(
         new SimpleProxyEditor_mProxyList_listSelectionAdapter(this)
      );
      mProxyEditorPanel.setLayout(mProxyEditorPanelLayout);
      mSourceLabel.setLabelFor(mInputSourceField);
      mSourceLabel.setText("Source #");
      mSourcePanel.setLayout(mSourcePanelLayout);
      mInputSourceField.setHorizontalAlignment(SwingConstants.TRAILING);
      mInputSourceField.setMinimumSize(new Dimension(30, 22));
      mInputSourceField.setPreferredSize(new Dimension(40, 22));
      mInputSourceField.addActionListener(
         new SimpleProxyEditor_mInputSourceField_actionAdapter(this)
      );
      mProxyEditorPanel.setBorder(mProxyEditorPanelTitle);
      mProxyEditorPanel.setMinimumSize(new Dimension(150, 56));
      mProxyEditorPanel.setPreferredSize(new Dimension(150, 56));
      mProxyListPanel.setBorder(new TitledBorder(BorderFactory.
                                                 createEtchedBorder(Color.white,
                                                                    new Color(140, 140, 140)),
                                                 "Proxies"));
      this.setDividerSize(4);
      mProxyButtonPanel.add(mProxyAddButton);
      mProxyButtonPanel.add(mProxyRemoveButton);
      mProxyListPanel.add(mProxyListPane, java.awt.BorderLayout.CENTER);
      this.add(mProxyEditorPanel, JSplitPane.RIGHT);
      mProxyEditorPanel.add(mSourcePanel, java.awt.BorderLayout.NORTH);
      mProxyListPane.getViewport().add(mProxyList);
      mSourcePanel.add(mSourceLabel,
                       new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                              GridBagConstraints.EAST,
                                              GridBagConstraints.NONE,
                                              new Insets(0, 3, 0, 3), 0, 0));
      mSourcePanel.add(mInputSourceField,
                       new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0,
                                              GridBagConstraints.WEST,
                                              GridBagConstraints.NONE,
                                              new Insets(3, 0, 3, 3), 0, 0));
      mProxyListPanel.add(mProxyButtonPanel, java.awt.BorderLayout.NORTH);
      this.add(mProxyListPanel, JSplitPane.LEFT);
   }

   private Class pickProxyTypeEditor()
   {
      Class editor_class;

      // This panel will be a position proxy editor.
      if ( mProxyDef.getToken().equals(POSITION_PROXY_TYPE) )
      {
         editor_class = PositionProxyEditor.class;
      }
      // This panel will be an analog or a digital proxy editor.
      else if ( mProxyDef.getToken().equals(ANALOG_PROXY_TYPE) ||
                mProxyDef.getToken().equals(DIGITAL_PROXY_TYPE) )
      {
         editor_class = DefaultProxyTypeEditor.class;
      }
      else
      {
         throw new IllegalArgumentException("Unsupported proxy type '" +
                                            mProxyDef.getToken() + "'");
      }

      return editor_class;
   }

   private ConfigContext    mContext;
   private ConfigElement    mDeviceElt;
   private ConfigDefinition mProxyDef;
   private JComponent       mCurEditor = null;

   private JPanel mProxyListPanel = new JPanel();
   private JPanel mProxyEditorPanel = new JPanel();
   private JPanel mProxyButtonPanel = new JPanel();
   private JScrollPane mProxyListPane = new JScrollPane();
   private JButton mProxyAddButton = new JButton();
   private JButton mProxyRemoveButton = new JButton();
   private JList mProxyList = new JList();
   private GridBagLayout mMainLayout = new GridBagLayout();
   private BorderLayout mProxyEditorPanelLayout = new BorderLayout();
   private JPanel mSourcePanel = new JPanel();
   private JLabel mSourceLabel = new JLabel();
   private JFormattedTextField mInputSourceField = new JFormattedTextField();
   private GridBagLayout mSourcePanelLayout = new GridBagLayout();
   private TitledBorder mProxyEditorPanelTitle = new TitledBorder("Properties");
   private BorderLayout mProxyListPanelLayout = new BorderLayout();

   /**
    * Swaps the proxy editor panel being displayed.
    *
    * @param listSelectionEvent ListSelectionEvent
    */
   void mProxyList_valueChanged(ListSelectionEvent listSelectionEvent)
   {
      if ( mCurEditor != null )
      {
         mProxyEditorPanel.remove(mCurEditor);
      }

      ProxyElement proxy = (ProxyElement) mProxyList.getSelectedValue();

      if ( proxy != null )
      {
         mProxyRemoveButton.setEnabled(true);

         ConfigElement elt = proxy.getProxyElt();
         mInputSourceField.setValue(elt.getProperty("unit", 0));
         mProxyEditorPanelTitle.setTitle(elt.getName() + " Properties");
         mCurEditor = (JComponent) proxy.getPanel();
         mProxyEditorPanel.add(mCurEditor, BorderLayout.CENTER);
         mProxyEditorPanel.revalidate();
         mProxyEditorPanel.repaint();
      }
      else
      {
         mProxyRemoveButton.setEnabled(false);
         mCurEditor = null;
      }
   }

   void mProxyAddButton_actionPerformed(ActionEvent actionEvent)
   {
      Window parent =
         (Window) SwingUtilities.getAncestorOfClass(Window.class, this);

      String alias_name =
         JOptionPane.showInputDialog(parent,
                                     "Enter a name for the new input source",
                                     "New Input Source Name",
                                     JOptionPane.QUESTION_MESSAGE);

      if ( alias_name != null )
      {
         String proxy_name = alias_name + " Proxy";
         ConfigBroker broker = new ConfigBrokerProxy();
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());

         ConfigElement proxy = factory.create(proxy_name, mProxyDef);
         proxy.setProperty("device", 0, mDeviceElt.getName(), mContext);

         // If we are working with a position proxy, we have to fill in a
         // default position transform filter.  It's too bad that this has to
         // be here.
         if ( mProxyDef.getToken().equals(POSITION_PROXY_TYPE) )
         {
            ConfigDefinition filter_def =
               broker.getRepository().get(POSITION_TRANSFORM_FILTER_TYPE);
            ConfigElement xform_filter = factory.create("Transform Filter 0",
                                                        filter_def);

            // Add the new config element to the position_filters property.
            proxy.addProperty(POSITION_FILTERS_PROPERTY, xform_filter);
         }

         broker.add(mContext, proxy);

         ConfigDefinition alias_def = broker.getRepository().get("alias");
         ConfigElement alias = factory.create(alias_name, alias_def);
         alias.setProperty("proxy", 0, proxy_name, mContext);
         broker.add(mContext, alias);

         try
         {
            Class editor_class = pickProxyTypeEditor();
            ProxyElement proxy_elt = new ProxyElement(proxy);

            ProxyTypeEditor editor =
               (ProxyTypeEditor) editor_class.newInstance();

            editor.setConfig(mContext, proxy);
            proxy_elt.setPanel(editor);
            proxy_elt.addAlias(alias);

            // This proxy config element will only be used by the editor
            // if its editor was successfully created and configured.
            // Otherwise, there is no point in holding onto it.
            ((DefaultListModel) mProxyList.getModel()).addElement(proxy_elt);

            // Make sure to select the new proxy.
            mProxyList.setSelectedValue(proxy_elt, true);
         }
         catch(Exception ex)
         {
            ex.printStackTrace();
         }
      }
   }

   void mProxyRemoveButton_actionPerformed(ActionEvent actionEvent)
   {
      ProxyElement cur_proxy = (ProxyElement) mProxyList.getSelectedValue();

      // Remove the selected proxy from the GUI first.
      mProxyEditorPanel.remove(mCurEditor);
      mCurEditor = null;
      mProxyEditorPanel.revalidate();
      mProxyEditorPanel.repaint();

      mProxyList.clearSelection();
      ((DefaultListModel) mProxyList.getModel()).removeElement(cur_proxy);
      mInputSourceField.setText("");

      // Then remove it and all its aliases from the config context.
      ConfigBroker broker = new ConfigBrokerProxy();
      List aliases = cur_proxy.getAliases();

      for ( Iterator i = aliases.iterator(); i.hasNext(); )
      {
         broker.remove(mContext, (ConfigElement) i.next());
      }

      broker.remove(mContext, cur_proxy.getProxyElt());
   }

   void mInputSourceField_actionPerformed(ActionEvent actionEvent)
   {
      ProxyElement proxy = (ProxyElement) mProxyList.getSelectedValue();
      ConfigElement elt = proxy.getProxyElt();
      elt.setProperty("unit", 0, mInputSourceField.getValue(), mContext);
   }
}

class SimpleProxyEditor_mInputSourceField_actionAdapter
   implements ActionListener
{
   private SimpleProxyEditor adaptee;
   SimpleProxyEditor_mInputSourceField_actionAdapter(SimpleProxyEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mInputSourceField_actionPerformed(actionEvent);
   }
}

class ProxyElement
{
   public ProxyElement(ConfigElement elt)
   {
      proxyElt = elt;
   }

   public ConfigElement getProxyElt()
   {
      return proxyElt;
   }

   /**
    * Returns the list of all config elements that are aliases (type 'alias')
    * that refer to this proxy.  To populate the list, first invoke
    * findAliases().  The returned list may be empty, but it will not be null.
    *
    * @see findAliases
    */
   public List getAliases()
   {
      return aliases;
   }

   public void addAlias(ConfigElement aliasElt)
   {
      this.aliases.add(aliasElt);
   }

   /**
    * Searches the given list of config elements for aliases that point to
    * this proxy.  When the method returns, calling getAliases() will return
    * all the ConfigElement references that are aliases pointing to this
    * proxy.
    *
    * @param elements The list of elements to search for aliases pointing to
    *                 this proxy.
    *
    * @see getAliases
    */
   public void findAliases(List elements)
   {
      aliases.clear();

      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinition alias_def = broker.getRepository().get("alias");

      for ( Iterator i = elements.iterator(); i.hasNext(); )
      {
         ConfigElement e = (ConfigElement) i.next();

         if ( e.getDefinition().equals(alias_def) )
         {
            ConfigElementPointer proxy_name =
               (ConfigElementPointer) e.getProperty("proxy", 0);

            if ( proxy_name.getTarget().equals(proxyElt.getName()) )
            {
               aliases.add(e);
            }
         }
      }
   }

   public void setPanel(ProxyTypeEditor panel)
   {
      this.panel = panel;
   }

   public ProxyTypeEditor getPanel()
   {
      return panel;
   }

   public String toString()
   {
      String rep;

      if ( aliases.size() == 0 )
      {
         rep = proxyElt.getName();
      }
      else
      {
         Iterator i = aliases.iterator();

         ConfigElement alias = (ConfigElement) i.next();
         rep = alias.getName();

         for ( ; i.hasNext(); )
         {
            alias = (ConfigElement) i.next();
            rep = rep + ", " + alias.getName();
         }

         rep = rep + " \u2192 " + proxyElt.getName();
      }

      return rep;
   }

   private ConfigElement   proxyElt;
   private List            aliases = new ArrayList();
   private ProxyTypeEditor panel;
}

class SimpleProxyEditor_mProxyRemoveButton_actionAdapter
   implements ActionListener
{
   private SimpleProxyEditor adaptee;
   SimpleProxyEditor_mProxyRemoveButton_actionAdapter(SimpleProxyEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mProxyRemoveButton_actionPerformed(actionEvent);
   }
}

class SimpleProxyEditor_mProxyList_listSelectionAdapter
   implements ListSelectionListener
{
   private SimpleProxyEditor adaptee;
   SimpleProxyEditor_mProxyList_listSelectionAdapter(SimpleProxyEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void valueChanged(ListSelectionEvent listSelectionEvent)
   {
      adaptee.mProxyList_valueChanged(listSelectionEvent);
   }
}

class SimpleProxyEditor_mProxyAddButton_actionAdapter
   implements ActionListener
{
   private SimpleProxyEditor adaptee;
   SimpleProxyEditor_mProxyAddButton_actionAdapter(SimpleProxyEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mProxyAddButton_actionPerformed(actionEvent);
   }
}
