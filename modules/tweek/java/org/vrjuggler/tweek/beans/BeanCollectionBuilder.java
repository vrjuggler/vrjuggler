/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

package org.vrjuggler.tweek.beans;

import java.io.File;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;
import javax.swing.tree.*;
import org.jdom.*;
import org.vrjuggler.tweek.beans.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;


/**
 * This class performs the rather complex task of constructing the branches
 * of a BeanTree.  Given a path, it will search the directory for .xml files.
 * If any of those XML files contain a "beantree" document, it is parsed, and
 * a tree of DefaultMutableTreeNode objects is constructed based on the
 * contents.  Subsequent "beantree" documents add to the tree, and the tree is
 * updated automatically to reflect these additions.
 *
 * @version $Revision$
 * @since 1.0
 */
public class BeanCollectionBuilder
{
   public static BeanCollectionBuilder instance ()
   {
      if ( m_instance == null )
      {
         m_instance = new BeanCollectionBuilder();
      }

      return m_instance;
   }

   /**
    * Constructs the collection of known JavaBeans.  If there are no known
    * Beans, a new collection is created.  Otherwise, the existing collection
    * is extended to include the newly discovered Beans.
    */
   public void build (String path, boolean is_directory)
      throws BeanPathException
   {
      File[] files = null;

      if ( is_directory )
      {
         FileFinder finder = new FileFinder(".xml", path);
         files = finder.getFiles();
      }
      else
      {
         files    = new File[1];
         files[0] = new File(path);

         if ( ! files[0].isFile() )
         {
            throw new BeanPathException(path + " is not a file");
         }
      }

      extend(files);
   }

   public BeanTreeModel getPanelTree ()
   {
      return m_panel_tree;
   }

   public Vector getServices ()
   {
      return m_services;
   }

   public Vector getViewers ()
   {
      return m_viewers;
   }

   public Vector getGenericBeans ()
   {
      return m_generic_beans;
   }

   // =========================================================================
   // Protected methods.
   // =========================================================================

   /**
    * Default constructor.
    */
   protected BeanCollectionBuilder ()
   {
   }

   protected static BeanCollectionBuilder m_instance = null;

   protected void extend (File[] files)
   {
      org.jdom.input.SAXBuilder builder = new org.jdom.input.SAXBuilder();

      for ( int i = 0; i < files.length; i++ )
      {
         try
         {
            Document document = builder.build(files[i]);
            Element root      = document.getRootElement();

            // If the root element is a <beantree>, proceed.  Otherwise, this
            // XML document is not one about which we care.
            if ( root.getName().equals("beanlist") )
            {
               manageChildren(root.getChildren());
            }
         }
         catch (org.jdom.JDOMException e)
         {
            e.printStackTrace();
         }
      }
   }

   /**
    * Iterates over the given list of XML Elements (children of some parent
    * Element) and processes each.
    */
   protected void manageChildren (List children)
   {
      Iterator i = children.iterator();

      DefaultMutableTreeNode root =
         (DefaultMutableTreeNode) m_panel_tree.getRoot();

      while ( i.hasNext() )
      {
         Element child = (Element) i.next();
         String name   = child.getName();

         // The current child is a <bean></bean> block.
         if ( name.equals("guipanel") )
         {
            handleGuiPanel(child, root);
         }
         else if ( name.equals("service") )
         {
            handleService(child);
         }
         else if ( name.equals("viewer") )
         {
            handleViewer(child);
         }
         else if ( name.equals("generic") )
         {
            handleGeneric(child);
         }
         else
         {
            System.err.println("WARNING: Unknown child type <" + name + ">");
         }
      }
   }

