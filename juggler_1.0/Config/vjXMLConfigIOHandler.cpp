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

#include <vjConfig.h>

//#include <util/PlatformUtils.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <dom/DOM.hpp>
#include <dom/DOM_Document.hpp>

#include <stdlib.h>

#include <Config/vjConfigChunkDB.h>
#include <Config/vjChunkFactory.h>
#include <Config/vjParseUtil.h>
#include <Utils/vjXercesXMLParserPool.h>
#include <Utils/vjDebug.h>

#include <Config/vjXMLConfigIOHandler.h>

// a totally cheezy kludge to avoid asserts becasue of some disagreement
// between xerces & jackal/juggler/etc.  Doing this causes the loader code
// to leak memory like a sieve.
#ifdef VJ_OS_Win32
#define JCCL_WIN32_KLUDGE
#endif

// Used for doing character conversions when writing out XML.  The idea is
// that you create a vjXMLFormatTarget wrapping your iostream and create
// an XMLFormatter wrapping the vjXMLFormatTarget.  then use 
// XMLFormatter::formatBuf() or the convenience functions 
// vjXMLConfigIOHandler::writeBuf() to write your text output.
class vjXMLFormatTarget: public XMLFormatTarget {
private:
    std::ostream* out;
public:
    vjXMLFormatTarget (std::ostream& _out) {
        out = &_out;
    }

    virtual ~vjXMLFormatTarget () {
    }

    virtual void writeChars (const XMLByte* const buf, 
                             const unsigned int buflen,
                             XMLFormatter* const formatter) {
        out->write((char*)buf, buflen);
    }
};




vjXMLConfigIOHandler::vjXMLConfigIOHandler (): vjConfigIOHandler () {
    ;
}


/*virtual*/ vjXMLConfigIOHandler::~vjXMLConfigIOHandler () {
    ;
}



/*virtual*/ bool vjXMLConfigIOHandler::readConfigChunkDB (const std::string& filename, vjConfigChunkDB& db) {
    bool retval = true;
    DOM_Node doc;
    vjXercesXMLParser* p = vjXercesXMLParserPool::instance()->getParser();
    if (!p) {
        vjDEBUG(vjDBG_ERROR, 0) << clrOutNORM(clrRED, "ERROR:") 
                                << " Couldn't get XML Parser; loading '"
                                << filename.c_str() << "' failed.\n" 
                                << vjDEBUG_FLUSH;
        return false;
    }

    retval = p->readFile (filename, doc);
    if (retval) {
        // retval == true so we're still ok
        // build a ConfigChunkDB out of what's in the DOM tree
        db.setFileName (filename);
        retval = buildChunkDB (db, doc);
    }
    else
        vjDEBUG(vjDBG_ERROR, 0) << clrOutNORM(clrRED, "ERROR:")
                                << " reading XML file '" 
                                << filename.c_str() << "' failed.\n" 
                                << vjDEBUG_FLUSH;

    return retval;
}


/*virtual*/ bool vjXMLConfigIOHandler::readConfigChunkDB (std::istream& input, vjConfigChunkDB& db) {

    bool retval = true;
    DOM_Node doc;
    vjXercesXMLParser* p = vjXercesXMLParserPool::instance()->getParser();
    if (!p) {
        vjDEBUG(vjDBG_ERROR, 0) << clrOutNORM(clrRED, "ERROR:")
                                << " Couldn't get XML Parser; loading "
            "ConfigChunkDB failed.\n" << vjDEBUG_FLUSH;
        return false;
    }

    retval = p->readStream (input, doc);
    if (retval) {
        // retval == true so we're still ok
        // build a ConfigChunkDB out of what's in the DOM tree
        retval = buildChunkDB (db, doc);
    }
    else
        vjDEBUG(vjDBG_ERROR, 0) << clrOutNORM(clrRED, "ERROR:")
                                << " reading XML stream failed.\n" 
                                << vjDEBUG_FLUSH;
    return retval;
}



/*virtual*/ bool vjXMLConfigIOHandler::writeConfigChunkDB (std::ostream& out, const vjConfigChunkDB& db) {
    vjXMLFormatTarget target(out);
    XMLFormatter formatter ("USASCII", &target, 
                            XMLFormatter::NoEscapes, 
                            XMLFormatter::UnRep_Replace);

    bool retval = true;
    vjConfigChunkDB::const_iterator it;
    writeBuf (&formatter, "<ConfigChunkDB>\n");
    for (it = db.begin(); (it != db.end()) && retval; it++) {
        retval = writeConfigChunk (&formatter, **it, std::string("    "));
        out << std::flush;
    }
    writeBuf (&formatter, "</ConfigChunkDB>\n");
    return retval;
}


/*virtual*/ bool vjXMLConfigIOHandler::writeConfigChunk (std::ostream& out, const vjConfigChunk& ch, const std::string& pad) {
    vjXMLFormatTarget target(out);
    XMLFormatter formatter ("USASCII", &target, 
                            XMLFormatter::NoEscapes, 
                            XMLFormatter::UnRep_Replace);
    return writeConfigChunk (&formatter, ch, pad);
}


