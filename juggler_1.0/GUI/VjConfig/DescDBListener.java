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

package VjConfig;

import VjConfig.DescDBEvent;

public interface DescDBListener extends java.util.EventListener {

    public abstract void addDesc (DescDBEvent e);
    public abstract void removeDesc (DescDBEvent e);
    public abstract void replaceDesc (DescDBEvent e);
    public abstract void removeAllDescs (DescDBEvent e);
}


