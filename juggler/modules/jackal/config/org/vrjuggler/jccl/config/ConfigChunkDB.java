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
{
   public static final double CONFIG_VERSION_VALUE = 2.0;

   private static final String CONFIG_VERSION_ATTR = "config.db.version";

   /**
    * Creates a new ConfigChunk database initially empty and the name
    * "Untitled".
    */
   public ConfigChunkDB()
   {
      super();
      chunks = new Vector();
      targets = new ArrayList();
      name = "Untitled";
      inputFile = new File("Untitled");

      // Create a new document for this DB.
      mDoc = new Document();
      addVersion(mDoc);
      mDoc.setRootElement(new Element(ConfigTokens.chunk_db_TOKEN));
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
         ConfigChunk ch;
         int i, n = chunks.size();
         db.chunks = new Vector(n);
         for (i = 0; i < n; i++)
         {
            ch = (ConfigChunk)chunks.get(i);
            db.chunks.add (new ConfigChunk(ch));
         }
         return db;
      }
      catch (CloneNotSupportedException e)
      {
         return null; // not gonna happen
      }
   }

   /** Returns the file used for loading/saving */
   public final File getInputFile()
   {
      return inputFile;
   }

   /** Sets the identifier string for this DB */
   public final void setName(String name)
   {
      this.name = name;
   }

   /** Returns the identifier string for this DB */
   public final String getName()
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
   public void build(File inputFile)
      throws IOException
   {
      this.inputFile = inputFile;

      if ( null != inputFile || ! inputFile.canRead() )
      {
         try
         {
            SAXBuilder builder = new SAXBuilder();
            mDoc = builder.build(inputFile);
            loadChunks(mDoc.getRootElement());
         }
         catch (JDOMException e)
         {
            throw new IOException(e.getMessage());
         }
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
      this.inputFile = null;

      try
      {
         SAXBuilder builder = new SAXBuilder();
         mDoc = builder.build(stream);
         loadChunks(mDoc.getRootElement());
      }
      catch (JDOMException e)
      {
         throw new IOException(e.getMessage());
      }
   }

   public void build(Element elem)
      throws IOException
   {
      // XXX: Is this right?
      mDoc.getRootElement().addContent(elem);
      loadChunks(elem);
   }

   public void write()
      throws IOException
   {
      write(inputFile);
   }

   public void write(File file)
      throws IOException
   {
      if ( inputFile != file )
      {
         inputFile = file;
      }

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
    * Populates the internal collection of chunks with ConfigChunk objects
    * created from the children of docRoot.
    */
   private void loadChunks(Element docRoot)
      throws IOException
   {
      if ( docRoot.getName().equals(ConfigTokens.chunk_db_TOKEN) )
      {
         List children = docRoot.getChildren();
         Iterator i    = children.iterator();

         while ( i.hasNext() )
         {
            Element chunk_elem = (Element) i.next();

            // We call insertOrdered() directly here because we already have
            // chunk_elem as a child of our DOM tree.
            insertOrdered(new ConfigChunk(chunk_elem));
         }
      }
      else
      {
         throw new IOException("Unrecognized element '" +
                               docRoot.getName() + "'.");
      }
   }

   /** Returns the difference between self and db.
    *  Build a new ConfigChunkDB that's a "difference" of its arguments -
    *  the returned db contains every chunk in db that isn't in self,
    *  or that differs from the same-named chunk in self.
    */
   public ConfigChunkDB diff(ConfigChunkDB d)
   {
      ConfigChunk c1, c2;
      ConfigChunkDB newdb = new ConfigChunkDB();

      for (int i = 0; i < d.size(); i++)
      {
         c1 = (ConfigChunk)d.chunks.get(i);
         c2 = get(c1.getName());
         if ((c2 == null) || (!c1.equals(c2)))
         {
            newdb.insertOrdered(c1);
         }
      }
      return newdb;
   }

   /**
    * Gets the number of ConfigChunks in this database.
    */
   public final int size()
   {
      return chunks.size();
   }

   /**
    * Gets the ith ConfigChunk in this database.
    *
    * @param idx     the index of the ConfigChunk to retrieve
    */
   public ConfigChunk get(int idx)
      throws IndexOutOfBoundsException
   {
      return(ConfigChunk)chunks.get(idx);
   }

   /** Removed all ConfigChunks from self and notifies ChunkDBListeners. */
   public void clear()
   {
      chunks.clear();
      notifyChunkDBTargets(new ChunkDBEvent(this, ChunkDBEvent.REMOVEALL, null));
   }

   /** Removes the ConfigChunk at index i and notifies ChunkDBListeners.
    *  @return The object that was removed
    */
   public ConfigChunk remove (int i)
      throws IndexOutOfBoundsException
   {
      ConfigChunk ch = (ConfigChunk)chunks.get(i);
      ChunkDBEvent e = new ChunkDBEvent(this, ChunkDBEvent.REMOVE, ch);

      // We have to remove the chunk's node from our DOM tree.
      Element chunk_elem = ch.getNode();
      mDoc.getRootElement().removeContent(chunk_elem);

      chunks.remove(i);
      notifyChunkDBTargets(e);

      return ch;
   }

   /** Removes the ConfigChunk in self whose name == name.
    *  @return The object that was removed, or null if it wasn't found.
    */
   public ConfigChunk remove(String name)
   {
      ConfigChunk ch;
      ListIterator i = chunks.listIterator();
      while (i.hasNext())
      {
         ch = (ConfigChunk)i.next();
         if ( ch.getName().equals(name) )
         {
            ChunkDBEvent e = new ChunkDBEvent(this, ChunkDBEvent.REMOVE, ch);
            i.remove();

            // We have to remove the chunk's node from our DOM tree.
            Element chunk_elem = ch.getNode();
            mDoc.getRootElement().removeContent(chunk_elem);

            notifyChunkDBTargets(e);
            return ch;
         }
      }
      return null;
   }

   /** Removes a ConfigChunk with the same name as ch.
    *  @return The object that was removed, or null if it wasn't found.
    */
   public final ConfigChunk remove(ConfigChunk ch)
   {
      return remove(ch.getName());
   }

   /** Removes any ConfigChunks in self with the same name as any Chunk
    *  in db.
    *  @return True if any ConfigChunks were removed, else false.
    */
   public boolean removeAll(ConfigChunkDB db)
   {
      boolean retval = false;
      ConfigChunk ch;
      ListIterator i = db.chunks.listIterator();
      while (i.hasNext())
      {
         ch = (ConfigChunk)i.next();
         retval |= (null != remove(ch));
      }
      return retval;
   }

   /** Find the named ConfigChunk.
    * @return A ConfigChunk in self whose name matches the parameter, or
    *         null if none is found.
    */
   public ConfigChunk get(String name)
   {
      ConfigChunk ch;
      Iterator i = chunks.iterator();
      while (i.hasNext())
      {
         ch = (ConfigChunk)i.next();
         if ( ch.getName().equals(name) )
         {
            return ch;
         }
      }
      return null;
   }

   /** Gets all ConfigChunks using a particular ChunkDesc.
    *  @param typename A ChunkDesc name.
    *  @return A list of all ConfigChunks in self using the named ChunkDesc.
    *          List may be empty, but is never null.  The list is not
    *          synchronized.
    */
   public List getOfDescName(String typename)
   {
      List v = new ArrayList();
      ConfigChunk ch;
      Iterator i = chunks.iterator();
      while (i.hasNext())
      {
         ch = (ConfigChunk)i.next();
         if ( ch.getDescName().equals(typename) )
         {
            v.add (ch);
         }
      }
      return v;
   }

   /** Gets all ConfigChunks using a particular ChunkDesc.
    *  @param typename A ChunkDesc name.
    *  @return A vector of all ConfigChunks in self using the named ChunkDesc.
    *          This may be an empty Vector, but is never null.
    */
   public List getOfDescToken(String typename)
   {
      List v = new ArrayList();
      ConfigChunk ch;
      Iterator i = chunks.iterator();
      while (i.hasNext())
      {
         ch = (ConfigChunk)i.next();
         if ( ch.getDescToken().equals(typename) )
         {
            v.add (ch);
         }
      }
      return v;
   }

   public void setUnsavedChanges(boolean dirty)
   {
      mNeedToSave = dirty;
   }

   public boolean hasUnsavedChanges()
   {
      return mNeedToSave;
   }

   /**
    * Inserts ch into self.  Any same-named Chunk already in self is removed.
    * The DOM tree node associated with the given chunk is added as a child
    * of this DB's tree node.
    */
   public final void add(ConfigChunk ch)
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

      insertOrdered(insert_chunk);
   }

   /** Inserts all Chunks in db into self.
    *  Any same-named Chunks already in self are removed.
    */
   public void addAll(ConfigChunkDB db)
   {
      Iterator i = db.chunks.iterator();
      while (i.hasNext())
      {
         add((ConfigChunk)i.next());
      }
   }

   /** Inserts c into self.
    *  Adds a Chunk into self, with the constraint that it's stored with
    *  adjacent to all other Chunks using the same ChunkDesc.
    */
   public void insertOrdered(ConfigChunk c)
   {
      remove(c.getName());

      String desc_token = c.getDescToken();

      // If the chunk being inserted does not have a description, it is an
      // unknown chunk type.  We will hang onto it instead of just tossing it
      // aside, but there is nothing we can do with it.
      if ( null == desc_token )
      {
         chunks.add(c);
      }
      else
      {
         // Desription includes are forced to be first in the chunk collection.
         if ( desc_token.equals(ConfigTokens.vj_include_desc_file_TOKEN) )
         {
            chunks.add(0, c);
         }
         // This appears to add a config file include "chunk" before the first
         // element of the chunk collection that is not an include directive.
         // I suppose this is to ensure that includes always come before loaded
         // chunks.
         else if ( desc_token.equals(ConfigTokens.vj_include_file_TOKEN) )
         {
            int i;
            String desc_token2;

            for ( i = 0; i < chunks.size(); ++i )
            {
               // Get the chunk description type of a previously loaded chunk.
               desc_token2 = ((ConfigChunk) chunks.get(i)).getDescToken();

               if ( ! desc_token2.equals(ConfigTokens.vj_include_desc_file_TOKEN) &&
                    ! desc_token2.equals(ConfigTokens.vj_include_file_TOKEN) )
               {
                  break;
               }
            }
            chunks.add(i, c);
         }
         else
         {
            chunks.add(c);
         }

         ChunkDBEvent e = new ChunkDBEvent(this, ChunkDBEvent.INSERT, c);
         notifyChunkDBTargets(e);
      }
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

   /** Utility for getDependencies. */
   protected void addDependsForChunk(ConfigChunk ch, List v, String nameprefix)
   {
      // returns vector of pde's
      // finds dependencies in ch and adds 'em to v.
      int prop_count;

      Iterator i = ch.getDesc().getPropertyDescs().iterator();

      while ( i.hasNext() )
      {
         PropertyDesc prop_desc = (PropertyDesc) i.next();

         if ( prop_desc.getValType() == ValType.CHUNK )
         {
            prop_count = ch.getNumPropertyValues(prop_desc.getToken());

            for ( int j = 0; j < prop_count; ++j )
            {
               VarValue val = ch.getProperty(prop_desc.getToken(), j);
               String chunk_name = val.getString();

               if ( ! chunk_name.equals("") )
               {
                  ConfigChunk dep_chunk = this.get(chunk_name);
                  if ( null == dep_chunk )
                  {
                     PropDependEntry pde = new PropDependEntry();
                     pde.propertyname = nameprefix + chunk_name;

                     // XXX: I'm not sure if prop_count is the right variable
                     // to look at here...
                     if ( prop_count > 1 )
                     {
                        pde.propertyname += "[" + j + "]";
                     }

                     pde.dependency_name = chunk_name;
                     v.add(pde);
                  }
               }
            }
         }
         else if ( prop_desc.getValType() == ValType.EMBEDDEDCHUNK )
         {
            prop_count = ch.getNumPropertyValues(prop_desc.getToken());
            ConfigChunk child_chunk;

            for ( int j = 0; j < prop_count; ++j )
            {
               child_chunk =
                  ch.getProperty(prop_desc.getToken(), j).getEmbeddedChunk();
               String prefix = nameprefix + child_chunk.getName();

               if ( prop_count > 1 )
               {
                  prefix += "[" + j + "].";
               }
               else
               {
                  prefix += ".";
               }

               addDependsForChunk(child_chunk, v, nameprefix + prefix);
            }
         }
      }
   }

   /** Gets dependencies of all ConfigChunks in self.
    *  Includes only dependencies that aren't satisfied by self.
    *  @return a List of ChunkDependEntries.
    */
   public List getDependencies()
   {
      // gets the dependencies of _this_chunkdb_only_
      int i;
      ConfigChunk ch;
      ChunkDependEntry cde;

      List dep = new ArrayList();

      for (i = 0; i < chunks.size(); i++)
      {
         cde = null;
         ch = (ConfigChunk)chunks.get(i);
         cde = new ChunkDependEntry();
         cde.chunk = ch;
         addDependsForChunk(ch, cde.propdepends, "");
         if (cde.propdepends.size() > 0)
         {
            dep.add(cde);
         }
      }
      return dep;
   }

   /** Searches self for Chunks that satisfy dependencies.
    *  @param v A vector of ChunkDependEntries, assumably generated by
    *           getDependencies called on another ChunkDB.  searchDependencies
    *           modifies the DependEntries with new information.
    */
   public void searchDependencies(List v)
   {
      ChunkDependEntry cde;
      PropDependEntry pde;
      ConfigChunk ch;
      int i, j;

      for (i = 0; i < v.size(); i++)
      {
         cde = (ChunkDependEntry)v.get(i);
         for (j = 0; j < cde.propdepends.size(); j++)
         {
            pde = (PropDependEntry)cde.propdepends.get(j);
            //ch = get (pde.dependency_name);
            ch = searchDependency(pde.dependency_name);
            if (ch != null)
            {
               pde.other_files.add(getName());
            }
         }
      }
   }

   private ConfigChunk searchDependency(String dep_name)
   {
      if (!ConfigChunk.hasSeparator(dep_name))
      {
         return get(dep_name);
      }
      else
      {
         // embedded chunk dependency
         String first = ConfigChunk.getFirstNameComponent(dep_name);
         ConfigChunk ch = get(first);
         if (ch == null)
         {
            return null;
         }
         else
         {
            return ch.getEmbeddedChunk(dep_name);
         }
      }
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
      mNeedToSave = true;
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
         new ProcessingInstruction(ConfigTokens.SETTINGS_INSTRUCTION, pi_attrs);
      doc.addContent(ver);
   }

   /** Vector of ConfigChunks. */
   private List chunks;

   /** Targets for ChunkDBEvents. */
   private ArrayList targets;

   /** Identifier for the ConfigChunkDB.
    *  Note: It's generally the case that name will be the last component
    *  of file, but this isn't always true.  VjControl will append tags
    *  to the name in order to distinguish files with the same name
    *  (e.g. "config [1]" and "config [2]").
    */
   private String name;

   /** File that the ChunkDB was loaded from.
    *  This is used for loading & saving the ChunkDB, and is guaranteed
    *  not to be mangled like name could be.
    *  <p>
    *  Note: file defaults to "Untitled", but is never null unless
    *  explicitly set.
    */
   private File inputFile;

   /** This is the XML document from which this DB was constructed. */
   protected Document mDoc;

   /** Flag to determine if file has been modified since last saved.
    */
   protected boolean mNeedToSave = false;
}