bool vjXMLConfigIOHandler::writeConfigChunk (XMLFormatter* f, const vjConfigChunk& ch, const std::string& pad) {
    bool retval = true;
    std::string newpad = pad + pad;
    vjConfigChunk::const_iterator it;
    writeBuf (f, pad);
    writeBuf (f, "<");
    writeBuf (f, ch.getDescToken());
    writeBuf (f, " name=\"");
    writeBuf (f, ch.getName(), XMLFormatter::AttrEscapes);
    writeBuf (f, "\">\n");

    for (it = ch.begin(); (it != ch.end()) && retval; it++)
        if (vjstrcasecmp((*it)->getToken(), "name"))
            retval = writeProperty (f, **it, newpad);

    writeBuf (f, pad);
    writeBuf (f, "</");
    writeBuf (f, ch.getDescToken());
    writeBuf (f, ">\n");
    return retval;
}


bool vjXMLConfigIOHandler::writeProperty (XMLFormatter* f, const vjProperty& p, const std::string& pad) {
    writeBuf (f, pad);
    writeBuf (f, "<");
    writeBuf (f, p.getToken());
    writeBuf (f, ">");
    VarType t = p.getType();
    std::string newpad = pad + "    ";
    unsigned int i;
    unsigned int n = p.value.size();

    if (t == T_EMBEDDEDCHUNK) {
        writeBuf (f, "\n");
        for (i = 0; i < n; i++) {
            // ick... wish we weren't copying a chunk here
            vjConfigChunk *ch = p.getValue(i);
            writeConfigChunk (f, *ch, newpad);
            delete (ch);
        }
        writeBuf (f, pad);
    }
    else {
        for (i = 0; i < n; i++) {
            vjVarValue *v = ((p.value))[i];
            if ((t == T_STRING) || (t == T_CHUNK)) {
                writeBuf (f, "\"");
                writeBuf (f, (std::string)*v, XMLFormatter::CharEscapes);
                writeBuf (f, "\"");
            }
            else {
                writeBuf (f, (std::string)*v, XMLFormatter::CharEscapes);
            }
            if (i < n-1)
                writeBuf (f, " ");
        }
    }

    writeBuf (f, "</");
    writeBuf (f, p.getToken());
    writeBuf (f, ">\n");
    return true;
}


bool vjXMLConfigIOHandler::parseTextValues (vjProperty* p, int& startval, char* text) {
    // the string could be something like "56 63 22" and we need to
    // get values out of it.  sigh.
    char *ch, *ptr;
    bool retval = true;

    switch (p->getType()) {
    case T_STRING:
    case T_CHUNK:
        // need to cut apart if there are multiple quote-separated strings.
        ch = stringTokenizer (text, ptr);
        for (;retval;) {
            if (!ch)
                break;
            retval = p->tryAssign (startval++, ch);
            ch = stringTokenizer (0, ptr);
        }
        break;
    case T_INT:
    case T_FLOAT:
    case T_BOOL:
        // these 3 we need to cut text apart on whitespace to get 
        // individual values.
#ifdef HAVE_STRTOK_R
        ch = strtok_r (text, " \t\n", &ptr);
#else
        ch = strtok (text, " \t\n");
#endif
        for (;retval;) {
            if (!ch)
                break;
            ch = stripQuotes(ch);
            retval = p->tryAssign(startval++, ch);
#ifdef HAVE_STRTOK_R
            ch = strtok_r (0, " \t\n", &ptr);
#else
            ch = strtok (0, " \t\n");
#endif
        }
        break;
    case T_EMBEDDEDCHUNK:
        // not handled here
        break;
    default:
        ;
    }

    if (!retval) {
        vjDEBUG(vjDBG_CONFIG,0) << clrOutNORM(clrRED, "ERROR:") << 
            "vjXMLConfigIOHandler: failed tryAssign of '" << ch << 
            "' to property '" << p->getToken() << "'.\n" <<
            vjDEBUG_FLUSH;
    }
    
    return retval;
}


