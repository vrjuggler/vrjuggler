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
#include <Config/vjConfigChunk.h>
#include <Config/vjParseUtil.h>
#include <Kernel/vjDebug.h>
#include <Config/vjChunkFactory.h>
#include <Config/vjChunkDesc.h>
#include <Config/vjConfigTokens.h>

#include <stdlib.h>
#include <ctype.h>



vjConfigChunk::vjConfigChunk (): props(), type_as_varvalue(T_STRING) {
    desc = 0;
    validation = 1;
}



vjConfigChunk::vjConfigChunk (vjChunkDesc *d) :props(), type_as_varvalue(T_STRING) {
    validation = 1;
    associateDesc (d);
}



vjConfigChunk::~vjConfigChunk () {
    assertValid();

    validation = 0;
    /* XXX
    for (unsigned int i = 0; i < props.size(); i++)
        delete (props[i]);
        */
}



vjConfigChunk::vjConfigChunk (const vjConfigChunk& c):props(), type_as_varvalue(T_STRING) {
    validation = 1;
    *this = c;
}



#ifdef VJ_DEBUG
void vjConfigChunk::assertValid () const {
    assert (validation == 1 && "Trying to use deleted config chunk");
    for (unsigned int i = 0; i < props.size(); i++)
        props[i]->assertValid();
}
#endif



void vjConfigChunk::associateDesc (vjChunkDesc* d) {
    assertValid();

    unsigned int i;

    desc = d;
    type_as_varvalue = desc->getToken();

    /* XXX:
    for (i = 0; i < props.size(); i++)
        delete (props[i]);
    */
    props.clear();

    for (i = 0; i < desc->plist.size(); i++) {
        vjPropertyDesc* pd = desc->plist[i];
        vjProperty* pr = new vjProperty (pd);
        props.push_back (pr);
    }
}



vjConfigChunk& vjConfigChunk::operator = (const vjConfigChunk& c) {
    assertValid();
    c.assertValid();

    unsigned int i;
    if (this == &c)     // ack! same object!
        return *this;

    desc = c.desc;
    type_as_varvalue = c.type_as_varvalue;

    /* XXX:
    for (i = 0; i < props.size(); i++)
        delete (props[i]);
        */
    props.clear();

    for (i = 0; i < c.props.size(); i++) {
        props.push_back (new vjProperty(*(c.props[i])));
    }
    return *this;
}



//: tests for equality of two vjConfigChunks
bool vjConfigChunk::operator== (const vjConfigChunk& c) const {
    assertValid();
    c.assertValid();

    // equality operator - this makes a couple of assumptions:
    // 1. the descs must be the _same_object_, not just equal.
    // 2. the properties will be in the _same_order_.  This is
    //    reasonable if 1. is true.

    if (desc != c.desc)
        return false;
    if (props.size() != c.props.size()) // probably redundant
        return false;
    for (unsigned int i = 0; i < props.size(); i++) {
        if (*(props[i]) != *(c.props[i]))
            return false;
    }
    return true;
}



//: Compares two vjConfigChunks based on their instance names
bool vjConfigChunk::operator< (const vjConfigChunk& c) const {
    assertValid();

    std::string s1 = getProperty ("name");
    std::string s2 = c.getProperty ("name");
    return s1 < s2;
}



//: Return a list of chunk names dependant upon this one
// This is used to sort a db by dependancy.
std::vector<std::string> vjConfigChunk::getChunkPtrDependencies() const
{
    assertValid();

   std::string chunkname;
   std::vector<std::string> dep_list;     // Create return vector
   unsigned int i;
   int j;

   //cout << "Dependency test for " << getProperty ("name") << endl;
   for (i=0;i<props.size();i++)                       // For each property
   {
      if (props[i]->getType() == T_CHUNK)             // If it is a chunk ptr
      {
         for (j=0;j<props[i]->getNum();j++)           // For each property
         {
            vjVarValue prop_var_val = props[i]->getValue(j);
            chunkname = static_cast<std::string>(prop_var_val);
            if (!(chunkname == ""))
            {
               dep_list.push_back(chunkname);
            }
         }
      }
      else if (props[i]->getType() == T_EMBEDDEDCHUNK)
      {
         std::vector<std::string> child_deps;
         for (j = 0; j < props[i]->getNum(); j++)
         {
            // XXX: if we ever have cyclic dependencies, we're in trouble
            child_deps = ((vjConfigChunk*)props[i]->getValue(j))->getChunkPtrDependencies();
            dep_list.insert (dep_list.end(), child_deps.begin(), child_deps.end());
         }
      }
   }
   return dep_list;      // Return the list
}



vjProperty* vjConfigChunk::getPropertyPtrFromName (const std::string& property) const {
    assertValid();

    for (unsigned int i = 0; i < props.size(); i++) {
        if (!vjstrcasecmp (props[i]->getName(), property))
            return props[i];
    }
    return NULL;
}