   /**
    * Processes a <guipanel></guipanel> block of the beanlist document.
    */
   protected void handleGuiPanel (Element bean, DefaultMutableTreeNode root)
   {
      BeanAttributes attrs = new BeanAttributes();
      attrs.parse(bean);

      String name     = attrs.getName();
      String jar_file = attrs.getJarFile();
      String entry    = attrs.getEntry();
      Vector deps     = attrs.getDependencies();

      // The rest are specific to <guipanel>.
      String icon_path = null;
      String tool_tip  = null;
      Vector tree_path = new Vector();

      List children = bean.getChildren();
      Iterator i    = children.iterator();

      while ( i.hasNext() )
      {
         Element e = (Element) i.next();

         // <tree /> element.
         if ( e.getName().equals("tree") )
         {
            tree_path.add(e.getAttribute("path").getValue());
         }
         // <icon /> element.
         else if ( e.getName().equals("icon") )
         {
            icon_path = expandEnvVars(e.getAttribute("source").getValue());
            tool_tip  = expandEnvVars(e.getAttribute("tooltip").getValue());
         }
      }

      DefaultMutableTreeNode node = null;
      System.out.println("Working on " + jar_file);

      node = new DefaultMutableTreeNode(new PanelBean(jar_file, name, icon_path,
                                                      tool_tip, entry, deps));

      for ( int j = 0; j < tree_path.size(); j++ )
      {
         String cur_path = (String) tree_path.elementAt(j);
         addTreePath(m_panel_tree, cur_path, root, node);
      }
   }

   /**
    * Extends the given tree model to include the path represented by the
    * <code>path</code> parameter.  The path is rooted at the given root, and
    * the new node is added at as a leaf of the path.
    */
   protected void addTreePath (DefaultTreeModel tree_model, String path,
                               DefaultMutableTreeNode root,
                               DefaultMutableTreeNode new_node)
   {
      if ( path.startsWith("/") )
      {
         String work_path = path;

         DefaultMutableTreeNode cur_node = root;

         int begin_index = 1;
         int end_index   = path.indexOf('/', 1);

         while ( end_index != -1 || begin_index < path.length() )
         {
            if ( end_index == -1 )
            {
               end_index = path.length();
            }

            String path_element = path.substring(begin_index, end_index);

            java.util.Enumeration children = cur_node.children();
            boolean found = false;

            while ( children.hasMoreElements() )
            {
               DefaultMutableTreeNode node =
                  (DefaultMutableTreeNode) children.nextElement();

               try
               {
                  String level = (String) node.getUserObject();

                  if ( level.equals(path_element) )
                  {
                     cur_node = node;
                     found    = true;
                     break;
                  }
               }
               // This will happen if the current child is actually a BeanPanel
               // object.  Perhaps there is a better way to deal with this?
               catch (ClassCastException e)
               {
                  /* Move on. */ ;
               }
            }

            if ( ! found )
            {
               DefaultMutableTreeNode n = new DefaultMutableTreeNode(path_element);
               int[] indices = new int[1];
               indices[0] = cur_node.getChildCount();
               cur_node.add(n);
               tree_model.nodesWereInserted(cur_node, indices);
               cur_node = n;
            }

            begin_index = end_index + 1;
            end_index   = path.indexOf('/', begin_index);
         }

         int[] indices = new int[1];
         indices[0] = cur_node.getChildCount();
         cur_node.add(new_node);
         tree_model.nodesWereInserted(cur_node, indices);
      }
      else
      {
         System.err.println("WARNING: Invalid tree path '" + path + "'");
      }
   }

   /**
    * Processes a <service></service> block of the beanlist document.
    */
   protected void handleService (Element service)
   {
      BeanAttributes attrs = new BeanAttributes();
      attrs.parse(service);

      String name     = attrs.getName();
      String jar_file = attrs.getJarFile();
      String entry    = attrs.getEntry();
      Vector deps     = attrs.getDependencies();

      m_services.add(new ServiceBean(name, jar_file, entry, deps));
   }

   /**
    * Processes a <viewer></viewer> block of the beanlist document.
    */
   protected void handleViewer (Element viewer)
   {
      BeanAttributes attrs = new BeanAttributes();
      attrs.parse(viewer);

      String name     = attrs.getName();
      String jar_file = attrs.getJarFile();
      String entry    = attrs.getEntry();
      Vector deps     = attrs.getDependencies();

      m_viewers.add(new ViewerBean(name, jar_file, entry, deps));
   }