bool vjXMLConfigIOHandler::buildProperty (vjConfigChunk* ch, const DOM_Node& doc, bool use_defaults) {
    char* name = doc.getNodeName().transcode();
    char* value = doc.getNodeValue().transcode();
    DOM_Node child;
    vjProperty* p;
    int valindex = 0;
    bool retval = true;
    vjConfigChunk* ch2;

    switch (doc.getNodeType()) {
    case DOM_Node::ELEMENT_NODE:
        p = ch->getPropertyPtrFromToken (name);
        if (p) {
            child = doc.getFirstChild();
            while (retval && (child != 0)) {
                if (p->getType() == T_EMBEDDEDCHUNK) {
                    switch (child.getNodeType()) {
                    case DOM_Node::ELEMENT_NODE:
                        ch2 = buildConfigChunk (child, use_defaults);
                        if (ch2)
                            p->setValue (ch2, valindex++);
                        else {
                            vjDEBUG(vjDBG_CONFIG,0) << 
                                clrOutNORM(clrRED, "ERROR:") << 
                                "Reading embedded chunk failed: " << 
                                ch->getDescToken() << "->" << name << 
                                ".\n" << vjDEBUG_FLUSH;
                            retval = false;
                        }
                        break;
                    case DOM_Node::TEXT_NODE:
                    case DOM_Node::NOTATION_NODE:
                    case DOM_Node::PROCESSING_INSTRUCTION_NODE:
                    case DOM_Node::COMMENT_NODE:
                        break;
                    default:
                        vjDEBUG(vjDBG_CONFIG,2) << "vjXMLConfigIOHandler::"
                            "buildProperty: Unexpected DOM_Node.\n" << 
                            vjDEBUG_FLUSH;
                        retval = false;
                    }

                }
                else {
                    switch (child.getNodeType()) {
                    case DOM_Node::TEXT_NODE:
                        retval = parseTextValues (p, valindex, child.getNodeValue().transcode());
                        break;
                    default:
                        vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler::buildProperty(): Unexpected XML Node.\n" << vjDEBUG_FLUSH;
                        break;
                    }
                }
                child = child.getNextSibling();
            }
        }
        else {
            retval = false;
            vjDEBUG(vjDBG_CONFIG,0) << clrOutNORM(clrRED, "ERROR:")
                                    << " Property doesn't exist: '" 
                                    << ch->getDescToken() << "->"
                                    << name << "'.\n" << vjDEBUG_FLUSH;
        }
        break;
    case DOM_Node::ATTRIBUTE_NODE:
        //cout << "parsing property attribute" <<std::endl;
        p = ch->getPropertyPtrFromToken (name);
        if (p) {
            retval = parseTextValues (p, valindex, value);
        }
        else {
            retval = false;
            vjDEBUG(vjDBG_CONFIG,0) << clrOutNORM(clrRED, "ERROR:")
                                    << " Property doesn't exist: '" 
                                    << ch->getDescToken() << "->"
                                    << name << "'.\n" << vjDEBUG_FLUSH;
        }
        break;
    case DOM_Node::TEXT_NODE:
    case DOM_Node::NOTATION_NODE:
    case DOM_Node::PROCESSING_INSTRUCTION_NODE:
    case DOM_Node::COMMENT_NODE:
        break;
    default:
        vjDEBUG(vjDBG_CONFIG,2) << "vjXMLConfigIOHandler::buildProperty:"
            " Unexpected DOM_Node.\n" << vjDEBUG_FLUSH;
    }
#ifndef JCCL_WIN32_KLUDGE
    delete[] name;
    delete[] value;
#endif
    return retval;
}



vjConfigChunk* vjXMLConfigIOHandler::buildConfigChunk (const DOM_Node& doc, bool use_defaults) {
    char* name = doc.getNodeName().transcode();
    DOM_Node child;
    DOM_NamedNodeMap attributes;
    int attrCount;
    int i;
    vjConfigChunk* ch = 0;
    bool retval = true;

    switch (doc.getNodeType()) {
    case DOM_Node::DOCUMENT_NODE:
        child = ((const DOM_Document*)&doc)->getDocumentElement();
        ch = buildConfigChunk (child);
        break;
    case DOM_Node::ELEMENT_NODE:
        ch = vjChunkFactory::instance()->createChunk(name, use_defaults);
        if (ch) {
            // parse attributes
            attributes = doc.getAttributes();
            attrCount = attributes.getLength();
            for (i = 0; retval && (i < attrCount); i++) {
                child = attributes.item(i);
                retval = buildProperty (ch, child, use_defaults);
            }
            
            // parse child elements
            child = doc.getFirstChild();
            while (retval && (child != 0)) {
                retval = buildProperty (ch, child, use_defaults);
                child = child.getNextSibling();
            }
        }
        else {
            vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << 
                " vjXMLConfigIOHandler::buildConfigChunk: Creating ConfigChunk"
                " failed: Couldn't find ChunkDesc '" << name << 
                "'.\n" << vjDEBUG_FLUSH;
            retval = false;
        }
        break;
    case DOM_Node::TEXT_NODE:
    case DOM_Node::NOTATION_NODE:
    case DOM_Node::PROCESSING_INSTRUCTION_NODE:
    case DOM_Node::COMMENT_NODE:
        break;
    default:
        vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler::build"
            "ConfigChunk: Unexpected DOM_Node.\n" << vjDEBUG_FLUSH;
    }
#ifndef JCCL_WIN32_KLUDGE
    delete[] name;
#endif
    if (!retval) {
        // in the event of a failure, we completely skip the chunk. 
        // don't risk actually sending on some mangled thing.
        if (ch) {
            vjDEBUG(vjDBG_CONFIG,5) << "Rejecting ConfigChunk due to errors:\n"
                                    << *ch << vjDEBUG_FLUSH;
            // delete ch;
            ch = 0;
        }
    }
    return ch;
}



