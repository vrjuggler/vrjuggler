/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

import javax.xml.parsers.*;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.w3c.dom.*;
import java.io.*;

import VjConfig.*;

/** ConfigIOHandler for XML files.
 *  This class handles reading and writing ChunkDescDB and ConfigChunkDB files
 *  using the new, spiffy XML syntax.  It also includes publicly-available
 *  methods buildConfigChunkDB and buildChunkDescDB which convert DOM trees
 *  into {Chunk|Desc}DBs (used by the XMLConfigCommunicator, which does its
 *  own XML->DOM parsing).
 *
 *  @author Christopher Just
 *  @version $Revision$
 */
public class XMLConfigIOHandler implements ConfigIOHandler {

    /** Constructor. */
    public XMLConfigIOHandler () {
        ;
    }


    /** Escapes &, <, >, and other symbols that can't appear in xml strings.
     *  This is a utility used by the writer functions.
     *  These escapes get undone by the xml parser so we don't need to 
     *  worry about 'em ourselves.
     */
    public static String escapeString (String original) {
        // the overwhelming majority of strings won't need to be escaped.
        int amppos = original.indexOf('&');
        int lpos = original.indexOf('<');
        int gpos = original.indexOf('>');
        if (amppos == -1 && lpos == -1 && gpos == -1)
            return original;
        // ok, there's something that needs to be fixed... this probably
        // doesn't need to be fast cuz it'll be used rarely.
        StringBuffer s = new StringBuffer(original);
        // remove ampersands
        while (amppos != -1) {
            s.replace (amppos, amppos+1, "&amp;");
            amppos = indexOf (s, '&', amppos+1);
        }
        lpos = indexOf (s, '<', 0);
        while (lpos != -1) {
            s.replace (lpos, lpos+1, "&lt;");
            lpos = indexOf (s, '<', lpos+1);
        }
        gpos = indexOf (s, '>', 0);
        while (gpos != -1) {
            s.replace (gpos, gpos+1, "&gt;");
            gpos = indexOf (s, '>', gpos+1);
        }
        return s.toString();
    }


    /** does a stringbuffer indexof.
     *  remove this if we ever go to requiring jdk1.4.  sometimes the
     *  shortsightedness of the jdk developers really stuns me.
     */
    private static int indexOf (StringBuffer source, char ch, int startindex) {
        for (; startindex < source.length(); startindex++)
            if (source.charAt(startindex) == ch)
                return startindex;
        return -1;
    }


    public void readConfigChunkDB (File file, ConfigChunkDB db) throws IOException, ConfigParserException {

        ConfigIOStatus iostatus = new ConfigIOStatus();

        Document doc;
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        
	// build the XML stream into a DOM tree
        try {
            DocumentBuilder builder = factory.newDocumentBuilder();
            doc = builder.parse (file);
            buildChunkDB (db, doc, iostatus);
        }
        catch (javax.xml.parsers.ParserConfigurationException e1) {
            iostatus.addFailure (e1);
        }
        catch (org.xml.sax.SAXException e2) {
            iostatus.addFailure (e2);
        }

        // temporary
        System.out.println (iostatus.toString());

        if (iostatus.getStatus() == iostatus.FAILURE)
            throw new ConfigParserException (iostatus.toString());
    }


    public void readConfigChunkDB (InputStream in, ConfigChunkDB db) throws IOException, ConfigParserException {

        ConfigIOStatus iostatus = new ConfigIOStatus();

        boolean retval = false;
        Document doc;
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();

	// build the XML stream into a DOM tree
        try {
            DocumentBuilder builder = factory.newDocumentBuilder();
            doc = builder.parse (in);
            buildChunkDB (db, doc, iostatus);
        }
        catch (javax.xml.parsers.ParserConfigurationException e1) {
            iostatus.addFailure (e1);
        }
        catch (org.xml.sax.SAXException e2) {
            iostatus.addFailure (e2);
        }

        // temporary
        System.out.println (iostatus.toString());

        if (iostatus.getStatus() == iostatus.FAILURE)
            throw new ConfigParserException (iostatus.toString());
    }
    

    public boolean writeConfigChunkDB (DataOutputStream out, ConfigChunkDB db) {
        try {
            long time = System.currentTimeMillis();
            db.xmlRep();
            time = System.currentTimeMillis()-time;
            System.out.println ("ConfigChunkDB.xmlRep took " + time + " ms.");

            out.writeBytes (db.xmlRep());
            return true;
        }
        catch (Exception e) {
            return false;
        }    
    }


