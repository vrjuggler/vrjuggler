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


package VjConfig;

import VjConfig.ChunkDBEvent;

public interface ChunkDBListener extends java.util.EventListener {

    public abstract void addChunk (ChunkDBEvent e);
    public abstract void removeChunk (ChunkDBEvent e);
    public abstract void replaceChunk (ChunkDBEvent e);
    public abstract void removeAllChunks (ChunkDBEvent e);
}