bool vjXMLConfigIOHandler::buildChunkDB (vjConfigChunkDB& db, const DOM_Node& doc) {
    char* name = doc.getNodeName().transcode();
    DOM_Node child;
    vjConfigChunk* ch;
    bool retval = true;

    switch (doc.getNodeType()) {
    case DOM_Node::DOCUMENT_NODE:
        child = ((const DOM_Document*)&doc)->getDocumentElement();
        retval = buildChunkDB (db, child);
        break;
    case DOM_Node::ELEMENT_NODE:
        if (!vjstrcasecmp (name, "ConfigChunkDB")) {
            child = doc.getFirstChild();
            while (child != 0) {
                if (child.getNodeType() == DOM_Node::ELEMENT_NODE) {
                    ch = buildConfigChunk (child);
                    // look for includes...
                    if (ch) {
                        if (!vjstrcasecmp (ch->getType(), "vjIncludeFile")) {
                            std::string s = ch->getProperty ("Name");
                            std::string fname = db.getFileName();
                            retval = retval && db.load (s, fname);
                            // set filename back to what it was...
                            db.setFileName(fname);
                        }
                        else if (!vjstrcasecmp (ch->getType(), 
                                                "vjIncludeDescFile")) {
                            // the descs could be needed to parse the rest of
                            // the tree so load 'em now.
                            std::string s = ch->getProperty ("Name");
                            vjChunkDescDB newdb;
                            retval = retval && newdb.load (s, db.getFileName());
                            vjChunkFactory::instance()->addDescs (&newdb);
                        }
                        else {
                            db.addChunk(ch);
                        }
                    }
                }
                child = child.getNextSibling();
            }
        }
        else {
            retval = false;
            vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << 
                " vjXMLConfigIOHandler::buildConfigChunkDB: Couldn't find "
                "<ConfigChunkDB> tag.\n" << vjDEBUG_FLUSH;
        }
        break;
    case DOM_Node::TEXT_NODE:
    case DOM_Node::NOTATION_NODE:
    case DOM_Node::PROCESSING_INSTRUCTION_NODE:
    case DOM_Node::COMMENT_NODE:
        break;
    default:
        retval = false;
        vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler::build"
            "ConfigChunkDB: Unexpected DOM_Node.\n" << vjDEBUG_FLUSH;
    }
#ifndef JCCL_WIN32_KLUDGE
    delete[] name;
#endif
    return retval;
}


//----------------------- ChunkDescDB Methods -----------------------------

/*virtual*/ bool vjXMLConfigIOHandler::readChunkDescDB (const std::string& filename, vjChunkDescDB& db) {
    bool retval = true;
    DOM_Node doc;
    vjXercesXMLParser* p = vjXercesXMLParserPool::instance()->getParser();
    if (!p)
        return false;

    retval = p->readFile (filename, doc);
    if (retval) {
        // retval == true so we're still ok
        retval = buildChunkDescDB (db, doc);
    }

    // this absolutely positively cannot be permitted.  why?  because
    // this loadChunkDescDB can get called from inside a ChunkFactory
    // method, and it can (becuase of default parsing) try to call a
    // ChunkFactory method, and the vjSingleton::instance() interface
    // doesn't seem to be recursive-safe.
//      std::cout << "Just read the following file:\n";
//      writeChunkDescDB (std::cout, db);
//      std::cout << "done.\n";

    return retval;
}


/*virtual*/ bool vjXMLConfigIOHandler::readChunkDescDB (std::istream& input, vjChunkDescDB& db) {

    bool retval = true;
    DOM_Node doc;
    vjXercesXMLParser* p = vjXercesXMLParserPool::instance()->getParser();
    if (!p)
        return false;

    retval = p->readStream (input, doc);
    if (retval) {
        // retval == true so we're still ok
        retval = buildChunkDescDB (db, doc);
    }
    return retval;
}


/*virtual*/ bool vjXMLConfigIOHandler::writeChunkDescDB (std::ostream& output, const vjChunkDescDB& db) {
    vjXMLFormatTarget target(output);
    XMLFormatter formatter("USASCII", &target, 
                           XMLFormatter::NoEscapes, 
                           XMLFormatter::UnRep_Replace);
    bool retval = true;
    vjChunkDescDB::const_iterator it;
    writeBuf (&formatter, "<ChunkDescDB>\n");
    for (it = db.begin(); (it != db.end()) && retval; it++) {
        retval = writeChunkDesc (&formatter, **it, std::string("  "));
    }
    writeBuf (&formatter, "</ChunkDescDB>\n");
    return retval;
}


/*virtual*/ bool vjXMLConfigIOHandler::writeChunkDesc (std::ostream& output, const vjChunkDesc& desc, const std::string& pad) {
    vjXMLFormatTarget target(output);
    XMLFormatter f ("USASCII", &target, 
                    XMLFormatter::NoEscapes, 
                    XMLFormatter::UnRep_Replace);
    return writeChunkDesc (&f, desc, pad);
}