    public boolean writeConfigChunkDB (File out, ConfigChunkDB db) {
        try {
            DataOutputStream ostream = new DataOutputStream (new FileOutputStream (out));
            ostream.writeBytes (db.xmlRep());
            return true;
        }
        catch (Exception e) {
            return false;
        }
    }

    /** temporary hack to avoid breaking apis... */
    public boolean buildChunkDB (ConfigChunkDB db, Node doc) {
        ConfigIOStatus iostatus = new ConfigIOStatus();
        buildChunkDB (db, doc, iostatus);
        return (iostatus.getStatus() == iostatus.SUCCESS || iostatus.getStatus() == iostatus.WARNINGS);
    }

    /** temporary hack */
    public ConfigChunk buildConfigChunk (Node doc, boolean use_defaults) {
        ConfigIOStatus iostatus = new ConfigIOStatus();
        ConfigChunk ch = buildConfigChunk (doc, use_defaults, iostatus);
        if (iostatus.getStatus() == iostatus.ERRORS || iostatus.getStatus() == iostatus.FAILURE)
            return null;
        else
            return ch;
    }


    /** Converts a DOM tree to a ConfigChunkDB.
     *  This method reads the DOM tree rooted at doc, and puts all
     *  ConfigChunks it finds into db.  doc can be a DOCUMENT, 
     *  DOCUMENT_FRAGMENT (with the first element being "<ConfigChunkDB">)
     *  or an ELEMENT (labeled "<ConfigChunkDB>").
     *
     *  @returns False if errors were encountered, otherwise true.  If
     *           false, some ConfigChunks may still have been read into db.
     */
    public void buildChunkDB (ConfigChunkDB db, Node doc, 
                              ConfigIOStatus iostatus) {
        Node child;
        ConfigChunk ch;
        boolean retval = true;
        String name = doc.getNodeName();
        
        switch (doc.getNodeType()) {
        case Node.DOCUMENT_NODE:
        case Node.DOCUMENT_FRAGMENT_NODE:
            child = doc.getFirstChild();
            while (child != null) {
                buildChunkDB (db, child, iostatus);
                child = child.getNextSibling();
            }
            break;
        case Node.ELEMENT_NODE:
            if (name.equalsIgnoreCase ("ConfigChunkDB")) {
                child = doc.getFirstChild();
                while (child != null) {
                    //System.out.println ("got a child of ConfigChunkDB");
                    if (child.getNodeType() == Node.ELEMENT_NODE) {
                        ch = buildConfigChunk (child, true, iostatus);
                        if (ch != null)
                            db.add (ch);
                    }
                    child = child.getNextSibling();
                }
            }
            else {
                iostatus.addWarning ("Unrecognized element '" + name + "'.\n");
            }
            break;
        case Node.COMMENT_NODE:
        case Node.NOTATION_NODE:
        case Node.PROCESSING_INSTRUCTION_NODE:
        case Node.TEXT_NODE:
            break;
        default:
            iostatus.addWarning ("Unexpected DOM node type...\n");
        }
    }


