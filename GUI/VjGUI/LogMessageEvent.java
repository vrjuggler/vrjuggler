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

/*
 * Event type for sending log, notification and error type messages
 * to VjControl components that need it (e.g. the ControlUI to display
 * in the status bar or the console panel for logging).
 */

package VjGUI;

import java.util.EventObject;

public class LogMessageEvent extends EventObject {
    String source_name;
    String message;
    int type;

    final static int PERMANENT_MESSAGE = 1;
    final static int TEMPORARY_MESSAGE = 2;
    final static int PERMANENT_ERROR = 3;

    public LogMessageEvent (Object _source, String _source_name, String _message, int _type) {
	super (_source);
	source_name = _source_name;
	message = _message;
	type = _type;
    }

    public int getStyle () {
	return type;
    }

    public String getSourceName () {
	return source_name;
    }

    public String getMessage () {
	return message;
    }

}

