package VjGUI;

import VjGUI.CoreDBEvent;

public interface LogMessageListener extends java.util.EventListener {

    public abstract void logMessage (LogMessageEvent e);

}