    public ConfigChunk buildConfigChunk (Node doc, boolean use_defaults,
                                         ConfigIOStatus iostatus) {
        Node child;
        NamedNodeMap attributes;
        int attrcount;
        int i;
        ConfigChunk ch = null;
        String name = doc.getNodeName();

        switch (doc.getNodeType()) {
        case Node.DOCUMENT_NODE:
        case Node.DOCUMENT_FRAGMENT_NODE:
            child = doc.getFirstChild();
            return buildConfigChunk (child, use_defaults, iostatus);
        case Node.ELEMENT_NODE:
            ch = ChunkFactory.createChunkWithDescToken (name, use_defaults);
            if (ch == null) {
                iostatus.addError ("Unable to create ConfigChunk of type '"
                                   + name + "' - no ChunkDesc known.\n");
            }
            else {
                // parse attributes
                attributes = doc.getAttributes();
                attrcount = attributes.getLength();
                for (i = 0; i < attrcount; i++) {
                    child = attributes.item(i);
                    buildProperty (ch, child, iostatus);
                }
                // parse child elements
                child = doc.getFirstChild();
                while (child != null) {
                    buildProperty (ch, child, iostatus);
                    child = child.getNextSibling();
                }
            }
            break;
        case Node.COMMENT_NODE:
        case Node.NOTATION_NODE:
        case Node.PROCESSING_INSTRUCTION_NODE:
        case Node.TEXT_NODE:
            break;
        default:
            iostatus.addWarning ("Unexpected DOM node type...\n");
        }

        return ch;
    }
        
            
    private void buildProperty (ConfigChunk ch, Node doc,
                                ConfigIOStatus iostatus) {
        String name = doc.getNodeName();
        String value = doc.getNodeValue();
        boolean retval = true;
        Node child;
        Property p;
        int valindex = 0;

        switch (doc.getNodeType()) {
        case Node.ELEMENT_NODE:
            p = ch.getPropertyFromToken (name);
            if (p != null) {
                child = doc.getFirstChild();
                while (child != null) {
                    if (p.getValType() == ValType.EMBEDDEDCHUNK) {
                        ch = buildConfigChunk (child, true, iostatus);
                        if (ch != null)
                            p.setValue (new VarValue(ch), valindex++);
                    }
                    else {
                        switch (child.getNodeType()) {
                        case Node.TEXT_NODE:
                            // watch it... we're not gonna handle multiple
                            // text fragments at all with this bit of code.
                            // need to muck with parseTextValues cuz we're
                            // not passing valindex by reference like we do
                            // on the C++ side.
                            parseTextValues (p, valindex, child.getNodeValue(), iostatus);
                            break;
                        default:
                            iostatus.addWarning ("Unexpected XML element in " +
                                                 "Property definition: '" +
                                                 child.getNodeValue() + 
                                                 "'.\n");
                        }
                    }
                    child = child.getNextSibling();
                }
            }
            else
                iostatus.addWarning ("No such property '" + name + 
                                     "' in ConfigChunk of type '" +
                                     ch.getDescName() + "'.\n");
            break;
        case Node.ATTRIBUTE_NODE:
            if (name.equalsIgnoreCase ("name")) {
                ch.setName (value);
            }
            else {
                p = ch.getPropertyFromToken (name);
                if (p != null) {
                    parseTextValues (p, valindex, value, iostatus);
                }
                else
                    iostatus.addWarning ("No such property '" + name + 
                                         "' in ConfigChunk of type '" +
                                         ch.getDescName() + "'.\n");
            }
            break;
        case Node.COMMENT_NODE:
        case Node.NOTATION_NODE:
        case Node.PROCESSING_INSTRUCTION_NODE:
        case Node.TEXT_NODE:
            break;
        default:
            iostatus.addWarning ("Unexpected DOM node type...\n");
        }
    }


    private String stringTokenizer (StringBuffer buf) {
        String s = null;
        //String tbuf = new String(buf); // for debugging
        char ch;
        int i = 0;
        int j;
        int n = buf.length();
        for (i = 0; i < n;) {
            ch = buf.charAt(i);
            if (Character.isWhitespace (ch)) {
                i++;
                continue;
            }
            if (ch == '"') {
                // we found a quoted string
                for (j = i+1; j < n; j++)
                    if (buf.charAt(j) == '"')
                        break;
                s = buf.substring (i+1, j);
                buf.delete (0, j+1);
                break;
            }
            else {
                // we found an unquoted string
                for (j = i+1; j < n; j++)
                    if (Character.isWhitespace(buf.charAt(j)))
                        break;
                s = buf.substring (i, j);
                buf.delete (0, j);
                break;
            }
        }
        //System.out.println ("String Tokenizer called with '" + tbuf + "' returns '" + s + "'");
        return s;
    }


    private void parseTextValues (Property p, int startval, String text,
                                  ConfigIOStatus iostatus) {

        StringBuffer buf = new StringBuffer (text);
        String s;
        VarValue v;

        ValType vt = p.getValType();
        if (vt == ValType.CHUNK) {
            // don't check enums for this type, because it contains a list
            // of allowed chunk _types_, not chunk references.
            while ((s = stringTokenizer (buf)) != null) {
                v = new VarValue(ValType.CHUNK);
                v.set (s);
                p.setValue (v, startval++);
            }
        }
        else if (vt == ValType.STRING) {
            while ((s = stringTokenizer (buf)) != null) {
                v = p.desc.getEnumValue (s);
                p.setValue (v, startval++);
            }
        }
        else if (vt == ValType.INT || vt == ValType.FLOAT || 
                 vt == ValType.BOOL) {
            while ((s = stringTokenizer (buf)) != null) {
                v = p.desc.getEnumValue (s);
                p.setValue (v, startval++);
            }
        }
        else
            iostatus.addError ("THIS SHOULD NOT HAPPEN in XMLConfigIOHandler" +
                               ".parseTextValues().\n");

    }


