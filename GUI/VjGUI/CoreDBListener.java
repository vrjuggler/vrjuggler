package VjGUI;

import VjGUI.CoreDBEvent;

public interface CoreDBListener extends java.util.EventListener {

    public abstract void addChunkDB (CoreDBEvent e);
    public abstract void removeChunkDB (CoreDBEvent e);
    public abstract void addDescDB (CoreDBEvent e);
    public abstract void removeDescDB (CoreDBEvent e);

}