bool vjXMLConfigIOHandler::writeChunkDesc (XMLFormatter* f, const vjChunkDesc& desc, const std::string& pad) {
    std::string newpad = pad + "  ";
    bool retval = true;
    vjConfigChunk* ch;
    vjChunkDesc::const_iterator it;
    writeBuf (f, "<ChunkDesc token=\"");
    writeBuf (f, desc.getToken(), XMLFormatter::AttrEscapes);
    writeBuf (f, "\" name=\"");
    writeBuf (f, desc.getName(), XMLFormatter::AttrEscapes);
    writeBuf (f, "\">\n");
    if (desc.getHelp() != "") {
        writeBuf (f, "<help>");
        writeBuf (f, desc.getHelp(), XMLFormatter::CharEscapes);
        writeBuf (f, "</help>\n");
    }
    for (it = desc.begin(); (it != desc.end()) && retval; it++) {
        retval = writePropertyDesc (f, **it, newpad);
    }
    std::cout << "about to write defaults..." << std::endl;
    if ((ch = desc.getDefaultChunk()) != 0) {
        std::cout << "yup, writing defaults..." << std::endl;
        writeBuf (f, newpad);
        writeBuf (f, "<Defaults>\n");
        retval = retval && writeConfigChunk (f, *ch, newpad + "  ");
        writeBuf (f, newpad);
        writeBuf (f, "</Defaults>\n");
    }
    std::cout << "done writing defaults..." << std::endl;
    writeBuf (f, pad);
    writeBuf (f, "</ChunkDesc>\n");
    return retval;
}


bool vjXMLConfigIOHandler::writePropertyDesc (XMLFormatter* f, const vjPropertyDesc& p, const std::string& pad) {
    std::string newpad = pad + "  ";
    bool retval = true;
    int n, i;
    vjEnumEntry* e;
    VarType t = p.getVarType();

    writeBuf (f, pad);
    writeBuf (f, "<PropertyDesc token=\"");
    writeBuf (f, p.getToken(), XMLFormatter::AttrEscapes);
    writeBuf (f, "\" name=\"");
    writeBuf (f, p.getName(), XMLFormatter::AttrEscapes);
    writeBuf (f, "\" type=\"");
    writeBuf (f, typeString(t), XMLFormatter::AttrEscapes);
    writeBuf (f, "\" num=\"");
    int num = p.getNumAllowed();
    std::string numstr;
    if (num == -1)
        numstr = "variable";
    else {
        char buf[128];
        sprintf (buf, "%i", num);
        numstr = buf;
    }
    writeBuf (f, numstr, XMLFormatter::AttrEscapes);
    writeBuf (f, "\">\n");
    if (p.getHelp() != "") {
        writeBuf (f, newpad);
        writeBuf (f, "<help>");
        writeBuf (f, p.getHelp(), XMLFormatter::CharEscapes);
        writeBuf (f, "</help>\n");
    }

    n = p.getValueLabelsSize();
    for (i = 0; i < n; i++) {
        writeBuf (f, newpad);
        writeBuf (f, "<label name=\"");
        writeBuf (f, p.getValueLabel(i), XMLFormatter::AttrEscapes);
        writeBuf (f, "\"/>\n");
    }

    n = p.getEnumerationsSize();
    for (i = 0; i < n; i++) {
        e = p.getEnumEntryAtIndex(i);
        writeBuf (f, newpad);
        writeBuf (f, "<enumeration name=\"");
        writeBuf (f, e->getName(), XMLFormatter::AttrEscapes);
        if (t == T_STRING || t == T_CHUNK || t == T_EMBEDDEDCHUNK)
            writeBuf (f, "\"/>\n");
        else {
            std::string s;
            writeBuf (f, "\" value=\"");
            writeBuf (f, (std::string)e->getValue(), XMLFormatter::AttrEscapes);
            writeBuf (f, "\"/>\n");
        }
    }
    writeBuf (f, pad);
    writeBuf (f, "</PropertyDesc>\n");
    return retval;
}


bool vjXMLConfigIOHandler::buildChunkDescDB (vjChunkDescDB& db, const DOM_Node& doc) {
    char* name = doc.getNodeName().transcode();
    DOM_Node child;
    vjChunkDesc* desc;
    bool retval = true;

    //std::cout << "ok, we've got a node named '" << name << "'." <<std::endl;

    switch (doc.getNodeType()) {
    case DOM_Node::DOCUMENT_NODE:
        child = ((const DOM_Document*)&doc)->getDocumentElement();
        retval = buildChunkDescDB (db, child);
        break;
    case DOM_Node::ELEMENT_NODE:
        if (!vjstrcasecmp (name, "ChunkDescDB")) {
            child = doc.getFirstChild();
            while (child != 0) {
                if (child.getNodeType() == DOM_Node::ELEMENT_NODE) {
                    desc = buildChunkDesc (child);
                    if (desc)
                        db.insert (desc);
                }
                child = child.getNextSibling();
            }
        }
        else {
            retval = false;
            vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << 
                " vjXMLConfigIOHandler::buildChunkDescDB: Couldn't find "
                "<ChunkDescDB> tag.\n" << vjDEBUG_FLUSH;
        }
        break;
    case DOM_Node::COMMENT_NODE:
    case DOM_Node::NOTATION_NODE:
    case DOM_Node::PROCESSING_INSTRUCTION_NODE:
    case DOM_Node::TEXT_NODE:
        // it's just white space & crap like that.
        break;
    default:
        retval = false;
        vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler::build"
            "ChunkDescDB: Unexpected DOM_Node.\n" << vjDEBUG_FLUSH;
    }
#ifndef JCCL_WIN32_KLUDGE
    delete[] name;
#endif
    return retval;
}



