/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#include <Config/vjChunkDesc.h>
#include <Config/vjParseUtil.h>

vjChunkDesc::vjChunkDesc () :plist() {
    name = "unnamed";
    token = "unnamed";
    help = "";
    vjPropertyDesc *d = new vjPropertyDesc("name",1,T_STRING," ");
    add (d);
}



vjChunkDesc::vjChunkDesc (vjChunkDesc& desc): plist() {
    *this = desc;
}



vjChunkDesc::~vjChunkDesc() {
    for (unsigned int i = 0; i < plist.size(); i++)
   delete plist[i];
}



vjChunkDesc& vjChunkDesc::operator= (const vjChunkDesc& other) {
    unsigned int i;

    if (&other == this)
   return *this;

    for (i = 0; i < plist.size(); i++)
    {
       delete plist[i];
       plist[i] = NULL;       // Overwrite dangling pointer
    }
    plist.clear();

    name = other.name;
    token = other.token;
    help = other.help;

    plist.reserve (other.plist.size());
    for (i = 0; i < other.plist.size(); i++)
    {
       //plist.push_back ( other.plist[i]);
       plist.push_back ( new vjPropertyDesc(*(other.plist[i])));
    }

    return *this;
}



void vjChunkDesc::setName (const std::string& _name) {
    name = _name;
}



void vjChunkDesc::setToken (const std::string& _token) {
    token = _token;
}



void vjChunkDesc::setHelp (const std::string& _help) {
    help = _help;
}



std::string vjChunkDesc::getName () {
    return name;
}


std::string vjChunkDesc::getToken () {
    return token;
}


std::string vjChunkDesc::getHelp () {
    return help;
}


void vjChunkDesc::add (vjPropertyDesc *pd) {
    remove(pd->getToken());
    plist.push_back(pd);
}



vjPropertyDesc* vjChunkDesc::getPropertyDesc (const std::string& _token) {
    for (unsigned int i = 0; i < plist.size(); i++)
   if (!vjstrcasecmp (_token, plist[i]->getToken()))
       return plist[i];
    return NULL;
}



bool vjChunkDesc::remove (const std::string& _token)
{
   std::vector<vjPropertyDesc*>::iterator begin = plist.begin();
   while (begin != plist.end())
   {
      if (!vjstrcasecmp ((*begin)->getToken(), _token))
      {
         delete (*begin);
         *begin = NULL;
         plist.erase(begin);
         return true;
      }
      begin++;
   }
   return false;
}



std::ostream& operator << (std::ostream& out, vjChunkDesc& self) {
    out << self.token.c_str() << " \"" << self.name.c_str() << "\" \""
   << self.help.c_str() << '"' << std::endl;
    for (unsigned int i = 0; i < self.plist.size(); i++)
   out << "  " << *(self.plist[i]) << std::endl;
    out << "  end" << std::endl;
    return out;
}



std::istream& operator >> (std::istream& in, vjChunkDesc& self)
{
   const int buflen = 512;
   char str[buflen];
   vjPropertyDesc *p;

   readString (in, str, buflen);
   self.token = str;

   readString (in, str, buflen);
   self.name = str;

   readString (in, str, buflen);
   self.help = str;

   for (unsigned int i = 0; i < self.plist.size(); i++)
   {
      delete self.plist[i];
      self.plist[i] = NULL;                  // Get rid of dangling pointer
   }
   //self.plist.erase (self.plist.begin(), self.plist.end());
   self.plist.clear();

   // this could use improvement
   do
   {
      p = new vjPropertyDesc();
      in >> *p;
      if (!vjstrcasecmp (p->getToken(),std::string("end")))
      {
         delete p;
         break;
      }
      self.add(p);
   } while (!in.eof());

   if (!self.getPropertyDesc ("name"))
      self.plist.insert (self.plist.begin(), new vjPropertyDesc("name",1,T_STRING," "));
   return in;
}



//:equality operator
// a little stricter than it needs to be.. it shouldn't care about the order of
// propertydescs...
bool vjChunkDesc::operator== (const vjChunkDesc& d) {
    if (vjstrcasecmp (token, d.token))
        return false;
    if (vjstrcasecmp (name, d.name))
        return false;
    if (plist.size() != d.plist.size())
        return false;
    for (int i = 0; i < plist.size(); i++)
        if ((*plist[i]) != (*plist[i]))
            return false;
    return true;
}
