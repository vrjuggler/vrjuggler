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
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


package VjGUI.util;

public interface ChildFrame {

    //: the frame should unregister for any events and dispose() itself.
    public void destroy ();


    //: a way to match the frame up with the db and object it talks about.
    // If o is null, just matches the db.  So that, for example, we can
    // test for all child frames that depend on some particular chunkdb.
    // If class is non-null, will only return true if this' class matches
    // the class name given.
    public boolean matches (String cl, Object db, Object o);

    //: called when various ui things happen, such as looknfeel changing.
    public void updateUI ();
}




