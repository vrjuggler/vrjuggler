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

import java.io.*;
import java.util.*;
import org.jdom.*;
import org.jdom.input.SAXBuilder;
import org.jdom.output.XMLOutputter;

/** Representation of a VR Juggler-style configuration file.
 *  Essentially, a ConfigChunkDB is a container for ConfigChunks. It
 *  guarantees that all the ConfigChunks it contains have unique names
 *  and provides a variety of search and access functions, as well as
 *  utilities for checking dependencies between ConfigChunks.
 *
 *  @see ConfigChunk
 *  @see ChunkDescDB
 *  @see ConfigIO
 */
public class ConfigChunkDB
   implements Cloneable
            , ConfigTokens
{
   public static final double CONFIG_VERSION_VALUE = 2.2;

   private static final String CONFIG_VERSION_ATTR = "config.db.version";
   private static final String FILE_ATTR           = "file";

   /**
    * Creates a new ConfigChunk database initially empty and the name
    * "Untitled".
    */
   public ConfigChunkDB()
   {
      super();
      targets = new ArrayList();
      name = "Untitled";

      // Create a new document for this DB.
      mDoc = new Document();
      addVersion(mDoc);
      mDoc.setRootElement(new Element(chunk_db_TOKEN));
   }

   /** Deep copy.
    *  Use this with caution, as any listeners on self will automatically
    *  become listeners on the clone.
    */
   public Object clone()
   {
      try
      {
         ConfigChunkDB db = (ConfigChunkDB)super.clone();
         db.targets = (ArrayList)targets.clone();
         db.mDoc = (Document)mDoc.clone();
         return db;
      }
      catch (CloneNotSupportedException e)
      {
         return null; // not gonna happen
      }
   }

   /** Sets the identifier string for this DB */
   public void setName(String name)
   {
      this.name = name;
   }

   /** Returns the identifier string for this DB */
   public String getName()
   {
      return name;
   }

   /**
    * Builds the database using the given file's contents. If the file contains
    * an XML document with the root type being &lt;ConfigChunkDB&gt;, the
    * children are examined for ConfigChunks. ConfigChunks found in the node's
    * tree are added to this ConfigChunkDB.
    *
    * @param file    the file in which to look for ConfigChunks
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
    * contains an XML document with the root type being &lt;ConfigChunkDB&gt;,
    * the children are examined for ConfigChunks. ConfigChunks found in the
    * node's tree are added to this ConfigChunkDB.
    *
    * @param stream     the stream in which to look for ConfigChunks
    *
    * @throws IOException  if there is an error while building the database
    */
   public void build(InputStream stream)
      throws IOException
   {
      try
      {
         SAXBuilder builder = new SAXBuilder();
         mDoc = builder.build(stream);
      }
      catch (JDOMException e)
      {
         throw new IOException(e.getMessage());
      }
   }

   /**
    * Builds the database from the given XML document. If the document's root
    * element is a ConfigChunkDB, the children are examined for ConfigChunks.
    * ConfigChunks found in the node's tree are added to this ConfigChunkDB.
    *
    * @param document      the XML document in which to look for ConfigChunks
    *
    * @throws IOException  if there is an error while building the database
    */
   public void build(Document document)
      throws IOException
   {
      mDoc = document;
   }

   public void build(Element elem)
      throws IOException
   {
      // XXX: Is this right?
      mDoc.getRootElement().addContent(elem);
   }

   public void write(File file)
      throws IOException
   {
      XMLOutputter outputter = new XMLOutputter("  ", true);
      outputter.setLineSeparator(System.getProperty("line.separator"));

      try
      {
         FileWriter writer = new FileWriter(file);
         outputter.output(mDoc, writer);
         writer.close();
      }
      catch (IOException e)
      {
         e.printStackTrace();
      }
   }

   public void write(OutputStream outStream)
   {
      XMLOutputter outputter = new XMLOutputter("  ", true);
      outputter.setLineSeparator(System.getProperty("line.separator"));

      try
      {
         outputter.output(mDoc, outStream);
      }
      catch (IOException e)
      {
         e.printStackTrace();
      }
   }

   /**
    * Gets the number of ConfigChunks in this database.
    */
   public int size()
   {
      return mDoc.getRootElement().getChildren().size();
   }

   /**
    * Gets the ith ConfigChunk in this database.
    *
    * @param idx     the index of the ConfigChunk to retrieve
    */
   public ConfigChunk get(int idx)
      throws IndexOutOfBoundsException
   {
      Element chunk_elt = (Element)mDoc.getRootElement().getChildren().get(idx);
      return getConfigChunkFor(chunk_elt);
   }

   /** Removed all ConfigChunks from self and notifies ChunkDBListeners. */
   public void clear()
   {
      mDoc.getRootElement().removeChildren();
      notifyChunkDBTargets(new ChunkDBEvent(this, ChunkDBEvent.REMOVEALL, null));
   }

   /** Removes the ConfigChunk at index i and notifies ChunkDBListeners.
    *  @return The object that was removed
    */
   public ConfigChunk remove(int idx)
      throws IndexOutOfBoundsException
   {
      // Remove the chunk
      Element chunk_elt = (Element)mDoc.getRootElement().getChildren().get(idx);
      return remove(chunk_elt);
   }

   /**
    * Removes the ConfigChunk that has the given element.
    *
    * @return  The object that was removed; null if it wasn't found.
    */
   public ConfigChunk remove(Element elt)
   {
      mDoc.getRootElement().removeContent(elt);

      // Notify listeners of the removal
      ConfigChunk chunk = getConfigChunkFor(elt);
      ChunkDBEvent e = new ChunkDBEvent(this, ChunkDBEvent.REMOVE, chunk);
      notifyChunkDBTargets(e);

      return chunk;
   }

   /** Removes the ConfigChunk in self whose name == name.
    *  @return The object that was removed, or null if it wasn't found.
    */
   public ConfigChunk remove(String name)
   {
      // Iterate over the chunk elements looking for the matching config chunk
      for (Iterator itr = mDoc.getRootElement().getChildren().iterator(); itr.hasNext(); )
      {
         Element elt = (Element)itr.next();
         String chunk_name = elt.getAttributeValue("name");
         if (name.equals(chunk_name))
         {
            return remove(elt);
         }
      }
      return null;
   }

   /** Removes a ConfigChunk with the same name as ch.
    *  @return The object that was removed, or null if it wasn't found.
    */
   public ConfigChunk remove(ConfigChunk ch)
   {
      return remove(ch.getName());
   }

   /**
    * Tests if the given ConfigChunk is contained in this database.
    *
    * @param chunk   the chunk to test for
    *
    * @return  true if the chunk is in the database, false otherwise
    */
   public boolean contains(ConfigChunk chunk)
   {
      for (Iterator itr = mDoc.getRootElement().getChildren().iterator(); itr.hasNext(); )
      {
         if (itr.next() == chunk.getNode())
         {
            return true;
         }
      }
      return false;
   }

   /**
    * Find the named ConfigChunk.
    *
    * @return A ConfigChunk in self whose name matches the parameter, or
    *         null if none is found.
    */
   public ConfigChunk get(String name)
   {
      List matches = ConfigUtilities.getChunksWithName(getAll(), name);
      if (matches.size() > 0)
      {
         return (ConfigChunk)matches.get(0);
      }
      return null;
   }

   /**
    * Gets a list of all ConfigChunks in this DB.
    */
   public List getAll()
   {
      List chunks = new ArrayList();
      for (Iterator itr = mDoc.getRootElement().getChildren().iterator(); itr.hasNext(); )
      {
         chunks.add(getConfigChunkFor((Element)itr.next()));
      }
      return chunks;
   }

   /**
    * Inserts ch into self.  Any same-named Chunk already in self is removed.
    * The DOM tree node associated with the given chunk is added as a child
    * of this DB's tree node.
    *
    * @param ch      the chunk to add to this database
    */
   public void add(ConfigChunk ch)
   {
      ConfigChunk insert_chunk;

      // If this chunk has a parent, we need to make a copy of it.  We cannot
      // detach it because it may be coming in from an existing valid
      // chunk database.
      if ( ch.getNode().getParent() != null )
      {
         insert_chunk = new ConfigChunk(ch);

         // The ConfigChunk "copy constructor" makes a duplicate of the
         // internal XML element.  This retains the parent/child relationship
         // already held by the element, so we have to detach it now.
         insert_chunk.getNode().detach();
      }
      // If it has no parent, we'll take it over.
      else
      {
         insert_chunk = ch;
      }

      // As far as I know, JDOM does not allow ordered addition of children,
      // so this just gets added to the end of the list.
      mDoc.getRootElement().addContent(insert_chunk.getNode());

      ChunkDBEvent evt = new ChunkDBEvent(this, ChunkDBEvent.INSERT, insert_chunk);
      notifyChunkDBTargets(evt);
   }

   /**
    * Adds an include URL directive to this DB.
    */
   public void addInclude(String url)
   {
      ProcessingInstruction include = new
         ProcessingInstruction(INCLUDE_INSTRUCTION, new HashMap());

      include.setValue(FILE_ATTR, url);
      mDoc.addContent(include);
   }

   /**
    * Removes an include url directive from this DB.
    */
   public void removeFileInclude(String url)
   {
      List instructions = mDoc.getContent();
      for (Iterator itr = instructions.iterator(); itr.hasNext(); )
      {
         Object next = itr.next();
         if (next instanceof ProcessingInstruction)
         {
            // Found a processing instruction, see if it's of the correct type
            ProcessingInstruction pi = (ProcessingInstruction)next;
            if (pi.getTarget().equals(INCLUDE_INSTRUCTION) ||
                pi.getTarget().equals(INCLUDE_DESC_INSTRUCTION))
            {
               // If this PI is an include for the target file, remove it
               if (pi.getValue(FILE_ATTR).equals(url))
               {
                  mDoc.removeContent(pi);
                  break;
               }
            }
         }
      }
   }

   /**
    * Gets a list of all the URLs included in this DB.
    */
   public List getIncludes()
   {
      List result = new ArrayList();

      List instructions = mDoc.getContent();
      for (Iterator itr = instructions.iterator(); itr.hasNext(); )
      {
         Object next = itr.next();
         if (next instanceof ProcessingInstruction)
         {
            // Found a processing instruction, see if it's of the correct type
            ProcessingInstruction pi = (ProcessingInstruction)next;
            if (pi.getTarget().equals(INCLUDE_INSTRUCTION) ||
                pi.getTarget().equals(INCLUDE_DESC_INSTRUCTION))
            {
               result.add(pi.getValue(FILE_ATTR));
            }
         }
      }

      return result;
   }

   /**
    * Tests if this DB includes the given url.
    */
   public boolean isURLIncluded(String url)
   {
      boolean included = false;

      List instructions = mDoc.getContent();
      for (Iterator itr = instructions.iterator(); itr.hasNext(); )
      {
         Object next = itr.next();
         if (next instanceof ProcessingInstruction)
         {
            // Found a processing instruction, see if it's of the correct type
            ProcessingInstruction pi = (ProcessingInstruction)next;
            if (pi.getTarget().equals(INCLUDE_INSTRUCTION))
            {
               if (pi.getValue(FILE_ATTR).equals(url))
               {
                  included = true;
               }
            }
         }
      }

      return included;
   }

   /**
    * Gets the ConfigChunk associated with the given DOM element. If a
    * ConfigChunk for that element is cached it is returned. Otherwise a new
    * ConfigChunk is created and cached before being returned.
    *
    * @param elt     the element for which to get a ConfigChunk object
    *
    * @return  the ConfigChunk associated with the element
    */
   private ConfigChunk getConfigChunkFor(Element elt)
   {
      ConfigChunk chunk = null;

      // Cache hit
      if (mChunkCache.containsKey(elt))
      {
         chunk = (ConfigChunk)mChunkCache.get(elt);
         System.out.println("Cache hit for: "+elt.getAttributeValue("name")+"; hashcode="+chunk.hashCode());
      }
      // Cache miss
      else
      {
         chunk = new ConfigChunk(elt);
         mChunkCache.put(elt, chunk);
         System.out.println("Cache miss for: "+elt.getAttributeValue("name")+"; hashcode="+chunk.hashCode());
      }

      return chunk;
   }

   /** Generates a name that won't conflict with anything in self.
    *  This generates a new ConfigChunk name, prefixed with root,
    *  that won't conflict with the names of any Chunks already in self.
    *  It does this by appending an integer to the end of the name.
    */
   public String getNewName(String root)
   {
      String name = root;
      int num = 2;
      while (get(name) != null)
      {
         name = root + Integer.toString(num++);
      }
      return name;
   }

   public void fireChunkUpdate(String origName, ConfigChunk updatedChunk)
   {
      notifyChunkDBTargets(new ChunkDBEvent(this, ChunkDBEvent.REPLACE,
                                            origName, updatedChunk));
   }

   public String toString()
   {
      XMLOutputter outputter = new XMLOutputter("   ", true);
      outputter.setLineSeparator(System.getProperty("line.separator"));
      return outputter.outputString(this.mDoc);
   }

   /*-******************* ChunkDB Target Stuff *********************/
   public synchronized void addChunkDBListener(ChunkDBListener l)
   {
      synchronized (targets)
      {
         targets.add (l);
      }
   }

   public void removeChunkDBListener(ChunkDBListener l)
   {
      synchronized (targets)
      {
         targets.remove (targets.indexOf(l));
      }
   }

   protected void notifyChunkDBTargets(ChunkDBEvent e)
   {
      ChunkDBListener[] l = new ChunkDBListener[20];
      synchronized (targets)
      {
         l = (ChunkDBListener[]) targets.toArray(l);
      }
      int a = e.getAction();
      for (int i = 0; (i < l.length) && (l[i] != null); i++)
      {
         if (a == e.INSERT)
         {
            l[i].configChunkAdded(e);
         }
         else if (a == e.REMOVE)
         {
            l[i].configChunkRemoved(e);
         }
         else if (a == e.REPLACE)
         {
            l[i].configChunkReplaced(e);
         }
         else if (a == e.REMOVEALL)
         {
            l[i].configChunksCleared(e);
         }
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
      pi_attrs.put(CONFIG_VERSION_ATTR, String.valueOf(CONFIG_VERSION_VALUE));

      ProcessingInstruction ver =
         new ProcessingInstruction(SETTINGS_INSTRUCTION, pi_attrs);
      doc.addContent(ver);
   }

   /** Targets for ChunkDBEvents. */
   private ArrayList targets;

   /** Identifier for the ConfigChunkDB.
    *  Note: It's generally the case that name will be the last component
    *  of file, but this isn't always true.  VjControl will append tags
    *  to the name in order to distinguish files with the same name
    *  (e.g. "config [1]" and "config [2]").
    */
   private String name;

   /** This is the XML document from which this DB was constructed. */
   protected Document mDoc;

   /**
    * A cache of the ConfigChunks wrapping the DOM elements within this
    * database.
    */
   private Map mChunkCache = new HashMap();
}