   /**
    * Processes a <viewer></viewer> block of the beanlist document.
    */
   protected void handleGeneric (Element gen_bean)
   {
      BeanAttributes attrs = new BeanAttributes();
      attrs.parse(gen_bean);

      String name     = attrs.getName();
      String jar_file = attrs.getJarFile();
      String entry    = attrs.getEntry();
      Vector deps     = attrs.getDependencies();

      m_generic_beans.add(new GenericBean(name, jar_file, entry, deps));
   }

   /**
    * Expand environment variables found within XML files as we know how.
    * This must exist here because the EnvironmentService is a JavaBean, and
    * it is this class' responsibility to *find* JavaBeans.
    *
    * @post Any environment variables known to the JVM as properties are
    *       expanded.  Unknown variables are left intact.
    */
   protected String expandEnvVars (String name)
   {
      String new_name = name;
      int index, end_index;
      String env_var, value;

      while ( (index = new_name.indexOf("$")) != -1 )
      {
         if ( name.substring(index, index + 2).equals("$(") )
         {
            end_index = name.indexOf(")");
         }
         else if ( name.substring(index, index + 2).equals("${") )
         {
            end_index = name.indexOf("}");
         }
         else
         {
            continue;
         }

         env_var  = name.substring(index + 2, end_index);
         value    = System.getProperty(env_var);

         if ( value != null )
         {
            new_name = name.substring(0, index) + value +
                       name.substring(end_index + 1);
         }
         else
         {
            System.err.println("WARNING: Environment variable " + env_var +
                               " has no value");
            new_name = name.substring(0, index) + name.substring(end_index + 1);
         }
      }

      return new_name;
   }

   // =========================================================================
   // Protected classes.
   // =========================================================================

   /**
    * Simple class for parsing and collecting the common contents of <beanlist>
    * document elements.  These include the Bean name, the Bean dependencies,
    * and the file containing the Bean.
    */
   protected class BeanAttributes
   {
      /**
       * Parse the given XML Element looking for the children <dependency />,
       * <path />, and <file />.
       */
      public void parse (Element root)
      {
         name = root.getAttribute("name").getValue();

         List children = root.getChildren();
         Iterator i    = children.iterator();

         while ( i.hasNext() )
         {
            Element e = (Element) i.next();

            if ( e.getName().equals("dependency") )
            {
               String dep_file = null;

               if ( e.getAttribute("file") != null )
               {
                  dep_file = e.getAttribute("file").getValue();
               }

               String dep_path = null;

               if ( e.getAttribute("path") != null )
               {
                  dep_path = expandEnvVars(e.getAttribute("path").getValue());
               }

               dependencies.add(new BeanDependency(dep_file, dep_path));
            }
            else if ( e.getName().equals("file") )
            {
               jarFile = expandEnvVars(e.getAttribute("name").getValue());
               entry   = e.getAttribute("entry").getValue();
            }
         }

         if ( ! jarFile.startsWith("http://") && ! jarFile.startsWith("ftp://") )
         {
            // Prepend "file:" onto jar_file if it is not already present.
            if ( ! jarFile.startsWith("file:") )
            {
               jarFile = "file:" + jarFile;
            }
         }
      }

      public String getName ()
      {
         return name;
      }

      public String getJarFile ()
      {
         return jarFile;
      }

      public String getEntry ()
      {
         return entry;
      }

      public Vector getDependencies ()
      {
         return dependencies;
      }

      private String name         = null;
      private String jarFile      = null;
      private String entry        = null;
      private Vector dependencies = new Vector();
   }

   // =========================================================================
   // Protected data members.
   // =========================================================================

   protected BeanTreeModel m_panel_tree    = new BeanTreeModel(new DefaultMutableTreeNode());
   protected Vector        m_services      = new Vector();
   protected Vector        m_viewers       = new Vector();
   protected Vector        m_generic_beans = new Vector();
}
