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

