/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


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

