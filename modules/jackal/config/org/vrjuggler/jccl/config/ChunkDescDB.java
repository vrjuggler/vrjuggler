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
package org.vrjuggler.jccl.config;

import java.beans.*;
import java.io.*;
import java.util.*;
import javax.swing.event.EventListenerList;
import org.jdom.*;
import org.jdom.input.SAXBuilder;
import org.jdom.output.XMLOutputter;

/**
 * A container/database of ChunkDescs. This allows a set of ChunkDescs to be
 * grouped together logically.
 */
public class ChunkDescDB
   implements ConfigTokens
            , PropertyChangeListener
            , ChunkDescListener
{
   public static final double DESC_VERSION_VALUE = 2.3;

   private static final String DESC_VERSION_ATTR = "desc.db.version";

   /**
    * Creates a new ChunkDesc database with initially no ChunkDescs contained
    * within it. The name defaults to "Unnamed".
    */
   public ChunkDescDB()
   {
      changeSupport = new PropertyChangeSupport(this);
   }

   /**
    * Sets the name of this ChunkDescDB.
    */
   public final void setName(String name)
   {
      this.name = name;
   }

   /**
    * Gets the name of this ChunkDescDB.
    */
   public final String getName()
   {
      return name;
   }

   /**
    * Builds the database using the given DOM node. If the node's type is
    * &lt;ChunkDescDB&gt;, the children are examined for ChunkDescs. ChunkDescs
    * found in the node's tree are added to this ChunkDescDB.
    *
    * @param elem    the DOM node from which to look for ChunkDescs
    *
    * @throws IOException  if there is an error while building the database
    */
   public void build(Element elem)
      throws IOException
   {
      loadChunkDescs(elem);
   }

   /**
    * Builds the database from the given XML document. If the document's root
    * element is a ChunkDescDB, the children are examined for ChunkDescs.
    * ChunkDescs found in the node's tree are added to this ChunkDescDB.
    *
    * @param document      the XML document in which to look for ChunkDescs
    *
    * @throws IOException  if there is an error while building the database
    */
   public void build(Document document)
      throws IOException
   {
      loadChunkDescs(document.getRootElement());
   }

   /**
    * Builds the database using the given file's contents. If the file contains
    * an XML document with the root type being &lt;ChunkDescDB&gt;, the children
    * are examined for ChunkDescs. ChunkDescs found in the node's tree are added
    * to this ChunkdescDB.
    *
    * @param file    the file from which to look for ChunkDescs
    *
    * @throws IOException  if there is an error while building the database
    */
   public void build(File file)
      throws IOException
   {
      if ( null != file || ! file.canRead() )
      {
         build(new BufferedInputStream(new FileInputStream(file)));
      }
      else
      {
         throw new IOException("No readable file given for input!");
      }
   }

   /**
    * Builds the database using the given stream's contents. If the stream
    * contains an XML document with the root type being &lt;ChunkDescDB&gt;, the
    * children are examined for ChunkDescs. ChunkDescs found in the node's tree
    * are added to this ChunkdescDB.
    *
    * @param stream     the stream in which to look for ChunkDescs
    *
    * @throws IOException  if there is an error while building the database
    */
   public void build(InputStream stream)
      throws IOException
   {
      // build the XML stream into a DOM tree
      try
      {
         SAXBuilder builder = new SAXBuilder();
         Document doc       = builder.build(stream);
         loadChunkDescs(doc.getRootElement());
      }
      catch (JDOMException e)
      {
         throw new IOException(e.getMessage());
      }
   }

   /**
    * Gets the number of ChunkDescs in this database.
    */
   public final synchronized int size()
   {
      return descs.size();
   }

   /**
    * Gets the nth ChunkDesc in this database.
    *
    * @param idx     the index of the ChunkDesc to retrieve
    */
   public final synchronized ChunkDesc get(int idx)
   {
      return (ChunkDesc)descs.get(idx);
   }

   /**
    * Gets a list of all the ChunkDescs in this database.
    */
   public final synchronized List getAll()
   {
      return descs;
   }

   /**
    * Tests if the given ChunkDesc is contained in this database.
    *
    * @param desc    the desc to test for
    *
    * @return  true if the desc is in the database, false otherwise
    */
   public boolean contains(ChunkDesc desc)
   {
      for (Iterator itr = descs.iterator(); itr.hasNext(); )
      {
         if (itr.next() == desc)
         {
            return true;
         }
      }
      return false;
   }

   public void fireDescDBUpdate (String origName, ChunkDesc updatedDesc)
   {
      notifyDescDBTargets(new DescDBEvent(this, DescDBEvent.REPLACE, origName,
                                          updatedDesc));
   }

   /**
    * Removes all ChunkDescs from this database, leaving this database empty.
    */
   public void clear()
   {
      synchronized(this)
      {
         // Make sure we remove ourself as a listener
         for (Iterator itr = descs.iterator(); itr.hasNext(); )
         {
            ChunkDesc desc = (ChunkDesc)itr.next();
            desc.removePropertyChangeListener(this);
            desc.removeChunkDescListener(this);
         }
         // Now we can remove all the descs
         descs.clear();
      }
      notifyDescDBTargets(new DescDBEvent(this, DescDBEvent.REMOVEALL, null));
   }

   /**
    * Removes the given ChunkDesc from this database. If the ChunkDesc is not in
    * this database, nothing is removed.
    *
    * @param desc    the ChunkDesc to remove
    *
    * @return  the ChunkDesc that was removed, null if nothing was removed
    */
   public ChunkDesc remove(ChunkDesc d)
   {
      return removeByToken(d.getToken());
   }

   /**
    * Removes the ChunkDesc from this database that has the given token. If the
    * ChunkDesc is not in this database, nothing is removed.
    *
    * @param tok     the token of the ChunkDesc to remove
    *
    * @return  the ChunkDesc that was removed, null if nothing was removed
    */
   public ChunkDesc removeByToken(String tok)
   {
      ChunkDesc d, retval = null;
      synchronized (this)
      {
         int n = descs.size();
         for (int i = 0; i < n; i++)
         {
            d = (ChunkDesc)descs.get(i);
            if (d.getToken().equals(tok))
            {
               // Remove ourself as a listener
               d.removePropertyChangeListener(this);
               d.removeChunkDescListener(this);
               retval = d;
               descs.remove(i);
               break;
            }
         }
      }
      if (retval != null)
      {
         DescDBEvent e = new DescDBEvent(this, DescDBEvent.REMOVE, retval);
         notifyDescDBTargets(e);
      }
      return retval;
   }

   /**
    * Removes the first ChunkDesc in this database that has the given name. If
    * no such ChunkDescs are in this database, nothing is removed.
    *
    * @param name    the name of the ChunkDesc to remove
    *
    * @return  the ChunkDesc that was removed, null if nothing was removed
    */
   public ChunkDesc removeByName(String name)
   {
      ChunkDesc d, retval = null;
      synchronized (this)
      {
         int n = descs.size();
         for (int i = 0; i < n; i++)
         {
            d = (ChunkDesc)descs.get(i);
            if ( d.getName().equals(name) )
            {
               // Remofe ourself as a listener
               d.removePropertyChangeListener(this);
               d.removeChunkDescListener(this);
               retval = d;
               descs.remove(i);
               break;
            }
         }
      }
      if (retval != null)
      {
         DescDBEvent e = new DescDBEvent(this, DescDBEvent.REMOVE, retval);
         notifyDescDBTargets(e);
      }
      return retval;
   }

   /**
    * Retrieves a list of ChunkDescs whose tokens have the given prefix.
    *
    * @param prefix     the prefix value to test
    *
    * @return  the list of matching ChunkDescs. The list is empty if no
    *          ChunkDescs in this database have the prefix.
    */
   public List getTokenBegins(String prefix)
   {
      ChunkDesc d;
      ArrayList v = new ArrayList();
      ChunkDesc[] da;
      synchronized (this)
      {
         da = new ChunkDesc[descs.size()];
         da = (ChunkDesc[])descs.toArray(da);
      }
      for (int i = 0; i < da.length; i++)
      {
         //d = (ChunkDesc)descs.get(i);
         if ( da[i].getToken().startsWith(prefix) )
         {
            v.add(da[i]);
         }
      }
      return v;
   }

   public final synchronized String getTokenFromName(String _name)
   {
      int n = descs.size();
      for (int i = 0; i < n; i++)
      {
         if ( ((ChunkDesc)descs.get(i)).getName().equals(_name) )
         {
            return ((ChunkDesc)descs.get(i)).getToken();
         }
      }
      return null;
   }

   public final synchronized String getNameFromToken(String tok)
   {
      int n = descs.size();
      for (int i = 0; i < n; i++)
      {
         if ( ((ChunkDesc)descs.get(i)).getToken().equals(tok) )
         {
            return ((ChunkDesc)descs.get(i)).getName();
         }
      }
      return null;
   }

   /**
    * Adds the give chunk description object to this database's internal
    * collection of chunk descriptions.  A chunk description with the same
    * token will be removed before insertion.  The DOM node held by the chunk
    * description is added to this database's DOM tree.
    */
   public void add(ChunkDesc newDesc)
   {
      // Detach from the chunk description from its parent (if it has one).
      newDesc.getNode().detach();

      // Now do the insertion into the database.
      insertDesc(newDesc);
   }

   /**
    * Inserts the given chunk description into the internal database.  The
    * node held by d must already be part of the DOM tree.
    */
   private void insertDesc(ChunkDesc d)
   {
      // remove any desc with the same token
      removeByToken(d.getToken());

      synchronized (this)
      {
         // make sure the name will be unique
         List matches = ConfigUtilities.getDescsWithName(getAll(), d.getName());
         if (matches.size() > 0)
         {
            // this is ugly & clunky, but it's a rarely occurring case,
            // and java (yay!) will handle the memory for us.  If we're
            // changing the name on insert, we need to allocate a new
            // chunkdesc with the new name, so that other chunkdescdbs
            // using this same chunk keep the original name.  since
            // we always replace instead of modifying chunkdescs, this
            // lets the same desc appear in 2 descdbs simultaneously.
            d = (ChunkDesc)d.clone();
            d.setName(generateUniqueName(d.getName()));
         }

         // Make ourself a listener for changes
         d.addPropertyChangeListener(this);
         d.addChunkDescListener(this);

         // Add the ChunkDesc d to the descs list.  We sort them based on their
         // token.
         ChunkDesc cur_desc;
         boolean added = false;

         for ( int i = 0; i < descs.size(); ++i )
         {
            cur_desc = (ChunkDesc) descs.get(i);

            if ( cur_desc.getName().compareTo(d.getName()) > 0 )
            {
               descs.add(i, d);
               added = true;
               break;
            }
         }

         // If the above loop did not insert the desc, then append it.
         if ( ! added )
         {
            descs.add(d);
         }
      }

      DescDBEvent e = new DescDBEvent(this, DescDBEvent.INSERT, d);
      notifyDescDBTargets (e);
   }

   protected String generateUniqueName(String name)
   {
      String n;
      int i = 1;
      do
      {
         n = name + " <" + (i++) + ">";
      }
      while (ConfigUtilities.getDescsWithName(getAll(), n).size() > 0);
      return n;
   }

   public void addAll(List v)
   {
      int n = v.size();
      for (int i = 0; i < n; i++)
      {
         add((ChunkDesc)v.get(i));
      }
   }

   public final void addAll(ChunkDescDB db)
   {
      synchronized (db)
      {
         addAll(db.descs);
      }
   }

   /**
    * Gets the XML representation of this ChunkDescDB as a String.
    */
   public String toString()
   {
      XMLOutputter outputter = new XMLOutputter("   ", true);
      outputter.setLineSeparator(System.getProperty("line.separator"));

      // Construct an XML document to write out.
      Document doc = new Document();
      this.makeDocument(doc);

      return outputter.outputString(doc);
   }

   public void write(File file)
      throws IOException
   {
      Document doc = new Document();
      this.makeDocument(doc);

      XMLOutputter outputter = new XMLOutputter("  ", true);
      FileWriter writer      = new FileWriter(file);

      outputter.setLineSeparator(System.getProperty("line.separator"));
      outputter.output(doc, writer);
      writer.close();
   }

   public void write(OutputStream outStream)
      throws IOException
   {
      Document doc = new Document();
      this.makeDocument(doc);

      XMLOutputter outputter = new XMLOutputter("  ", true);
      outputter.setLineSeparator(System.getProperty("line.separator"));
      outputter.output(doc, outStream);
   }

   /*-******************* DescDB Target Stuff *********************/
   public synchronized void addDescDBListener(DescDBListener l)
   {
      synchronized (targets)
      {
         targets.add(l);
      }
   }

   public void removeDescDBListener(DescDBListener l)
   {
      synchronized (targets)
      {
         targets.remove(targets.indexOf(l));
      }
   }

   protected void notifyDescDBTargets(DescDBEvent e)
   {
      DescDBListener[] l = new DescDBListener[20];

      synchronized (targets)
      {
         l = (DescDBListener[]) targets.toArray(l);
      }

      int a = e.getAction();

      for (int i = 0; (i < l.length) && (l[i] != null); i++)
      {
         if (a == e.INSERT)
         {
            l[i].chunkDescAdded(e);
         }
         else if (a == e.REMOVE)
         {
            l[i].chunkDescRemoved(e);
         }
         else if (a == e.REPLACE)
         {
            l[i].chunkDescReplaced(e);
         }
         else if (a == e.REMOVEALL)
         {
            l[i].chunkDescsCleared(e);
         }
      }
   }

   protected void loadChunkDescs(Element docRoot)
      throws IOException
   {
      String name = docRoot.getName();

      if ( name.equals(chunk_desc_db_TOKEN) )
      {
         Iterator i = docRoot.getChildren().iterator();
         Element desc_elem;

         while ( i.hasNext() )
         {
            desc_elem = (Element) i.next();
            if ( desc_elem.getName().equals(chunk_desc_TOKEN) )
            {
               // We have to detach this node from its parent so that we can
               // create a new root document when writing out the database.
               this.insertDesc(new ChunkDesc((Element) desc_elem.clone()));
            }
            else
            {
            }
         }
      }
      else
      {
         throw new IOException("Unrecognized chunk description element '" +
                               name + "'.");
      }
   }

   private void makeDocument(Document doc)
   {
      addVersion(doc);
      Element root = new Element(chunk_desc_db_TOKEN);
      doc.setRootElement(root);

      ChunkDesc cur_desc;

      Iterator i = descs.iterator();
      while ( i.hasNext() )
      {
         cur_desc = (ChunkDesc) i.next();

         // Ensure that the node isn't attached somewhere else.
         cur_desc.getNode().detach();
         root.addContent(cur_desc.getNode());
      }
   }

   /**
    * Adds the version processing directive to the given Document instance.
    */
   private void addVersion(Document doc)
   {
      // Create a map to hold the attributes for the processing instruction
      // to be created.
      java.util.Map pi_attrs = new java.util.HashMap();

      // Set the version information.
      pi_attrs.put(DESC_VERSION_ATTR, String.valueOf(DESC_VERSION_VALUE));

      ProcessingInstruction ver =
         new ProcessingInstruction(SETTINGS_INSTRUCTION, pi_attrs);
      doc.addContent(ver);
   }

   /**
    * All we do when we find out that a desc has changed is pass it on to our
    * listeners.
    */
   public void propertyChange(PropertyChangeEvent evt)
   {
      System.out.println("ChunkDescDB.propertyChange for property "+evt.getPropertyName());
      changeSupport.firePropertyChange(evt);
   }

   /**
    * Adds the given listener to be notified whenever a ChunkDesc within this
    * database has changed.
    */
   public void addPropertyChangeListener(PropertyChangeListener listener)
   {
      changeSupport.addPropertyChangeListener(listener);
   }

   /**
    * Removes the given listener that was notified whenever a ChunkDesc within
    * this database had changed.
    */
   public void removePropertyChangeListener(PropertyChangeListener listener)
   {
      changeSupport.removePropertyChangeListener(listener);
   }

   /**
    * Adds the given listener to be notified when this ChunkDesc has been
    * modified.
    */
   public void addChunkDescListener(ChunkDescListener listener)
   {
      listenerList.add(ChunkDescListener.class, listener);
   }

   /**
    * Removes the given listener from this ChunkDesc.
    */
   public void removeChunkDescListener(ChunkDescListener listener)
   {
      listenerList.remove(ChunkDescListener.class, listener);
   }

   /**
    * Forwards the given event from the contained chunk descs to listeners of
    * this database.
    */
   public void nameChanged(ChunkDescEvent evt)
   {
      System.out.println("ChunkDescDB.nameChanged");
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            ((ChunkDescListener)listeners[i+1]).nameChanged(evt);
         }
      }
   }

   /**
    * Forwards the given event from the contained chunk descs to listeners of
    * this database.
    */
   public void tokenChanged(ChunkDescEvent evt)
   {
      System.out.println("ChunkDescDB.tokenChanged");
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            ((ChunkDescListener)listeners[i+1]).tokenChanged(evt);
         }
      }
   }

   /**
    * Forwards the given event from the contained chunk descs to listeners of
    * this database.
    */
   public void helpChanged(ChunkDescEvent evt)
   {
      System.out.println("ChunkDescDB.helpChanged");
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            ((ChunkDescListener)listeners[i+1]).helpChanged(evt);
         }
      }
   }

   /**
    * Forwards the given event from the contained chunk descs to listeners of
    * this database.
    */
   public void categoryAdded(ChunkDescEvent evt)
   {
      System.out.println("ChunkDescDB.categoryAdded");
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            ((ChunkDescListener)listeners[i+1]).categoryAdded(evt);
         }
      }
   }

   /**
    * Forwards the given event from the contained chunk descs to listeners of
    * this database.
    */
   public void categoryRemoved(ChunkDescEvent evt)
   {
      System.out.println("ChunkDescDB.categoryRemoved");
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            ((ChunkDescListener)listeners[i+1]).categoryRemoved(evt);
         }
      }
   }

   /**
    * Forwards the given event from the contained chunk descs to listeners of this
    * database.
    */
   public void propertyDescChanged(ChunkDescEvent evt)
   {
      System.out.println("ChunkDescDB.propertyDescChanged");
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            ((ChunkDescListener)listeners[i+1]).propertyDescChanged(evt);
         }
      }
   }

   /**
    * Forwards the given event from the contained chunk descs to listeners of this
    * database.
    */
   public void propertyDescAdded(ChunkDescEvent evt)
   {
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            ((ChunkDescListener)listeners[i+1]).propertyDescAdded(evt);
         }
      }
   }

   /**
    * Forwards the given event from the contained chunk descs to listeners of this
    * database.
    */
   public void propertyDescRemoved(ChunkDescEvent evt)
   {
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            ((ChunkDescListener)listeners[i+1]).propertyDescRemoved(evt);
         }
      }
   }

   /**
    * The name of this ChunkDescDB.
    */
   private String name = "Unnamed";

   private List targets = new ArrayList();
   private List descs   = new ArrayList();

   /**
    * Property change support for notification of listeners that a desc inside
    * this desc db has changed.
    */
   private PropertyChangeSupport changeSupport = null;

   private EventListenerList listenerList = new EventListenerList();
}
