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
package org.vrjuggler.vrjconfig.controlpanel.io;

import java.util.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import org.jdom.*;
import org.jdom.transform.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.controlpanel.*;

/**
 * This object knows how to create a configuration definition (all versions of
 * it) from an XML document.
 */
public class ControlPanelParser
{
   public ControlPanelParser()
   {}

   /**
    * Parses the given XML document in order to load a ControlPanel.
    *
    * @param doc           the XML document to parse
    */
   public DefaultMutableTreeNode parse(Document doc)
      throws ParseException
   {
      return(parseBaseControlPanel(doc.getRootElement()));
   }

   /**
    * Parse the root XML node.
    *
    * @param root       the root XML node
    */
   private DefaultMutableTreeNode parseBaseControlPanel(Element root)
      throws ParseException
   {
      // Verify that the root element is a definition
      if (!root.getName().equals(CONTROL_PANEL))
      {
         throw new ParseException("Root element must be: " + CONTROL_PANEL);
      }

      // Verify that the base control_panel has a title
      String title = root.getAttributeValue(TITLE);
      if (title == null)
      {
         throw new MissingAttributeException(TITLE);
      }

      CategoryNode tree_root = new CategoryNode("/", title, null);
      // Parse initial list of control panel items.
      parseControlPanel(root, tree_root);
      return tree_root;
   }

   /**
    * Helper function to recursively parse control panel items.
    *
    * @param root       the current root XML for recursion
    * @param tree_root  the current root TreeNode
    */
   private void parseControlPanel(Element root, DefaultMutableTreeNode tree_root)
      throws ParseException
   {
      // Parse all sub-panels.
      for (Iterator itr = root.getChildren().iterator(); itr.hasNext(); )
      {
         Element elt = (Element)itr.next();
         String name = elt.getName();
         
         if(elt.getName().equals(CATEGORY))
         {
            DefaultMutableTreeNode cat = parseCategory(elt);
            tree_root.add(cat);

            parseControlPanel(elt, cat);
         }
         else if(elt.getName().equals(TYPE))
         {
            tree_root.add(parseType(elt));
         }
         else if(elt.getName().equals(EDITOR))
         {
            tree_root.add(parseEditor(elt));
         }
         else if(elt.getName().equals(WIZARD))
         {
            tree_root.add(parseWizard(elt));
         }
      }
   }

   /**
    * Parse a category node.
    */
   private CategoryNode parseCategory(Element root)
      throws ParseException
   {
      if (!root.getName().equals(CATEGORY))
      {
         throw new ParseException("Element must be: " + CATEGORY);
      }
      
      String label = root.getAttributeValue(LABEL);
      if (label == null)
      {
         throw new MissingAttributeException(LABEL);
      }
      
      String title = root.getAttributeValue(TITLE);
      if (title == null)
      {
         throw new MissingAttributeException(TITLE);
      }
      
      String icon_path = root.getAttributeValue(ICON);
      if (icon_path == null)
      {
         throw new MissingAttributeException(ICON);
      }

      Icon icon = null;

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         icon = new ImageIcon(loader.getResource(icon_path));
      }
      catch (Exception ex)
      {
      }

      return(new CategoryNode(label, title, icon));
   }
   
   /**
    * Parse a type node.
    */
   private TypeNode parseType(Element root)
      throws ParseException
   {
      // Verify that the root element is a type node.
      if (!root.getName().equals(TYPE))
      {
         throw new ParseException("Element must be: " + TYPE);
      }
      
      String label = root.getAttributeValue(LABEL);
      if (label == null)
      {
         throw new MissingAttributeException(LABEL);
      }
      
      String title = root.getAttributeValue(TITLE);
      if (title == null)
      {
         throw new MissingAttributeException(TITLE);
      }
 
      String token = root.getAttributeValue(TOKEN);
      if (token == null)
      {
         throw new MissingAttributeException(TOKEN);
      }
      
      String icon_path = root.getAttributeValue(ICON);
      if (icon_path == null)
      {
         throw new MissingAttributeException(ICON);
      }

      Icon icon = null;

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         icon = new ImageIcon(loader.getResource(icon_path));
      }
      catch (Exception ex)
      {
      }

      return(new TypeNode(label, title, token, icon));
   }
   
   /**
    * Parse an editor node.
    */
   private EditorNode parseEditor(Element root)
      throws ParseException
   {
      // Verify that the root element is a editor node.
      if (!root.getName().equals(EDITOR))
      {
         throw new ParseException("Element must be: " + EDITOR);
      }
      
      String label = root.getAttributeValue(LABEL);
      if (label == null)
      {
         throw new MissingAttributeException(LABEL);
      }
      
      String class_name = root.getAttributeValue(CLASS);
      if (class_name == null)
      {
         throw new MissingAttributeException(CLASS);
      }
 
      String icon_path = root.getAttributeValue(ICON);
      if (icon_path == null)
      {
         throw new MissingAttributeException(ICON);
      }

      Icon icon = null;

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         icon = new ImageIcon(loader.getResource(icon_path));
      }
      catch (Exception ex)
      {
      }

      return(new EditorNode(label, class_name, icon));
   }
   
   /**
    * Parse a wizard node.
    */
   private WizardNode parseWizard(Element root)
      throws ParseException
   {
      // Verify that the root element is a wizard element.
      if (!root.getName().equals(WIZARD))
      {
         throw new ParseException("Element must be: " + WIZARD);
      }
      
      String label = root.getAttributeValue(LABEL);
      if (label == null)
      {
         throw new MissingAttributeException(LABEL);
      }
      
      String wizard_location = root.getAttributeValue(WIZARD_LOCATION);
      if (wizard_location == null)
      {
         throw new MissingAttributeException(WIZARD_LOCATION);
      }
 
      String icon_path = root.getAttributeValue(ICON);
      if (icon_path == null)
      {
         throw new MissingAttributeException(ICON);
      }

      Icon icon = null;

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         icon = new ImageIcon(loader.getResource(icon_path));
      }
      catch (Exception ex)
      {
      }

      return(new WizardNode(label, wizard_location, icon));
   }


   /** Constants to make XML structure changes easier to handle. */
   private static final Namespace CP_NS = Namespace.getNamespace("http://www.vrjuggler.org/vrjuggler/xsd/1.0/definition");

   private static final String CONTROL_PANEL    = "control_panel";
   private static final String CATEGORY         = "category";
   private static final String TYPE             = "type";
   private static final String EDITOR           = "editor";
   private static final String WIZARD           = "wizard";
   
   private static final String LABEL            = "label";
   private static final String ICON             = "icon";
   private static final String TITLE            = "title";
   private static final String TOKEN            = "token";
   private static final String CLASS            = "class";
   private static final String WIZARD_LOCATION  = "wizard_location";
}
