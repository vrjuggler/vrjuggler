package VjGUI;

import VjGUI.NetControlEvent;

public interface NetControlListener extends java.util.EventListener {

    public abstract void openedConnection (NetControlEvent e);
    public abstract void closedConnection (NetControlEvent e);
    public abstract void addressChanged (NetControlEvent e);

}