    //------------------------ ChunkDescDB Methods --------------------------
    
    public void readChunkDescDB (File file, ChunkDescDB db) throws IOException, ConfigParserException {

        ConfigIOStatus iostatus = new ConfigIOStatus();
        Document doc;
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        
	// build the XML stream into a DOM tree
        try {
            DocumentBuilder builder = factory.newDocumentBuilder();
            doc = builder.parse (file);
            buildChunkDescDB (db, doc, iostatus);
        }
        catch (javax.xml.parsers.ParserConfigurationException e1) {
            iostatus.addFailure (e1);
        }
        catch (org.xml.sax.SAXException e2) {
            iostatus.addFailure (e2);
        }

        // temporary
        System.out.println (iostatus.toString());

        if (iostatus.getStatus() == iostatus.FAILURE)
            throw new ConfigParserException (iostatus.toString());
    }


    public void readChunkDescDB (InputStream in, ChunkDescDB db) throws IOException, ConfigParserException {

        ConfigIOStatus iostatus = new ConfigIOStatus();

        Document doc;
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();

	// build the XML stream into a DOM tree
        try {
            DocumentBuilder builder = factory.newDocumentBuilder();
            doc = builder.parse (in);
            buildChunkDescDB (db, doc, iostatus);
        }
        catch (javax.xml.parsers.ParserConfigurationException e1) {
            iostatus.addFailure (e1);
        }
        catch (org.xml.sax.SAXException e2) {
            iostatus.addFailure (e2);
        }

        // temporary
        System.out.println (iostatus.toString());

        if (iostatus.getStatus() == iostatus.FAILURE)
            throw new ConfigParserException (iostatus.toString());
    }
    

    public boolean writeChunkDescDB (DataOutputStream out, ChunkDescDB db) {
        try {
            out.writeBytes (db.xmlRep());
            return true;
        }
        catch (Exception e) {
            e.printStackTrace();
            return false;
        }    
    }

    public boolean writeChunkDescDB (File out, ChunkDescDB db) {
        try {
            DataOutputStream ostream = new DataOutputStream (new FileOutputStream (out));
            ostream.writeBytes (db.xmlRep());
            return true;
        }
        catch (Exception e) {
            return false;
        }
    }

    
    /** temporary hack to avoid breaking apis... */
    public boolean buildChunkDescDB (ChunkDescDB db, Node doc) {
        ConfigIOStatus iostatus = new ConfigIOStatus();
        buildChunkDescDB (db, doc, iostatus);
        return (iostatus.getStatus() == iostatus.SUCCESS || iostatus.getStatus() == iostatus.WARNINGS);
    }

    /** Converts a DOM tree to a ChunkDescDB.
     *  This method reads the DOM tree rooted at doc, and puts all
     *  ChunkDescs it finds into db.  doc can be a DOCUMENT, 
     *  DOCUMENT_FRAGMENT (with the first element being "<ChunkDescDB">)
     *  or an ELEMENT (labeled "<ChunkDescDB>").
     *
     *  @returns False if errors were encountered, otherwise true.  If
     *           false, some ChunkDescs may still have been read into db.
     */
    public void buildChunkDescDB (ChunkDescDB db, Node doc, 
                                     ConfigIOStatus iostatus) {
        Node child;
        ChunkDesc desc;
        String name = doc.getNodeName();
        
        switch (doc.getNodeType()) {
        case Node.DOCUMENT_NODE:
        case Node.DOCUMENT_FRAGMENT_NODE:
            child = doc.getFirstChild();
            while (child != null) {
                buildChunkDescDB (db, child, iostatus);
                child = child.getNextSibling();
            }
            break;
        case Node.ELEMENT_NODE:
            if (name.equalsIgnoreCase ("ChunkDescDB")) {
                child = doc.getFirstChild();
                while (child != null) {
                    if (child.getNodeType() == Node.ELEMENT_NODE) {
                        //System.out.println ("got a child of ChunkDescDB");
                        desc = buildChunkDesc (child, iostatus);
                        if (desc != null)
                            db.add (desc);
                    }
                    child = child.getNextSibling();
                }
            }
            else {
                iostatus.addWarning ("Unrecognized element '" + name + "'.\n");
            }
            break;
        case Node.COMMENT_NODE:
        case Node.NOTATION_NODE:
        case Node.PROCESSING_INSTRUCTION_NODE:
        case Node.TEXT_NODE:
            break;
        default:
            iostatus.addWarning ("Unrecognized DOM Node type...\n");
        }

    }