vjChunkDesc* vjXMLConfigIOHandler::buildChunkDesc (const DOM_Node& doc) {
    char* name = doc.getNodeName().transcode();
    char* child_name;
    char* child_value;
    DOM_Node child;
    DOM_NamedNodeMap attributes;
    int attr_count;
    int i;
    vjChunkDesc* desc = NULL;
    bool retval = true;

    switch (doc.getNodeType()) {
    case DOM_Node::DOCUMENT_NODE:
        child = ((const DOM_Document*)&doc)->getDocumentElement();
        desc = buildChunkDesc (child);
        break;
    case DOM_Node::ELEMENT_NODE:
        if (!vjstrcasecmp(name, "ChunkDesc")) {
            desc = new vjChunkDesc ();
            // parse attributes
            attributes = doc.getAttributes();
            attr_count = attributes.getLength();
            for (i = 0; i < attr_count; i++) {
                child = attributes.item(i);
                child_name = child.getNodeName().transcode();
                child_value = child.getNodeValue().transcode();
                if (!vjstrcasecmp (child_name, "token"))
                    desc->setToken (child_value);
                else if (!vjstrcasecmp (child_name, "name"))
                    desc->setName (child_value);
#ifndef JCCL_WIN32_KLUDGE
                delete[] child_name;
                delete[] child_value;
#endif
            }
            if (desc->getToken() == "") {
                retval = false;
                vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") <<
                    " vjXMLConfigIOHandler::buildChunkDesc: ChunkDesc "
                    "has no Token argument.\n" << vjDEBUG_FLUSH;
            }
            if (desc->getName() == "")
                desc->setName(desc->getToken());

            // parse child elements
            child = doc.getFirstChild();
            while (retval && (child != 0)) {
                retval = parseChunkDescChildElement (*desc, child);
                child = child.getNextSibling();
            }
        }
        else {
            vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << 
                " vjXMLConfigIOHandler::buildChunkDesc: Couldn't find "
                "<ChunkDesc> tag.\n" << vjDEBUG_FLUSH;
        }
        break;
    case DOM_Node::COMMENT_NODE:
    case DOM_Node::NOTATION_NODE:
    case DOM_Node::PROCESSING_INSTRUCTION_NODE:
    case DOM_Node::TEXT_NODE:
        // it's just white space & crap like that.
        break;
    default:
        vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler::build"
            "ConfigChunk: Unexpected DOM_Node.\n" << vjDEBUG_FLUSH;
    }
#ifndef JCCL_WIN32_KLUDGE
    delete[] name;
#endif
    if (!retval) {
        // in the event of a failure, we completely skip the chunk. 
        // don't risk actually sending on some mangled thing.
        if (desc) {
            vjDEBUG(vjDBG_CONFIG,5) << "Rejecting ChunkDesc due to errors:\n"
                                    << *desc << vjDEBUG_FLUSH;
            // delete desc;
            desc = 0;
        }
    }
    return desc;
}


