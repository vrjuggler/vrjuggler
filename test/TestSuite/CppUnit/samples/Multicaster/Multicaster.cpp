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

#include "Multicaster.h"

bool Multicaster::subscribe (MulticastObserver *observer, std::string address)
{
    AddressSpace::iterator  it = m_addresses.find (address);

    if (it == m_addresses.end ())
    {
        m_addresses.insert (std::pair<std::string, Subscriptions> (address, Subscriptions ()));       
        it = m_addresses.find (address);

        if (it == m_addresses.end ())
            return false;
    }

    (*it).second.push_back (observer);
    return true;

}


bool Multicaster::unsubscribe (MulticastObserver *observer, std::string address)
{
    AddressSpace::iterator  it = m_addresses.find (address);

    if (it == m_addresses.end ())
        return false;

    unsubscribe ((*it).second, observer);

    return true;

}

bool Multicaster::publish (MulticastObserver *publisher, std::string address, Value value)
{
    AddressSpace::iterator  it = m_addresses.find (address);

    if (it == m_addresses.end ())
        return false;

    Subscriptions& subscriptions = (*it).second;

    for (Subscriptions::iterator subit = subscriptions.begin ();
            subit != subscriptions.end ();
            ++subit) {
 
        MulticastObserver *subscriber = *subit;

        if (subscriber != publisher)
            subscriber->accept (address, value);
        
    } 

    return true;
}


void Multicaster::unsubscribeFromAll (MulticastObserver *observer)
{
    for (AddressSpace::iterator it = m_addresses.begin ();
            it != m_addresses.end ();
            ++it)
        unsubscribe ((*it).second, observer);
}


void Multicaster::unsubscribe (Subscriptions& subscriptions, MulticastObserver *observerToRemove)
{
    for (Subscriptions::iterator it = subscriptions.begin (); it != subscriptions.end (); ) {
        if (*it == observerToRemove)
            it = subscriptions.erase (it);
        else
            it++;
    }

}
