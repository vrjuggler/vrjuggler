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

#include <vrj/vrjConfig.h>
#include <ctype.h>
#include <vrj/Config/ConfigChunk.h>
#include <vrj/Util/Debug.h>
#include <vrj/Config/ParseUtil.h>
#include <vrj/Config/ConfigTokens.h>
#include <vrj/Config/VarValue.h>

namespace vrj
{

VarValue* VarValue::invalid_instance = NULL;
const std::string VarValue::using_invalid_msg = "Casting from VJ_T_INVALID VarValue - this may mean we're confused";


/*static*/ VarValue& VarValue::getInvalidInstance () {
    if (invalid_instance == NULL)
        invalid_instance = new VarValue (VJ_T_INVALID);
    return *invalid_instance;
}


VarValue::VarValue (const VarValue &v)
{
    validation = 1;

    //strval = std::string("");
    intval = 0;
    floatval = 0.0;
    embeddedchunkval = NULL;
    boolval = false;
    *this = v;
}


VarValue::VarValue (const ConfigChunk* ch)
{
    validation = 1;

    //strval = std::string("");
    intval = 0;
    floatval = 0.0;
    embeddedchunkval = NULL;
    boolval = false;
    type = T_EMBEDDEDCHUNK;
    if (ch)
        embeddedchunkval = new ConfigChunk(*ch);
}


VarValue::VarValue ( VarType t )
{
    validation = 1;

    //strval = std::string("");
    type = t;
    intval = 0;
    floatval = 0.0;
    embeddedchunkval = NULL;
    boolval = false;
}



VarValue::~VarValue() {
    validation = 0;

//     if (embeddedchunkval)
//    delete embeddedchunkval;
}


#ifdef VJ_DEBUG
void VarValue::assertValid () const {
    assert (validation == 1 && "Trying to use deleted VarValue");
    if ((type == T_EMBEDDEDCHUNK) && embeddedchunkval)
        embeddedchunkval->assertValid();
}
#endif



VarValue& VarValue::operator= (const VarValue &v) {
    assertValid();
    v.assertValid();

    if (&v == this)
        return *this;

    type = v.type;

    if (embeddedchunkval) {
        // delete embeddedchunkval; XXX
        embeddedchunkval = NULL;
    }
    intval = v.intval;
    floatval = v.floatval;
    boolval = v.boolval;
    strval = v.strval;
    if (v.embeddedchunkval) {

        embeddedchunkval = new ConfigChunk (*v.embeddedchunkval);
    }
    return *this;
}



//: Equality Operator
bool VarValue::operator == (const VarValue& v) const {
    assertValid();
    v.assertValid();

    if (type != v.type)
        return false;
    switch (type) {
    case T_INT:
        return (intval == v.intval);
    case T_FLOAT:
        return (floatval == v.floatval);
    case T_STRING:
    case T_CHUNK:
        return (strval == v.strval);
    case T_BOOL:
        return (boolval == v.boolval);
    case T_EMBEDDEDCHUNK:
        if (embeddedchunkval) {
            if (v.embeddedchunkval)
                return (*embeddedchunkval == *(v.embeddedchunkval));
            else
                return false;
        }
        else
            return (!v.embeddedchunkval);
    default:
        return false;
    }
}




VarValue::operator int() const {
    assertValid();

    switch (type) {
    case T_INT:
        return intval;
    case T_BOOL:
        return boolval;
    case T_FLOAT:
        return (int)floatval;
    case VJ_T_INVALID:
        vprDEBUG(vrjDBG_CONFIG,4) << using_invalid_msg.c_str() << 1
                                << std::endl << vprDEBUG_FLUSH;
        return 0;
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in cast to int.\n"
                               << vprDEBUG_FLUSH;
        return 0;
    }
}



VarValue::operator ConfigChunk*() const {
    assertValid();

    switch (type) {
    case T_EMBEDDEDCHUNK:
        // we need to make a copy because if the value is deleted, it deletes
        // its embeddedchunk
        if (embeddedchunkval)
            return new ConfigChunk (*embeddedchunkval);
        else {
            return NULL;
        }
    case VJ_T_INVALID:
        vprDEBUG(vrjDBG_CONFIG,4) << using_invalid_msg.c_str() << 2
                 << std::endl << vprDEBUG_FLUSH;
        return NULL;
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in cast to ConfigChunk* - real type is " << typeString(type) << ".\n" << vprDEBUG_FLUSH;
        return NULL;
    }
}



VarValue::operator bool() const {
    assertValid();

    if ((type == T_BOOL))
        return boolval;
    switch (type) {
    case T_BOOL:
        return boolval;
    case T_INT:
        return (bool)intval;
    case T_FLOAT:
        return (bool)floatval;
    case VJ_T_INVALID:
        vprDEBUG(vrjDBG_CONFIG,4) << using_invalid_msg.c_str() << 3
                                << std::endl << vprDEBUG_FLUSH;
        return false;
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in cast to bool.\n"
                               << vprDEBUG_FLUSH;
   return false;
    }
}



VarValue::operator float () const {
    assertValid();

    switch (type) {
    case T_FLOAT:
        return floatval;
    case T_INT:
        return (float)intval;
    case T_BOOL:
        return (float)boolval;
    case VJ_T_INVALID:
        vprDEBUG(vrjDBG_CONFIG,4) <<  using_invalid_msg.c_str() << 4
                                << std::endl << vprDEBUG_FLUSH;
        return 0.0f;
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in cast to float.\n" << vprDEBUG_FLUSH;
        return 0.0f;
    }
}



char* VarValue::cstring () const {
    assertValid();
    char buf[256];

    switch (type) {
    case T_STRING:
    case T_CHUNK:
        return strdup (strval.c_str());
    case T_INT:
        sprintf (buf, "%i", intval);
        return strdup (buf);
    case T_FLOAT:
        sprintf (buf, "%f", floatval);
        return strdup (buf);
    case T_BOOL:
        return strdup (boolval?"true":"false");
    case VJ_T_INVALID:
        vprDEBUG(vrjDBG_CONFIG,4) <<  using_invalid_msg.c_str() << 5
                                << std::endl << vprDEBUG_FLUSH;
        return strdup("");
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in cstring().\n" << vprDEBUG_FLUSH;
        return strdup("");
    }
}



VarValue::operator std::string () const {
    assertValid();
    //std::string s;
    //char* c;
    switch (type) {
    case T_STRING:
    case T_CHUNK:
        return strval;
    case T_INT:
        {
            char buf[256];
            sprintf (buf, "%i", intval);
            return std::string (buf);
        }
    case T_FLOAT:
        {
            char buf[256];
            sprintf (buf, "%f", floatval);
            return std::string (buf);
        }
//          c = cstring();
//          s = c;
//          delete[] c;
//          return s;
//      case T_INT:
//          //return std::string(intval);
//          //std::string s = intval;
//          s = intval;
//          return s;
//      case T_FLOAT:
//          //return std::string(floatval);
//          s = floatval;
//          return s;
    case T_BOOL:
        return boolval?"true":"false";
    case VJ_T_INVALID:
        vprDEBUG(vrjDBG_CONFIG,4) <<  using_invalid_msg.c_str() << 6
                                << std::endl << vprDEBUG_FLUSH;
        return (std::string)"";
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in cast to std::string.\n" << vprDEBUG_FLUSH;
        return (std::string)"";
    }
}



VarValue &VarValue::operator = (int i) {
    assertValid();

    switch (type) {
    case T_INT:
        intval = i;
        break;
    case T_FLOAT:
        floatval = (float)i;
        break;
    case T_BOOL:
        boolval = (bool)i;
        break;
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - VarValue(" << typeString(type) << ") = int.\n" << vprDEBUG_FLUSH;
    }
    return *this;
}



VarValue& VarValue::operator = (bool i) {
    assertValid();

    switch (type) {
    case T_INT:
        intval = (int)i;
        break;
    case T_BOOL:
        boolval = i;
        break;
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - VarValue(" << typeString(type) << ") = bool.\n" << vprDEBUG_FLUSH;
    }
    return *this;
}



VarValue &VarValue::operator = (float i) {
    assertValid();

    switch (type) {
    case T_FLOAT:
    case T_DISTANCE:
        floatval = i;
        break;
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - VarValue(" << typeString(type) << ") = float.\n" << vprDEBUG_FLUSH;
    }
    return *this;
}



VarValue &VarValue::operator = (const std::string& s) {
    assertValid();

    return *this = s.c_str();
}



VarValue &VarValue::operator = (const char *val) {
    assertValid();

    bool err = false;
    char* endval;
    int i;
    float f;
    bool b;

    if (val == NULL) {
        val = "";
    }

    switch (type) {
    case T_STRING:
    case T_CHUNK:
        strval = val;
        break;
    case T_INT:
        i = strtol (val, &endval, 0);
        if (*endval == '\0')
            intval = i;
        else
            err = true;
        break;
    case T_FLOAT:
        f = (float)strtod (val, &endval);
        if (*endval == '\0')
            floatval = f;
        else
            err = true;
        break;
    case T_BOOL:
        if (!strcasecmp (val, true_TOKEN))
            boolval = true;
        else if (!strcasecmp (val, false_TOKEN))
            boolval = false;
        else { // we'll try to accept a numeric value
            b = strtol (val, &endval, 0);
            if (*endval == '\0')
                boolval = b;
            else
                err = true;
        }
        break;
    default:
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - VarValue(" << typeString(type) << ") = string/char*.\n" << vprDEBUG_FLUSH;
        break;
    }
    if (err)
        vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: couldn't assign string '"
                               << val << "'.\n" << vprDEBUG_FLUSH;
    return *this;
}


VarValue &VarValue::operator = (const ConfigChunk *s) {
   assertValid();

   switch (type)
       {
       case T_EMBEDDEDCHUNK:
           /* XXX: Leave it hanging for now.
              if (embeddedchunkval)
              delete embeddedchunkval;
           */
           if (s)
               embeddedchunkval = new ConfigChunk (*s);
           else
               embeddedchunkval = NULL;
           break;
       default:
           vprDEBUG(vprDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - VarValue(" << typeString(type) << ") = ConfigChunk*.\n" << vprDEBUG_FLUSH;
       }
   return *this;
}



std::ostream& operator << (std::ostream& out, const VarValue& v) {
    v.assertValid();

    //      vprDEBUG(vprDBG_ERROR,0) << "in << func" << vprDEBUG_FLUSH;

    switch (v.type) {
    case T_INT:
        out << v.intval;
        return out;
    case T_FLOAT:
    case T_DISTANCE:
        out << v.floatval;
        return out;
    case T_BOOL:
        out << ((v.boolval)?"true":"false");
        return out;
    case T_STRING:
    case T_CHUNK:
        out << v.strval.c_str();
        return out;
    case T_EMBEDDEDCHUNK:
        if (v.embeddedchunkval)
            out << *(v.embeddedchunkval);
        return out;
    default:
        out << "[can't print value for type " << (int)v.type << " ]";
        return out;
    }
}

};
