package VjGUI.util;

import VjGUI.util.ListElem;
import java.util.NoSuchElementException;

public class ListIterator {

    public ListElem first;
    public ListElem last;
    public ListElem current;

    public ListIterator (ListElem _first, ListElem _last, ListElem _current) {
	first = _first;
	last = _last;
	current = _current;
    }

    public Object next() throws NoSuchElementException {
	if (current == last || current.next == last)
	    throw new NoSuchElementException();
	else {
	    current = current.next;
	    return current.data;
	}
    }


    public Object prev() throws NoSuchElementException {
	if (current == first || current.prev == first)
	    throw new NoSuchElementException();
	else {
	    current = current.prev;
	    return current.data;
	}
    }


    public boolean hasNext() {
	return (current != last) && (current.next != last);
    }

    public boolean hasPrev() {
	return (current != first) && (current.prev != first);
    }

}


