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


package VjComponents.Network;

import java.io.*;


/** An InputStream subclass that terminates and returns EOF when a particular
 *  string is read from the stream it attaches to.  It doesn't read any
 *  farther than that from the parent stream.  The terminator itself is
 *  not returned via any of the read() methods.
 *  <p>
 *  The JAXP input handler for vjcontrol uses this to know when to stop 
 *  reading from an input stream coming over a network socket.
 *
 *  @author Christopher Just
 *  @version $Revision$
 */
public class ProtocolInputStream extends InputStream {

    /** InputStream to read from. */
    private InputStream parent;

    /** Terminator string. */
    private String terminator;

    private int termlength;

    /** Number of characters of the terminator string that have been read.
     *  ProtocolInputStream works like a state machine, with termlength+1
     *  states.  In each state, if we find the next character of the
     *  terminator, we go to the next state, otherwise we go to state 0.
     */
    private int search_state;

    /** Set true when we've read the complete terminator string.
     *  When finished == true, read() returns -1.
     */
    private boolean finished;

    /** When we've read x characters of the terminator, and then the next
     *  character doesn't match, it's a false positive.
     *  In this case, we start returning the first x characters of the
     *  terminator string, followed by the character that showed the
     *  false positive.
     */
    private boolean false_positive;

    /** Indicates our position when recovering from a 'false_positive'.
     */
    private int recycle_partial_term; 

    /** Temp storage for the character that caused a 'false_positive'.
     *  Once the partial terminator, plus the character stored in
     *  false_signifier, have been returned by read() calls, we set
     *  false_positive to 0 and start reading from the stream normally.
     */
    private int false_signifier;


    /** Constructor. */
    public ProtocolInputStream (InputStream _parent, String _terminator) {
        parent = _parent;
        terminator = _terminator;
        termlength = terminator.length();
        search_state = 0;
        finished = false;
        false_positive = false;
    }


    /** Reads a single character from the stream.
     *  The other forms of InputStream.read are not overloaded; the default
     *  versions fall back to calling this read() multiple times.
     */
    public int read () throws IOException {
        int ch;
        if (false_positive) {
            if (recycle_partial_term == search_state) {
                recycle_partial_term = 0;
                search_state = 0;
                false_positive = false;
                ch = false_signifier;
            }
            else {
                ch = (int)terminator.charAt(recycle_partial_term);
                recycle_partial_term++;
            }
            return ch;
        }
        // read chars...
        if (finished)
            return -1;
        for (;;) {
            ch = parent.read();
            if (ch == -1) {
                finished = true;
                //return ch;
            }
            if (ch == (int)terminator.charAt(search_state)) {
                search_state++;
                if (search_state == termlength) {
                    //return -1;
                    finished = true;
                    ch = -1;
                    break;
                }
            }
            else if (search_state > 0) {
                recycle_partial_term = 0;
                false_positive = true;
                false_signifier = ch;
                return read();
            }
            else
                break;
        }
        return ch;
    }


    /** Reads characters from the stream until we read the terminator or EOF.
     *  This is a convenience used to make sure we've read to the end of
     *  a protocol block, so that the caller doesn't get confused trying to
     *  parse thru anything that the protocol handler didn't bother to read.
     *  This is particularly helpful if the handler happened to bomb out with
     *  an error, or if the handler has it's own termination identifier and
     *  so doesn't bother reading the protocol terminator string.
     */
    public void windToEnd () throws IOException {
        while (read() != -1)
            ;
    }

}