    /** Builds a ChunkDesc out of the DOM tree rooted at doc. */
    public ChunkDesc buildChunkDesc (Node doc, ConfigIOStatus iostatus) {
        Node child;
        NamedNodeMap attributes;
        int attrcount;
        int i;
        ChunkDesc desc = null;
        String name = doc.getNodeName();

        switch (doc.getNodeType()) {
        case Node.DOCUMENT_NODE:
        case Node.DOCUMENT_FRAGMENT_NODE:
            child = doc.getFirstChild();
            return buildChunkDesc (child, iostatus);
        case Node.ELEMENT_NODE:
            if (name.equalsIgnoreCase ("ChunkDesc")) {
                desc = new ChunkDesc ();
                // parse attributes
                attributes = doc.getAttributes();
                attrcount = attributes.getLength();
                for (i = 0; i < attrcount; i++) {
                    child = attributes.item(i);
                    if (child.getNodeName().equalsIgnoreCase ("token"))
                        desc.setToken (child.getNodeValue());
                    else if (child.getNodeName().equalsIgnoreCase ("name"))
                        desc.setName (child.getNodeValue());
                }
                if (desc.getName().equals (""))
                    desc.setName (desc.getToken());
                // parse child elements
                child = doc.getFirstChild();
                while (child != null) {
                    //buildProperty (ch, child);
                    parseChunkDescChildElement (desc, child);
                    child = child.getNextSibling();
                }
            }
            else {
                System.out.println ("Error: buildChunkDesc - unrecognized element '" + name + "'\n");
                //retval = false;
            }
            break;
        case Node.COMMENT_NODE:
        case Node.NOTATION_NODE:
        case Node.PROCESSING_INSTRUCTION_NODE:
        case Node.TEXT_NODE:
            break;
        default:
            System.out.println ("Unexpected node type...");
        }

        return desc;
    }
        


    /** Parse a child element of a ChunkDesc element.
     *  This is a utility function for buildChunkDesc.
     *  The children of ChunkDesc nodes include PropertyDesc, help,
     *  and Defaults nodes.
     */
    private boolean parseChunkDescChildElement (ChunkDesc desc, Node doc) {
        String name = doc.getNodeName();
        String value = doc.getNodeValue();
        boolean retval = true;
        Node child;
        String childname;
        String childval;
        NamedNodeMap attributes;
        int attrcount;
        int valindex = 0;

        switch (doc.getNodeType()) {
        case Node.ELEMENT_NODE:
            if (name.equalsIgnoreCase ("PropertyDesc")) {
                PropertyDesc p = new PropertyDesc();
                // parse attributes
                attributes = doc.getAttributes();
                attrcount = attributes.getLength();
                for (int i = 0; i < attrcount; i++) {
                    child = attributes.item(i);
                    childname = child.getNodeName();
                    childval = child.getNodeValue();
                    if (childname.equalsIgnoreCase ("token"))
                        p.setToken (childval);
                    else if (childname.equalsIgnoreCase ("name"))
                        p.setName (childval);
                    else if (childname.equalsIgnoreCase ("type"))
                        p.setValType (ValType.getValType(childval));
                    else if (childname.equalsIgnoreCase ("num")) {
                        if (childval.equalsIgnoreCase ("var") ||
                            childval.equalsIgnoreCase ("variable"))
                            p.setNumValues (-1);
                        else {
                            p.setNumValues (Integer.parseInt (childval));
                        }
                    }
                    else if (childname.equalsIgnoreCase ("userlevel")) {
                        if (childval.equalsIgnoreCase ("expert"))
                            p.setUserLevel (1);
                        else if (childval.equalsIgnoreCase ("beginner"))
                            p.setUserLevel (0);
                    }
                }
                if (p.getName().equals (""))
                    p.setName (p.getToken());
                // parse child elements
                child = doc.getFirstChild();
                while (child != null) {
                    parsePropertyDescChildElement (p, child);
                    child = child.getNextSibling();
                }
                desc.addPropertyDesc (p);
            }
            else if (name.equalsIgnoreCase ("help")) {
                String h = "";
                child = doc.getFirstChild();
                while (child != null) {
                    switch (child.getNodeType()) {
                    case Node.TEXT_NODE:
                        h = h + child.getNodeValue();
                        break;
                    default:
                        System.out.println ("parser confused about an element value: '" + child.getNodeValue() + "'\n");
                    }
                    child = child.getNextSibling();
                }
                desc.setHelp (h);
            }
            else if (name.equalsIgnoreCase ("Defaults")) {
                child = doc.getFirstChild();
                while (child != null) {
                    switch (child.getNodeType()) {
                    case Node.ELEMENT_NODE:
                        // we can't parse the chunk until we're done building
                        // the desc and stick it into the ChunkFactory's DescDB.
                        desc.setDefaultChunk (child);
                        break;
                    case Node.COMMENT_NODE:
                    case Node.NOTATION_NODE:
                    case Node.PROCESSING_INSTRUCTION_NODE:
                    case Node.TEXT_NODE:
                        // it's just white space & crap like that.
                        break;
                    default:
                        //vjDEBUG(vjDBG_CONFIG,2) << 
                        //  "Warning: vjXMLConfigIOHandler:"
                        //  "parsing ChunkDesc defaults value: "
                        //  "Unexpected DOM Node.\n" << vjDEBUG_FLUSH;
                    }
                    child = child.getNextSibling();
                }
                //vjDEBUG(vjDBG_CONFIG,2) << 
                //  "Warning: vjXMLConfigIOHandler: parsing ChunkDesc Defaults: "
                //  "No argument for <Defaults> tag.\n" << vjDEBUG_FLUSH;
            }
            else {
                System.out.println ("whoa... unexpected element " + name);
                retval = false;
            }
            break;
        case Node.COMMENT_NODE:
        case Node.NOTATION_NODE:
        case Node.PROCESSING_INSTRUCTION_NODE:
        case Node.TEXT_NODE:
            break;
        default:
            System.out.println ("Unexpected node type...");
        }

        return retval;
    }



