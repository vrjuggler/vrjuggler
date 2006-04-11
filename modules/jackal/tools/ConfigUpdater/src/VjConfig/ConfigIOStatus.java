/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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



package VjConfig;

import java.util.List;
import java.util.ArrayList;

/** Status object for ConfigIO & its parser objects.
 *  This is used to store various warning & error messages while 
 *  parsing ConfigChunk or ChunkDesc files.  They can then be 
 *  conveniently output to the user after parsing completes.
 *  This allows parsing to continue uninterrupted when minor warnings
 *  occur, and allows a GUI to display the messages in a user-friendly
 *  manner.
 *
 *  @version $Revision$
 */
public class ConfigIOStatus {

    List messages;  // either Strings or Throwables.
    int status;

    int num_warnings;
    int num_errors;
    
    public static final int SUCCESS = 1;
    public static final int WARNINGS = 2;
    public static final int ERRORS = 3;   // loaded, but there were errors
    public static final int FAILURE = 4;  // total failure; nothing loaded

    public ConfigIOStatus () {
        messages = new ArrayList();
        clear();
    }

    /** Clears messages and resets status to "success". */
    public void clear () {
        messages.clear();
        status = SUCCESS;
        num_warnings = 0;
        num_errors = 0;
    }

    /** Convenience method to set the status.  When adding messages,
     *  the overall severity can only get worse - ie we progress
     *  from SUCCESS -> WARNINGS -> ERRORS -> FAILURE.
     */
    private void setStatus (int new_status) {
        // wish i had an assert

        // things can only go downhill from here
        if (new_status > status)
            status = new_status;
    }

    public int getStatus () {
        return status;
    }
    
    public void addWarning (String msg) {
        setStatus (WARNINGS);
        messages.add (msg);
        num_warnings++;
    }

    public void addError (String msg) {
        setStatus (ERRORS);
        messages.add (msg);
        num_errors++;
    }

    public void addFailure (String msg) {
        setStatus (FAILURE);
        messages.add (msg);
    }

    public void addWarning (Throwable msg) {
        setStatus (WARNINGS);
        messages.add (msg);
        num_warnings++;
    }

    public void addError (Throwable msg) {
        setStatus (ERRORS);
        messages.add (msg);
        num_errors++;
    }

    public void addFailure (Throwable msg) {
        setStatus (FAILURE);
        messages.add (msg);
    }

    /** Returns number of messages. */
    public int size() {
        return messages.size();
    }

    /** Returns ith message object.  Messages are either Strings or
     *  Throwables.
     */
    public Object get (int i) {
        return messages.get(i);
    }

    /** Provides a one-line summary of # of warnings, errors, etc. */
    public String getSummary () {
        switch (status) {
        case SUCCESS:
            return "Loaded succesfully.";
        case WARNINGS:
            StringBuffer s = new StringBuffer();
            s.append ("Loaded with ");
            s.append (num_warnings);
            s.append ((num_warnings == 1)?" warning.":" warnings.");
            return new String(s);
        case ERRORS:
            StringBuffer s2 = new StringBuffer();
            s2.append ("Loaded with ");
            s2.append (num_errors);
            s2.append ((num_errors == 1)?" error, ":" errors, ");
            s2.append (num_warnings);
            s2.append ((num_warnings == 1)?" warning.":" warnings.");
            return new String(s2);
        case FAILURE:
        default:
            return "Load failed.";
        }
    }

    /** Creates a string containing the summary and all messages. */
    public String toString () {
        StringBuffer s = new StringBuffer();
        s.append (getSummary());
        s.append ("\n");

        for (int i = 0; i < messages.size(); i++) {
            Object o = messages.get(i);
            s.append (o.toString());
        }

        return new String(s);
    }

}

