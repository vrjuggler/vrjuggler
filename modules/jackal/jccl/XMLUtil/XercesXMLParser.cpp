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

#include <jccl/XMLUtil/XercesXMLParser.h>

#include <util/PlatformUtils.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <framework/XMLFormatter.hpp>
#include <util/TranscodingException.hpp>
#include <sax/SAXParseException.hpp>

#include <dom/DOM_DOMException.hpp>

#include <parsers/DOMParser.hpp>
#include <dom/DOM.hpp>
//#include <string.h>
#include <stdlib.h>

#include <jccl/XMLUtil/XercesStreamInputSource.h>
#include <jccl/XMLUtil/XercesXMLError.h>
#include <vpr/Util/Debug.h>

namespace jccl {

XercesXMLParser::XercesXMLParser () {
    // Xerces will have been initialized by the parser pool.
    parser = new DOMParser;
    parser->setValidationScheme(DOMParser::Val_Auto);
    parser->setDoNamespaces(false);
    error_handler = new XercesXMLError();
    parser->setErrorHandler(error_handler);
    parser->setCreateEntityReferenceNodes(false);
    parser->setToCreateXMLDeclTypeNode(true);
    
}


/*virtual*/ XercesXMLParser::~XercesXMLParser () {
    delete parser;
    delete error_handler;
}


bool XercesXMLParser::readFile (const std::string& file_name, DOM_Node& doc) {
    // we duplicate a lot of code in readFile & readStream just so we can
    // always use the 'best' version of parse - if we give it an actual
    // file name, we get better error messages from the exceptions.
    bool retval = true;
    try {
        parser->parse(file_name.c_str());
    }
    catch (const XMLException& e) {
        vprDEBUG(vprDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") <<
            "XercesXMLParser threw XMLException: " << e.getMessage() <<
            "\n" << vprDEBUG_FLUSH;
        retval = false;
    }
    catch (SAXParseException& e) {
        char* id = (DOMString(e.getSystemId())).transcode();
        char* msg = (DOMString(e.getMessage())).transcode();
        vprDEBUG (vprDBG_ERROR,1) << clrOutNORM (clrRED, "ERROR:") << 
            " in file \"" << id << "\", line " << e.getLineNumber() << 
            ", column " << e.getColumnNumber() << ": " << 
            msg << "\n" << vprDEBUG_FLUSH;
        delete[] id;
        delete[] msg;
        retval = false;
    }
    // Supposedly this one will never happen.
//     catch (const DOM_DOMException& e) {
//         vprDEBUG(vprDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") <<
//             "XercesXMLParser threw DOMException: " << //e.getMessage() <<
//             "\n" << vprDEBUG_FLUSH;
//         retval = false;
//     }
    catch (...) {
        vprDEBUG(vprDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") <<
            " XercesXMLParser threw unidentified exception.\n" << 
            vprDEBUG_FLUSH;
        retval = false;
    }
    if (retval) {
        // retval == true so we're still ok
        // build a ConfigChunkDB out of what's in the DOM tree
        doc = parser->getDocument();
    }
    return retval;
}


bool XercesXMLParser::readStream (std::istream& input, DOM_Node& doc) {
    XercesStreamInputSource input_source (input, "</protocol>");
    bool retval = true;
    try {
        parser->parse(input_source);
    }
    catch (const XMLException& e) {
        vprDEBUG(vprDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") <<
            "XercesXMLParser threw XMLException: " << e.getMessage() <<
            "\n" << vprDEBUG_FLUSH;
        retval = false;
    }
    catch (SAXParseException& e) {
        char* msg = (DOMString(e.getMessage())).transcode();
        vprDEBUG (vprDBG_ERROR,1) << clrOutNORM (clrRED, "ERROR:") << 
            " in file \"<unnamed stream>\", line " << e.getLineNumber() << 
            ", column " << e.getColumnNumber() << ": " << 
            msg << "\n" << vprDEBUG_FLUSH;
        delete[] msg;
        retval = false;
    }
    // Supposedly this one will never happen.
//     catch (const DOM_DOMException& e) {
//         vprDEBUG(vprDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") <<
//             "XercesXMLParser threw DOMException: " << //e.getMessage() <<
//             "\n" << vprDEBUG_FLUSH;
//         retval = false;
//     }
    catch (...) {
        vprDEBUG(vprDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") <<
            " XercesXMLParser threw unidentified exception.\n" << 
            vprDEBUG_FLUSH;
        retval = false;
    }
    if (retval) {
        // retval == true so we're still ok
        // build a ConfigChunkDB out of what's in the DOM tree
        doc = parser->getDocument();
    }
    return retval;
}

// used for the writer...
class vjDOMPrintFormatTarget: public XMLFormatTarget {
private:
    std::ostream* out;
public:
    vjDOMPrintFormatTarget (std::ostream& _out) {
        out = &_out;
    }

    virtual ~vjDOMPrintFormatTarget () {
    }

    void writeChars (const XMLByte* const buf, const unsigned int buflen,
                     XMLFormatter* const formatter) {
        out->write((char*)buf, buflen);
    }
};


bool XercesXMLParser::writeFile (const std::string& file_name, DOM_Node& doc) {
    std::ofstream out (file_name.c_str());
    if (!out)
        return false;
    else
        return writeStream (out, doc);
}


bool XercesXMLParser::writeStream (std::ostream& output, DOM_Node& doc) {
    output << "<Not_Implemented/>" << endl;
    return false;
}

};
