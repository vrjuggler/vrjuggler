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


package VjGUI.util;

import VjGUI.util.ListElem;
import VjGUI.util.ListIterator;
import java.util.NoSuchElementException;

public class LinkedList {

    ListElem first;
    ListElem last;
    int num;

    
    public LinkedList() {
	first = new ListElem();
	last = new ListElem();
	first.next = last;
	last.prev = first;
	num = 0;
    }

    
    public void add (Object o) {
	// adds to end;
	ListElem tmp = new ListElem();
	tmp.data = o;
	tmp.next = last;
	tmp.prev = last.prev;
	last.prev.next = tmp;
	last.prev = tmp;
	num++;
    }

    public void addFirst (Object o) {
	// adds to start
	ListElem tmp = new ListElem();
	tmp.data = o;
	tmp.next = first.next;
	tmp.prev = first;
	first.next.prev = tmp;
	first.next = tmp;
	num++;
    }

    public Object getFirst () throws NoSuchElementException {
	if (first.next == last)
	    throw new NoSuchElementException();
	else
	    return first.next.data;
    }

    public Object getLast () throws NoSuchElementException {
	if (last.prev == first)
	    throw new NoSuchElementException();
	else
	    return last.prev.data;
    }

    public Object removeFirst() throws NoSuchElementException {
	ListElem tmp;

	if (first.next == last)
	    throw new NoSuchElementException();
	else {
	    tmp = first.next;
	    tmp.next.prev = first;
	    first.next = tmp.next;
	    num--;
	    return tmp.data;
	}
    }

    public Object removeLast() throws NoSuchElementException {
	ListElem tmp;

	if (last.prev == first)
	    throw new NoSuchElementException();
	else {
	    tmp = last.prev;
	    tmp.prev.next = last;
	    last.prev = tmp.prev;
	    num--;
	    return tmp.data;
	}
    }


    public int size() {
	return num;
    }

    public ListIterator listIterator (int index) {
	// bug; assumes index is 0
	return new ListIterator (first, last, first);
    }
}



