/* Event type for sending network status notifications
 */

package VjGUI;

import java.util.EventObject;

public class NetControlEvent extends EventObject {
    public String host;
    public int port;
    public int event_type;

    final static int OPENED = 1;
    final static int CLOSED = 2;
    final static int ADDRESS = 3;

    public NetControlEvent (Object _source, int _event_type, String _host, int _port) {
	super (_source);
	event_type = _event_type;
	host = _host;
	port = _port;
    }

}

