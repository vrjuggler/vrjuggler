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

#ifndef MULTICASTER_H
#define MULTICASTER_H

#include <map>
#include <vector>
#include <string>


class Value 
{
};


class MulticastObserver
{
public:
    virtual void accept (std::string address, Value value) = 0;

};

typedef std::vector<MulticastObserver *>     Subscriptions;
typedef std::map<std::string,Subscriptions>  AddressSpace;



class Multicaster
{
public:
    virtual        ~Multicaster () {}
    virtual bool    subscribe (MulticastObserver *observer, std::string address);
    virtual bool    unsubscribe (MulticastObserver *observer, std::string address);
    virtual bool    publish (MulticastObserver *observer, std::string address, Value value);
    virtual void    unsubscribeFromAll (MulticastObserver *observer);

private:
    AddressSpace    m_addresses;
    void            unsubscribe (Subscriptions& subscriptions, MulticastObserver *observerToRemove);

};


#endif