bool vjXMLConfigIOHandler::parseChunkDescChildElement (vjChunkDesc& desc, DOM_Node doc) {
    char* name = doc.getNodeName().transcode();
    char* child_name;
    char* child_value;
    DOM_Node child;
    DOM_NamedNodeMap attributes;
    int attr_count;
    vjPropertyDesc* p;
    bool retval = true;

    //std::cout << "ok, we've got a node named '" << name << "'." <<std::endl;

    switch (doc.getNodeType()) {
    case DOM_Node::ELEMENT_NODE:
        if (!vjstrcasecmp (name, "PropertyDesc")) {
            p = new vjPropertyDesc();
            // parse attributes
            attributes = doc.getAttributes();
            attr_count = attributes.getLength();
            for (int i = 0; i < attr_count; i++) {
                child = attributes.item(i);
                child_name = child.getNodeName().transcode();
                child_value = child.getNodeValue().transcode();
                if (!vjstrcasecmp (child_name, "token"))
                    p->setToken(child_value);
                else if (!vjstrcasecmp (child_name, "name"))
                    p->setName (child_value);
                else if (!vjstrcasecmp (child_name, "type")) {
                    p->setVarType (stringToVarType (child_value));
                }
                else if (!vjstrcasecmp (child_name, "num")) {
                    if (!vjstrncasecmp (child_value, "var", 3))
                        p->setNumAllowed (-1);
                    else
                        p->setNumAllowed (atoi(child_value));
                }
                else {
                    vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler"
                        "::parseChunkDescChildElement: Unidentified "
                        "attribute '" << child_name << "=\"" << child_value <<
                        "\"'.\n" << vjDEBUG_FLUSH;
                }
#ifndef JCCL_WIN32_KLUDGE
                delete[] child_name;
                delete[] child_value;
#endif
            }
            if (p->getToken() == "") {
                retval = false;
                vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") <<
                    " vjXMLConfigIOHandler::parseChunkDescChildElem: "
                    "PropertyDesc has no Token argument.\n" << vjDEBUG_FLUSH;
            }
            if (p->getName() == "")
                p->setName (p->getToken());

            // parse child elements
            child = doc.getFirstChild();
            while (retval && (child != 0)) {
                retval = parsePropertyDescChildElement (*p, child);
                child = child.getNextSibling();
            }
            if (retval)
                desc.add (p);
            else {
                vjDEBUG(vjDBG_CONFIG,5) << "Rejecting PropertyDesc in " <<
                    desc.getToken() << " due to errors:" << *p << 
                    std::endl << vjDEBUG_FLUSH;
                delete p;
            }
        }

        else if (!vjstrcasecmp (name, "help")) {
            std::string h = "";
            child = doc.getFirstChild();
            while (child != 0) {
                child_value = child.getNodeValue().transcode();
                switch (child.getNodeType()) {
                case DOM_Node::TEXT_NODE:
                    h += child_value;
                    break;
                case DOM_Node::COMMENT_NODE:
                case DOM_Node::NOTATION_NODE:
                case DOM_Node::PROCESSING_INSTRUCTION_NODE:
                    break;
                default:
                    vjDEBUG(vjDBG_CONFIG,2) << "vjXMLConfigIOHandler::"
                        "parseChunkDescChildElement: Unexpected DOM_Node.\n"<< 
                        vjDEBUG_FLUSH;
                }
#ifndef JCCL_WIN32_KLUDGE
                delete[] child_value;
#endif
                child = child.getNextSibling();
            }
            desc.setHelp (h);
        }
        else if (!vjstrcasecmp (name, "Defaults")) {
            child = doc.getFirstChild();
            while (child != 0) {
                switch (child.getNodeType()) {
                case DOM_Node::ELEMENT_NODE:
                    // we can't parse the chunk until we're done building
                    // the desc and stick it into the ChunkFactory's DescDB.
                    desc.setDefaultChunk (&child);
                    //break;
                    goto finished; // break out of while
                case DOM_Node::COMMENT_NODE:
                case DOM_Node::NOTATION_NODE:
                case DOM_Node::PROCESSING_INSTRUCTION_NODE:
                case DOM_Node::TEXT_NODE:
                    // it's just white space & crap like that.
                    break;
                default:
                    vjDEBUG(vjDBG_CONFIG,2) << 
                        "Warning: vjXMLConfigIOHandler:"
                        "parsing ChunkDesc defaults value: "
                        "Unexpected DOM Node.\n" << vjDEBUG_FLUSH;
                }
                child = child.getNextSibling();
            }
            vjDEBUG(vjDBG_CONFIG,2) << 
                "Warning: vjXMLConfigIOHandler: parsing ChunkDesc Defaults: "
                "No argument for <Defaults> tag.\n" << vjDEBUG_FLUSH;
        finished:
            ;
        }
        else {
            vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler"
                "::parsing ChunkDesc: Unidentified Element '" <<
                name << "'.\n" << vjDEBUG_FLUSH;
            //retval = false;
        }
        break;
    case DOM_Node::COMMENT_NODE:
    case DOM_Node::NOTATION_NODE:
    case DOM_Node::PROCESSING_INSTRUCTION_NODE:
    case DOM_Node::TEXT_NODE:
        // it's just white space & crap like that.
        break;
    default:
        vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler::"
            "parseChunkDescChildElement: "
            "Unexpected DOM Node.\n" << vjDEBUG_FLUSH;
        break;
    }
#ifndef JCCL_WIN32_KLUDGE
    delete[] name;
#endif
    return retval;
}


