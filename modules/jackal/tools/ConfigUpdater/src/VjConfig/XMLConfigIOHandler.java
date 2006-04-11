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
package VjConfig;

import java.io.*;
import java.util.Iterator;
import java.util.List;
import javax.xml.parsers.*;
import org.jdom.*;
import org.jdom.input.SAXBuilder;


/** ConfigIOHandler for XML files.
 *  This class handles reading and writing ChunkDescDB and ConfigChunkDB files
 *  using the new, spiffy XML syntax.  It also includes publicly-available
 *  methods buildConfigChunkDB and buildChunkDescDB which convert DOM trees
 *  into {Chunk|Desc}DBs (used by the XMLConfigCommunicator, which does its
 *  own XML->DOM parsing).
 *
 *  @version $Revision$
 */
public class XMLConfigIOHandler
   implements ConfigIOHandler
{

   /** Constructor. */
   public XMLConfigIOHandler()
   {
      ;
   }

   /** Escapes &, <, >, and other symbols that can't appear in xml strings.
    *  This is a utility used by the writer functions.
    *  These escapes get undone by the xml parser so we don't need to
    *  worry about 'em ourselves.
    */
   public static String escapeString(String original)
   {
      // the overwhelming majority of strings won't need to be escaped.
      int amppos = original.indexOf('&');
      int lpos = original.indexOf('<');
      int gpos = original.indexOf('>');
      if (amppos == -1 && lpos == -1 && gpos == -1)
      {
         return original;
      }
      // ok, there's something that needs to be fixed... this probably
      // doesn't need to be fast cuz it'll be used rarely.
      StringBuffer s = new StringBuffer(original);
      // remove ampersands
      while (amppos != -1)
      {
         s.replace (amppos, amppos+1, "&amp;");
         amppos = indexOf(s, '&', amppos+1);
      }
      lpos = indexOf(s, '<', 0);
      while (lpos != -1)
      {
         s.replace (lpos, lpos+1, "&lt;");
         lpos = indexOf(s, '<', lpos+1);
      }
      gpos = indexOf(s, '>', 0);
      while (gpos != -1)
      {
         s.replace (gpos, gpos+1, "&gt;");
         gpos = indexOf(s, '>', gpos+1);
      }
      return s.toString();
   }


   /** does a stringbuffer indexof.
    *  remove this if we ever go to requiring jdk1.4.  sometimes the
    *  shortsightedness of the jdk developers really stuns me.
    */
   private static int indexOf(StringBuffer source, char ch, int startindex)
   {
      for (; startindex < source.length(); startindex++)
      {
         if (source.charAt(startindex) == ch)
         {
            return startindex;
         }
      }
      return -1;
   }


   public void readConfigChunkDB(File file, ConfigChunkDB db,
                                 ConfigIOStatus iostatus)
   {
      // build the XML stream into a DOM tree
      try
      {
         SAXBuilder builder = new SAXBuilder();
         Document doc = builder.build(file);
         buildChunkDB(db, doc.getRootElement(), iostatus);
      }
      catch (Exception e)
      {
         iostatus.addFailure(e);
      }
   }


   public void readConfigChunkDB(InputStream in, ConfigChunkDB db,
                                 ConfigIOStatus iostatus)
   {
      // build the XML stream into a DOM tree
      try
      {
         SAXBuilder builder = new SAXBuilder();
         Document doc = builder.build(in);
         buildChunkDB(db, doc.getRootElement(), iostatus);
      }
      catch (Exception e)
      {
         iostatus.addFailure(e);
      }
   }


   public void writeConfigChunkDB(DataOutputStream out, ConfigChunkDB db)
      throws IOException
   {
      try
      {
         String xml = db.xmlRep();
         out.writeBytes(xml);
      }
      catch (Exception e)
      {
         e.printStackTrace();
         throw new IOException(e.getMessage());
      }
   }


   public void writeConfigChunkDB(File out, ConfigChunkDB db)
      throws IOException
   {
      try
      {
         String xml = db.xmlRep();
         DataOutputStream ostream = new DataOutputStream(new FileOutputStream(out));
         ostream.writeBytes("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
         ostream.writeBytes("<?org-vrjuggler-jccl-settings config.db.version=\"2.0\"?>\n");
         ostream.writeBytes(xml);
      }
      catch (Exception e)
      {
         e.printStackTrace();
         throw new IOException(e.getMessage());
      }
   }


   /** Converts a DOM tree to a ConfigChunkDB.
    *  This method reads the DOM tree rooted at doc, and puts all
    *  ConfigChunks it finds into db.  doc can be a DOCUMENT,
    *  DOCUMENT_FRAGMENT (with the first element being "<ConfigChunkDB">)
    *  or an ELEMENT (labeled "<ConfigChunkDB>").
    *
    *  Any problems encountered during the build are recorded in the
    *  ConfigIOStatus argument.
    */
   public void buildChunkDB(ConfigChunkDB db, Element doc,
                            ConfigIOStatus iostatus)
   {
      Element child;
      ConfigChunk ch;
      boolean retval = true;
      String name = doc.getName();

      if ( name.equals(ConfigTokens.chunk_db_TOKEN) )
      {
         List children = doc.getChildren();
         Iterator i    = children.iterator();

         while ( i.hasNext() )
         {
            ch = buildConfigChunk((Element) i.next(), iostatus);
            if (ch != null)
            {
               db.add(ch);
            }
         }
      }
      else
      {
         iostatus.addWarning("Unrecognized element '" + name + "'.");
      }
   }


   public ConfigChunk buildConfigChunk (Element doc, ConfigIOStatus iostatus)
   {
      ConfigChunk ch = null;
      String name = doc.getName();

      ch = ChunkFactory.createChunkWithDescToken(name);

      if (ch == null)
      {
         iostatus.addError("Unable to create ConfigChunk of type '"
                           + name + "' - no ChunkDesc known.");
      }
      else
      {
         // Get the chunk's name attribute.
         ch.setName(doc.getAttribute("name").getValue());

         // Get child elements.
         buildProperty(ch, doc, iostatus);
      }

      return ch;
   }


   private void buildProperty (ConfigChunk ch, Element doc,
                               ConfigIOStatus iostatus)
   {
      String name = doc.getName();
      int valindex;

      PropertyDesc[] prop_descs = ch.getDesc().getPropertyDescs();
      List children             = doc.getChildren();
      int child_count           = children.size();

      // This starts at 1 because the "Name" property is always at index 0.
      // Since the chunk's name is an attribute of the element rather than
      // a child element, it is not handled here.
      for ( int i = 1; i < prop_descs.length; ++i )
      {
         String prop_name  = prop_descs[i].getToken();
         Property cur_prop = ch.getPropertyFromToken(prop_name);
         valindex = 0;

         int prop_count = prop_descs[i].getNumValues();
         Iterator j     = children.iterator();

         while ( j.hasNext() )
         {
            Element child = (Element) j.next();

            if ( child.getName().equals(prop_name) )
            {
               VarValue new_val = null;

               // I wish there weren't a special case for embedded chunk...
               if ( cur_prop.getValType() == ValType.EMBEDDEDCHUNK )
               {
                  if ( child.hasChildren() )
                  {
                     DescEnum[] allowed_types = prop_descs[i].getEnumerations();
                     Element next_child;
                     Iterator iter;

                     for ( int k = 0; k < allowed_types.length; ++k )
                     {
                        iter = child.getChildren(allowed_types[k].str).iterator();

                        while ( iter.hasNext() )
                        {
                           ConfigChunk child_chunk =
                              buildConfigChunk((Element) iter.next(), iostatus);

                           if ( child_chunk != null )
                           {
                              cur_prop.setValue(new VarValue(child_chunk),
                                                valindex);
                              valindex++;
                           }
                        }
                     }
                  }
                  else
                  {
                     iostatus.addError("ERROR: Embedded chunk expected for property " +
                                       prop_name + " but no chunk found");
                  }
               }
               else
               {
                  String child_value = child.getTextTrim();
                  new_val = cur_prop.desc.getEnumValue(child_value);

                  if ( new_val == null )
                  {
                     new_val = new VarValue(cur_prop.getValType());
                     try
                     {
                        new_val.set(child_value);
                     }
                     catch (Exception e)
                     {
                        iostatus.addWarning(e);
                     }
                  }

                  // prop_count will be -1 if there is no limit to the number
                  // of values for the curent property.  Otherwise, it will
                  // specify the maximum number that can be inserted.
                  if ( new_val != null &&
                       (prop_count == -1 || valindex < prop_count) )
                  {
                     cur_prop.setValue(new_val, valindex);
                     valindex++;
                  }
                  else if ( prop_count != -1 && valindex >= prop_count )
                  {
                     iostatus.addWarning("WARNING: Ignoring extra element for property " +
                                         prop_name + " (" + prop_count +
                                         " allowed, " + valindex + " read)");
                  }
               }
            }
         }
      }
   }

   //------------------------ ChunkDescDB Methods --------------------------

   public void readChunkDescDB(File file, ChunkDescDB db,
                               ConfigIOStatus iostatus)
   {
      // build the XML stream into a DOM tree
      try
      {
         SAXBuilder builder = new SAXBuilder();
         Document doc       = builder.build(file);
         buildChunkDescDB(db, doc.getRootElement(), iostatus);
      }
      catch (Exception e)
      {
         iostatus.addFailure(e);
      }
   }


   public void readChunkDescDB(InputStream in, ChunkDescDB db,
                               ConfigIOStatus iostatus)
   {
      // build the XML stream into a DOM tree
      try
      {
         SAXBuilder builder = new SAXBuilder();
         Document doc       = builder.build(in);
         buildChunkDescDB(db, doc.getRootElement(), iostatus);
      }
      catch (Exception e)
      {
         iostatus.addFailure(e);
      }
   }


   public void writeChunkDescDB(DataOutputStream out, ChunkDescDB db)
      throws IOException
   {
      try
      {
         String xml = db.xmlRep();
         out.writeBytes(xml);
      }
      catch (Exception e)
      {
         e.printStackTrace();
         throw new IOException(e.getMessage());
      }
   }

   public void writeChunkDescDB(File out, ChunkDescDB db)
      throws IOException
   {
      try
      {
         String xml = db.xmlRep();
         DataOutputStream ostream = new DataOutputStream (new FileOutputStream (out));
         ostream.writeBytes("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
         ostream.writeBytes("<?org-vrjuggler-jccl-settings desc.db.version=\"2.0\"?>\n");
         ostream.writeBytes(xml);
      }
      catch (Exception e)
      {
         e.printStackTrace();
         throw new IOException(e.getMessage());
      }
   }



   /** Converts a DOM tree to a ChunkDescDB.
    *  This method reads the DOM tree rooted at doc, and puts all
    *  ChunkDescs it finds into db.  doc can be a DOCUMENT,
    *  DOCUMENT_FRAGMENT (with the first element being "<ChunkDescDB">)
    *  or an ELEMENT (labeled "<ChunkDescDB>").
    *
    *  Any problems encountered during the build are recorded in the
    *  ConfigIOStatus argument.
    */
   public void buildChunkDescDB(ChunkDescDB db, Element doc,
                                ConfigIOStatus iostatus)
   {
      ChunkDesc desc;
      String name = doc.getName();

      if (name.equals(ConfigTokens.chunk_desc_db_TOKEN))
      {
         List children = doc.getChildren();
         Iterator i    = children.iterator();

         while ( i.hasNext() )
         {
            desc = buildChunkDesc((Element) i.next(), iostatus);
            if (desc != null)
            {
               db.add(desc);
            }
         }
      }
      else
      {
         iostatus.addWarning("Unrecognized element '" + name + "'.");
      }
   }

   public void buildChunkDescDB(ChunkDescDB db, Document doc,
                                ConfigIOStatus iostatus)
   {
      List children = doc.getRootElement().getChildren();
      Iterator i    = children.iterator();

      while ( i.hasNext() )
      {
         buildChunkDescDB(db, (Element) i.next(), iostatus);
      }
   }

   /** Builds a ChunkDesc out of the DOM tree rooted at doc.
    *  Any problems encountered during the build are recorded in the
    *  ConfigIOStatus argument.
    */
   public ChunkDesc buildChunkDesc (Element doc, ConfigIOStatus iostatus)
   {
      ChunkDesc desc = null;
      String name = doc.getName();

      if (name.equals("ChunkDesc"))
      {
         desc = new ChunkDesc();
         // parse attributes
         desc.setToken(doc.getAttribute("token").getValue());
         desc.setName(doc.getAttribute("name").getValue());

         if ( doc.getAttribute("name") == null )
         {
            desc.setName(desc.getToken());
         }
         else
         {
            desc.setName(doc.getAttribute("name").getValue());
         }

         List children = doc.getChildren();
         Iterator i    = children.iterator();

         while ( i.hasNext() )
         {
            parseChunkDescChildElement(desc, (Element) i.next(), iostatus);
         }
      }
      else
      {
         iostatus.addWarning("Unrecognized element '" + name + "'.");
      }

      return desc;
   }

   /** Parse a child element of a ChunkDesc element.
    *  This is a utility function for buildChunkDesc.
    *  The children of ChunkDesc nodes include PropertyDesc, help,
    *  and Defaults nodes.
    */
   private void parseChunkDescChildElement (ChunkDesc desc, Element doc,
                                            ConfigIOStatus iostatus)
   {
      String name = doc.getName();

      if (name.equals("PropertyDesc"))
      {
         PropertyDesc p = new PropertyDesc();
         Attribute attr;

         attr = doc.getAttribute("token");
         if ( attr != null )
         {
            p.setToken(attr.getValue());
         }

         attr = doc.getAttribute("name");
         if ( attr != null )
         {
            p.setName(attr.getValue());
         }

         attr = doc.getAttribute("type");
         if ( attr != null )
         {
            p.setValType(ValType.getValType(attr.getValue()));
         }

         attr = doc.getAttribute("num");
         if ( attr != null )
         {
            if ( attr.getValue().startsWith("var") )
            {
               p.setNumValues(-1);
            }
            else
            {
               p.setNumValues(Integer.parseInt(attr.getValue()));
            }
         }

         attr = doc.getAttribute("userlevel");
         if ( attr != null )
         {
            String val = attr.getValue();

            if ( val.equalsIgnoreCase("expert") )
            {
               p.setUserLevel(1);
            }
            else if ( val.equalsIgnoreCase("beginner") )
            {
               p.setUserLevel(0);
            }
         }

         if ( p.getName().equals("") )
         {
            p.setName(p.getToken());
         }

         // parse child elements
         List children = doc.getChildren();
         Iterator i    = children.iterator();

         while ( i.hasNext() )
         {
            parsePropertyDescChildElement(p, (Element) i.next(), iostatus);
         }

         // right here we should add some validation of the new
         // propertydesc...
         desc.addPropertyDesc (p);
      }
      else if (name.equals("help"))
      {
         desc.setHelp(doc.getText());
      }
      else
      {
         iostatus.addWarning("Unrecognized element '" + name + "'.");
      }
   }



   /** Parses the child Nodes of a PropertyDesc.
    *  Utility function for parseChunkDescChildElement.
    *  The children of a PropertyDesc node include label, enumeration,
    *  and help nodes.
    */
   private void parsePropertyDescChildElement (PropertyDesc p, Element doc,
                                               ConfigIOStatus iostatus)
   {
      String name = doc.getName();
      Element child;
      String childname;
      String childval;
      int attrcount;
      int valindex = 0;

      if ( name.equals("item") )
      {
         String label_name = doc.getAttribute("label").getValue();

         VarValue value = new VarValue(p.getValType());
         if ( doc.getAttribute("defaultvalue") != null )
         {
            value.set(doc.getAttribute("defaultvalue").getValue());
         }

         p.addItem(new PropertyDesc.Item(label_name, value));
      }
      else if ( name.equals("enumeration") )
      {
         String enumname = doc.getAttribute("name").getValue();

         if (!enumname.equals(""))
         {
            String enumval = "";
            if ( doc.getAttribute("value") != null )
            {
               enumval= doc.getAttribute("value").getValue();
            }

            p.appendEnumeration(enumname, enumval);
         }

      }
      else if ( name.equals("help") )
      {
         p.setHelp(doc.getText());
      }
      else
      {
         iostatus.addWarning("Unrecognized element '" + name + "'.");
      }
   }
}
