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
#include <Utils/vjDebug.h>
#include <Config/vjChunkFactory.h>
#include <Config/vjChunkDesc.h>
#include <Config/vjConfigTokens.h>

#include <stdlib.h>
#include <ctype.h>


/*static*/ const std::string vjConfigChunk::embedded_separator("->");

vjConfigChunk::vjConfigChunk (): props(), type_as_varvalue(T_STRING) {
    desc = 0;
    validation = 1;
}



vjConfigChunk::vjConfigChunk (vjChunkDesc *d, bool use_defaults) :props(), type_as_varvalue(T_STRING) {
    validation = 1;
    associateDesc (d, use_defaults);
}


vjConfigChunk::vjConfigChunk (const vjConfigChunk& c):props(), type_as_varvalue(T_STRING) {
    validation = 1;
    *this = c;
}


vjConfigChunk::~vjConfigChunk () {
    assertValid();

    validation = 0;
    /* XXX
    for (unsigned int i = 0; i < props.size(); i++)
        delete (props[i]);
        */
}

bool vjConfigChunk::hasSeparator (const std::string &path) {
    return (path.find(embedded_separator) != path.npos);
}

std::string vjConfigChunk::getRemainder (const std::string &path) {
    std::string::size_type i = path.find (embedded_separator);
    if (i == path.npos)
        return path;
    else 
        return path.substr (i + embedded_separator.length());
}

std::string vjConfigChunk::getFirstNameComponent (const std::string& path) {
    std::string::size_type i = path.find (embedded_separator);
    if (i == path.npos)
        return path;
    else 
        return path.substr (0, i);
}

#ifdef VJ_DEBUG
void vjConfigChunk::assertValid () const {
    assert (validation == 1 && "Trying to use deleted config chunk");
    for (unsigned int i = 0; i < props.size(); i++)
        props[i]->assertValid();
}
#endif



void vjConfigChunk::associateDesc (vjChunkDesc* d, bool use_defaults) {
    assertValid();
    vjConfigChunk* ch = 0;
    if (use_defaults)
        ch = d->getDefaultChunk();
    if (ch)
        *this = *ch;
    else {

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



// used for dependency resolution
vjConfigChunk* vjConfigChunk::getEmbeddedChunk (const std::string &path) {
    std::string propname, chunkname, subpath;
    vjProperty* prop;
    int i;
    vjConfigChunk *ch = this;
    vjConfigChunk *ch2, *ch3;
        
    if (vjstrcasecmp (ch->getName(), path /*getFirstNameComponent (path)*/)) {
        return 0;
    }
    else {
        if (!hasSeparator(path))
            return ch;
        else {
            subpath = getRemainder(path); // strip chunkname
            propname = getFirstNameComponent(path);
            subpath = getRemainder(path); // strip propname
            chunkname = getFirstNameComponent (path);
            prop = getPropertyPtrFromName(propname);
            if (prop) {
                for (i = 0; i < prop->getNum(); i++) {
                    ch2 = (vjConfigChunk*)prop->getValue(i);
                    if (ch2) {
                        ch3 = ch2->getEmbeddedChunk(path);
                        if (ch3)
                            return ch3;
                    }
                }
            }
            else {
                // this next bit is insurance against some of my 
                // own most likely mistakes
                prop = getPropertyPtrFromToken(propname);
                if (prop) {
                    for (i = 0; i < prop->getNum(); i++) {
                        ch2 = (vjConfigChunk*)prop->getValue(i);
                        if (ch2) {
                            ch3 = ch2->getEmbeddedChunk(path);
                            if (ch3)
                                return ch3;
                        }
                    }
                }
            }
            return 0;
        }
    }
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
std::vector<vjVarValue*> vjConfigChunk::getAllProperties(const std::string& prop_name) const
{
    assertValid();

    std::vector<vjVarValue*> ret_val;
    vjProperty* p = getPropertyPtrFromToken(prop_name);
    if (p) {
        int num_properties = p->getNum();
        for(int i=0;i<num_properties;i++) {
            vjVarValue* new_var_val = new vjVarValue(p->getValue(i));
            ret_val.push_back(new_var_val);
        }
    }

    return ret_val;
}



std::ostream& operator << (std::ostream& out, const vjConfigChunk& self) {
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
                    if (!p->tryAssign (i++, buf))
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
            if (!p->tryAssign (0, buf))
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