vjProperty* vjConfigChunk::getPropertyPtrFromToken (const std::string& token) const {
    assertValid();

    for (unsigned int i = 0; i < props.size(); i++) {
        if (!vjstrcasecmp(props[i]->getToken(), token))
            return props[i];
    }
    return NULL;
}



//: Return all the values for a given property
// This is just a simple helper function
//! NOTE: The vector has COPIES of the var values.
// cj - this is bad implementation... bad...
std::vector<vjVarValue*> vjConfigChunk::getAllProperties(const std::string& property)
{
    assertValid();

    int num_properties = getNum(property);
    std::vector<vjVarValue*> ret_val;
    for(int i=0;i<num_properties;i++) {
        vjVarValue* new_var_val = new vjVarValue(getProperty(property,i));
        ret_val.push_back(new_var_val);
    }

    return ret_val;
}



std::ostream& operator << (std::ostream& out, vjConfigChunk& self) {
    self.assertValid();

    // outputting an uninitialized chunk would be a mistake...
    if (self.desc) {
        out << self.desc->token.c_str() << std::endl;
        for (unsigned int i =0; i < self.props.size(); i++) {
            out << "  " << *(self.props[i]) << std::endl;
        }
        out << "  End" << std::endl;
    }
    return out;
}



bool vjConfigChunk::tryassign (vjProperty *p, int index, const char* val) {
    assertValid();

    /* This does some type-checking and translating before just
     * doing an assign into the right value entry of p. Some of
     * this functionality ought to just be subsumed by vjVarValue
     * itself, but this way we get back some feedback about
     * wether a type mismatch occurred (ie we return false if
     * a type mismatch occurs ).
     *
     * Incidentally, this is also where string values get
     * mangled into enumeration entries when assigning strings
     * to T_INTs.
     */
    char* endval;
    int i;
    float f;
    bool b;

    if(p->getType() != T_CHUNK) {          // T_CHUNKS have enumeration, but they are really strings (or something)
        vjEnumEntry* e = p->getEnumEntry (val);
        if (e) {
            p->setValue (e->getValue());
            return true;
        }
    }

    switch (p->getType()) {
    case T_INT:
        i = strtol (val, &endval, 0);
        if (*endval != '\0')
            vjDEBUG (vjDBG_CONFIG, 0) << clrOutNORM(clrYELLOW, "WARNING:") << " Parser expected int, got '"
                  << val << "'\n" << vjDEBUG_FLUSH;
        p->setValue (i, index);
        return true;
    case T_FLOAT:
        f = (float)strtod (val, &endval);
        if (*endval != '\0')
            vjDEBUG (vjDBG_CONFIG, 0) << clrOutNORM(clrYELLOW, "WARNING:") << " Parser expected float, got '"
                                      << val << "'\n" << vjDEBUG_FLUSH;
        p->setValue (f, index);
        return true;
    case T_BOOL:
        b = false;
        if (!strcasecmp (val, true_TOKEN))
            b = true;
        else if (!strcasecmp (val, false_TOKEN))
            b = false;
        else { // we'll try to accept a numeric value
            b = strtol (val, &endval, 0);
            if (endval != '\0') {
                b = false;
                vjDEBUG (vjDBG_CONFIG,0) << clrOutNORM(clrYELLOW, "WARNING:") << " Parser expected bool, got '"
                                         << val << "'\n" << vjDEBUG_FLUSH;
            }
        }
        p->setValue (b, index);
        return true;
    case T_STRING:
    case T_CHUNK:
        p->setValue (val, index);
        return true;
    case T_EMBEDDEDCHUNK:
        std::cout << "NOT HANDLED HERE!" << std::endl;
        return false;
    default:
        return false;
    }
}