bool vjXMLConfigIOHandler::parsePropertyDescChildElement (vjPropertyDesc &p, DOM_Node doc) {
 
    char* name = doc.getNodeName().transcode();
    char* child_name;
    char* child_value;
    DOM_Node child;
    DOM_NamedNodeMap attributes;
    int attr_count;
    bool retval = true;
    char* enum_name = 0;
    char* enum_value = 0;

    //std::cout << "ok, we've got a node named '" << name << "'." <<std::endl;

    switch (doc.getNodeType()) {
    case DOM_Node::ELEMENT_NODE:

        if (!vjstrcasecmp (name, "label")) {
            attributes = doc.getAttributes();
            attr_count = attributes.getLength();
            for (int i = 0; i < attr_count; i++) {
                child = attributes.item(i);
                child_name = child.getNodeName().transcode();
                child_value = child.getNodeValue().transcode();
                if (!vjstrcasecmp (child_name, "name"))
                    p.appendValueLabel (child_value);
                else {
                    vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler"
                        ": Parsing ValueLabel: Unidentified "
                        "attribute '" << child_name << "=\"" << child_value <<
                        "\"'.\n" << vjDEBUG_FLUSH;
                }
#ifndef JCCL_WIN32_KLUDGE
                delete[] child_name;
                delete[] child_value;
#endif
            }
        }
        else if (!vjstrcasecmp (name, "enumeration")) {
            // parse attributes
            
            attributes = doc.getAttributes();
            attr_count = attributes.getLength();
            for (int i = 0; i < attr_count; i++) {
                child = attributes.item(i);
                child_name = child.getNodeName().transcode();
                if (!vjstrcasecmp (child_name, "name"))
                    enum_name = child.getNodeValue().transcode();
                else if (!vjstrcasecmp (child_name, "value"))
                    enum_value = child.getNodeValue().transcode();
                else {
                    vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler"
                        ": Parsing Enumeration: Unidentified "
                        "attribute '" << child_name << "=\"" << child_value <<
                        "\"'.\n" << vjDEBUG_FLUSH;
                }
#ifndef JCCL_WIN32_KLUDGE
                delete[] child_name;
#endif
            }
            if (enum_name) {
                if (enum_value)
                    p.appendEnumeration (enum_name, enum_value);
                else 
                    p.appendEnumeration (enum_name, "");
#ifndef JCCL_WIN32_KLUDGE
                delete[] enum_name;
#endif
            }
#ifndef JCCL_WIN32_KLUDGE
            if (enum_value)
                delete[] enum_value;
#endif
        }
        else if (!vjstrcasecmp (name, "help")) {
            std::string h = "";
            child = doc.getFirstChild();
            while (child != 0) {
                child_value = child.getNodeValue().transcode();
                switch (child.getNodeType()) {
                case DOM_Node::TEXT_NODE:
                    h += child_value;
                    break;
                case DOM_Node::COMMENT_NODE:
                case DOM_Node::NOTATION_NODE:
                case DOM_Node::PROCESSING_INSTRUCTION_NODE:
                    break;
                default:
                    vjDEBUG(vjDBG_CONFIG,2) << "vjXMLConfigIOHandler: "
                        "parsing PropertyDesc Help: Unexpected DOM_Node.\n" << 
                        vjDEBUG_FLUSH;
                }
#ifndef JCCL_WIN32_KLUDGE
                delete[] child_value;
#endif
                child = child.getNextSibling();
            }
            p.setHelp (h);
        }
        else {
            vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler"
                ": parsing PropertyDesc: Unidentified Element '" <<
                name << "'.\n" << vjDEBUG_FLUSH;
            //retval = false;
        }
        break;
    case DOM_Node::COMMENT_NODE:
    case DOM_Node::NOTATION_NODE:
    case DOM_Node::PROCESSING_INSTRUCTION_NODE:
    case DOM_Node::TEXT_NODE:
        // it's just white space & crap like that.
        break;
    default:
        vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler::"
            "parsePropertyDescChildElement: "
            "Unexpected DOM Node.\n" << vjDEBUG_FLUSH;
        break;
    }
#ifndef JCCL_WIN32_KLUDGE
    delete[] name;
#endif
    return retval;
}


//------------------ private parser functions ------------------------

char* vjXMLConfigIOHandler::stripQuotes (char* ch) {
    if (ch[0] == '"')
        ch++;
    int n = strlen(ch)-1;
    if (ch[n] == '"')
        ch[n] = 0;
    return ch;
}



char* vjXMLConfigIOHandler::stringTokenizer (char* newstring, char*& tmp) {
    // concept based on strtok_r
    if (newstring) {
        tmp = newstring;
    }
    char *begin, *end;
    //int len = strlen (tmp);
    begin = strchr (tmp, '"');
    if (!begin) {
        if (newstring != 0)
            // then it was just a single-word unquoted string.
            return stripQuotes (newstring);
        else
            return 0;
    }
    end = strchr (begin+1, '"');
    if (!end) {
        vjDEBUG(vjDBG_CONFIG,2) << "Warning: vjXMLConfigIOHandler: " <<
            "Unterminated string '" << begin+1 << "'.\n" <<
            vjDEBUG_FLUSH;
        return 0;
    }
    *end = 0;
    tmp = end+1;
    return begin+1;
}


void vjXMLConfigIOHandler::writeBuf (XMLFormatter* formatter, const char* s, XMLFormatter::EscapeFlags flags) {
    DOMString st(s);
    formatter->formatBuf (st.rawBuffer(), st.length(), flags);
}

void vjXMLConfigIOHandler::writeBuf (XMLFormatter* formatter, const std::string& s, XMLFormatter::EscapeFlags flags) {
    DOMString st(s.c_str());
    formatter->formatBuf (st.rawBuffer(), st.length(), flags);
}
