package VjConfig;

import VjConfig.ChunkDBEvent;

public interface ChunkDBListener extends java.util.EventListener {

    public abstract void addChunk (ChunkDBEvent e);
    public abstract void removeChunk (ChunkDBEvent e);
    public abstract void replaceChunk (ChunkDBEvent e);
    public abstract void removeAllChunks (ChunkDBEvent e);
}