std::istream& operator >> (std::istream& in, vjConfigChunk& self) {
    self.assertValid();

    /* can't really use property >> because we don't know what
     * property to assign into.
     */
    const int buflen = 1024;
    char buf[buflen];
    vjProperty *p;
    int i;
    bool quoted;

    // if this chunk hasn't been assigned a description yet, something's wrong
    if (!self.desc)
        return in;

    while (readString (in, buf, buflen, NULL)) {

        if (!strcasecmp (buf, end_TOKEN))
            break;

        // We have a string token; assumably a property name.
        if (!(p = self.getPropertyPtrFromToken (buf))) {
            vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << " Property '" << buf << "' is not found in"
                                   << " Chunk " << self.desc->name.c_str() << std::endl << vjDEBUG_FLUSH;
            continue;
        }

        // We're reading a line of input for a valid Property.
        readString (in, buf, buflen, &quoted);

        if (!quoted && (buf[0] == '{')) {
            // We're reading values until we get a close bracket.
            i = 0;
            for (;;) {
                readString (in, buf, buflen, &quoted);
                if (!quoted && (buf[0] == '}'))
                    break;

                // this works because the chunk >> expects the typename to have
                // already been read (which we did when looking for '}')
                if (p->getType() == T_EMBEDDEDCHUNK) {
                    vjConfigChunk *ch = vjChunkFactory::instance()->createChunk (p->embeddesc);
                    in >> *ch;
                    p->setValue (ch, i++);
                }
                //       else if (tok.type == TK_Unit) {
                //           p->applyUnits (tok.unitval);
                //       }
                else {
                    if (!self.tryassign (p, i++, buf))
                        vjDEBUG(vjDBG_ERROR,2) << clrOutNORM(clrRED, "ERROR:") << " Assigning to property "
                                               << p->getName().c_str() << std::endl << vjDEBUG_FLUSH;
                }
            }

            if (p->hasFixedNumberOfValues() && (p->num != i))
                vjDEBUG(vjDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") << " vjProperty " << p->getName().c_str() << " should have "
                                       << p->num << " values; " << i << " found" << std::endl << vjDEBUG_FLUSH;
        }
        else {
            // we're just doing one value.
            if (!self.tryassign (p, 0, buf))
                vjDEBUG(vjDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") << " Assigning to property "
                                       << p->getName().c_str() << std::endl << vjDEBUG_FLUSH;
            //        self.getVJCFGToken (in,tok);
            //        if (tok.type == TK_Unit) {
            //       p->applyUnits (tok.unitval);
            //       self.getVJCFGToken (in, tok);
            //        }
            if (p->num > 1) {
                vjDEBUG(vjDBG_ERROR,3) << clrOutNORM(clrRED, "ERROR:") << " Property " << p->getName().c_str()
                                       << " expects " << p->num << " values." << std::endl << vjDEBUG_FLUSH;
            }
        }
    }

    return in;
}



int vjConfigChunk::getNum (const std::string& property_token) const {
    assertValid();

    vjProperty* p = getPropertyPtrFromToken (property_token);
    if (p)
        return p->getNum();
    else
        return 0;
}



const vjVarValue& vjConfigChunk::getType () const {
    assertValid();

    return type_as_varvalue;
}



const vjVarValue& vjConfigChunk::getProperty (const std::string& property_token, int ind) const {
    assertValid();

    if (!vjstrcasecmp(property_token,type_TOKEN)) {
        return type_as_varvalue;
    }

    vjProperty *p = getPropertyPtrFromToken (property_token);
    if (!p) {
        vjDEBUG(vjDBG_CONFIG,2) << "getProperty(\"" << property_token.c_str() << "\") in chunk \""
                                << getProperty("Name") << "\" - no such property; returning T_INVALID\n" << vjDEBUG_FLUSH;
        return vjVarValue::getInvalidInstance();
    }
    return p->getValue (ind);
}


/* we're probably gonna need to overload set for each kind of
 * value.  That gets passed on to the vjVarValue assign...
 */

bool vjConfigChunk::setProperty (const std::string& property, int val, int ind) {
    assertValid();

    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (!p)
        return false;
    return p->setValue (val, ind);
}

bool vjConfigChunk::setProperty (const std::string& property, float val, int ind) {
    assertValid();

    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (!p)
        return false;
    return p->setValue (val, ind);
}

bool vjConfigChunk::setProperty (const std::string& property, const std::string& val, int ind) {
    assertValid();

    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (!p)
        return false;
    return p->setValue (val, ind);
}

bool vjConfigChunk::setProperty (const std::string& property, vjConfigChunk* val, int ind) {
    assertValid();

    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (!p) {
        vjDEBUG (vjDBG_ERROR, 1) << "ConfigChunk.setProperty: no such property " << property.c_str()
                                 << "\n" << vjDEBUG_FLUSH;
        return false;
    }
    return p->setValue (val, ind);
}


bool vjConfigChunk::addValue (const std::string& property, int val) {
    assertValid();

    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (p == NULL)
        return false;
    if (p->hasFixedNumberOfValues())
        return false;
    return setProperty (property, val, p->value.size());
}

bool vjConfigChunk::addValue (const std::string& property, float val) {
    assertValid();

    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (p == NULL)
        return false;
    if (p->hasFixedNumberOfValues())
        return false;
    return setProperty (property, val, p->value.size());
}

bool vjConfigChunk::addValue (const std::string& property, const std::string& val) {
    assertValid();

    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (p == NULL)
        return false;
    if (p->hasFixedNumberOfValues())
        return false;
    return setProperty (property, val, p->value.size());
}

bool vjConfigChunk::addValue (const std::string& property, vjConfigChunk* val) {
    assertValid();

    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (p == NULL)
        return false;
    if (p->hasFixedNumberOfValues())
        return false;
    return setProperty (property, val, p->value.size());
}