    /** Parses the child Nodes of a PropertyDesc.
     *  Utility function for parseChunkDescChildElement.
     *  The children of a PropertyDesc node include label, enumeration,
     *  and help nodes.
     */
    private boolean parsePropertyDescChildElement (PropertyDesc p, Node doc) {
        String name = doc.getNodeName();
        String value = doc.getNodeValue();
        boolean retval = true;
        Node child;
        String childname;
        String childval;
        NamedNodeMap attributes;
        int attrcount;
        int valindex = 0;

        switch (doc.getNodeType()) {
        case Node.ELEMENT_NODE:
            if (name.equalsIgnoreCase ("label")) {
                String enumname = "";
                attributes = doc.getAttributes();
                attrcount = attributes.getLength();
                for (int i = 0; i < attrcount; i++) {
                    child = attributes.item(i);
                    childname = child.getNodeName();
                    if (childname.equalsIgnoreCase ("name"))
                        enumname = child.getNodeValue();
                    else
                        System.out.println ("unidentified attribute: " + childname);
                }
                if (!enumname.equals(""))
                    p.appendValueLabel (enumname);

            }
            else if (name.equalsIgnoreCase ("enumeration")) {
                // parse attributes
                String enumname = "", enumval = "";
                attributes = doc.getAttributes();
                attrcount = attributes.getLength();
                for (int i = 0; i < attrcount; i++) {
                    child = attributes.item(i);
                    childname = child.getNodeName();
                    if (childname.equalsIgnoreCase ("name"))
                        enumname = child.getNodeValue();
                    else if (childname.equalsIgnoreCase ("value"))
                        enumval = child.getNodeValue();
                    else
                        System.out.println ("unidentified attribute: " + childname);
                }
                if (!enumname.equals(""))
                    p.appendEnumeration (enumname, enumval);

            }
            else if (name.equalsIgnoreCase ("help")) {
                String h = "";
                child = doc.getFirstChild();
                while (child != null) {
                    switch (child.getNodeType()) {
                    case Node.TEXT_NODE:
                        h = h + child.getNodeValue();
                        break;
                    default:
                        System.out.println ("parser confused about an element value: '" + child.getNodeValue() + "'\n");
                    }
                    child = child.getNextSibling();
                }
                p.setHelp (h);
            }
            else {
                System.out.println ("whoa... unexpected element " + name);
                retval = false;
            }
            break;
        case Node.COMMENT_NODE:
        case Node.NOTATION_NODE:
        case Node.PROCESSING_INSTRUCTION_NODE:
        case Node.TEXT_NODE:
            break;
        default:
            System.out.println ("Unexpected node type...");
        }

        return retval;
    }



}
