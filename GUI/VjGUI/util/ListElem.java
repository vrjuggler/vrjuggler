package VjGUI.util;

/* I hate to do this, but it's a little too soon to give up jdk11 compatibility
 */

public class ListElem {

    public Object data;
    public ListElem prev;
    public ListElem next;

    public ListElem() {
	data = null;
	prev = null;
	next = null;
    }
}

