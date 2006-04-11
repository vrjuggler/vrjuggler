package org.vrjuggler.vrjconfig.wizard;

import java.util.Iterator;
import java.util.ArrayList;
import java.io.IOException;
import javax.swing.*;
import java.awt.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.wizard.*;
import java.awt.event.*;
import javax.swing.border.*;

public class WizardComboBoxModel extends DefaultComboBoxModel implements ConfigListener
{
  String mFileSource = null;
  ConfigBroker mBroker = null;
  ConfigContext mContext = null;
  java.util.List mChunkTypes = new ArrayList();

  public WizardComboBoxModel()
  {
    mBroker = new ConfigBrokerProxy();
    mContext = new ConfigContext();
    mBroker.addConfigListener(this);
    mBroker.addConfigListener(this);
  }
  public void setFileSource(String filesrc)
  {
    mFileSource = filesrc;
    mContext.add(mFileSource);
    update();
  }
  public void addChunkType(String chunk_type)
  {
    mChunkTypes.add(chunk_type);
    update();
  }
  public void removeChunkType(String chunk_type)
  {
    mChunkTypes.remove(chunk_type);
    update();
  }
  public void removeAllChunkTypea()
  {
    mChunkTypes.clear();
    update();
  }
  public void chunkDescAdded(ConfigEvent evt)
      {;}
  public void chunkDescRemoved(ConfigEvent evt)
      {;}
  public void configChunkAdded(ConfigEvent evt)
  { update(); }
  public void configChunkRemoved(ConfigEvent evt)
  { update(); }
  public void update()
  {
    this.removeAllElements();
    java.util.List matches = new ArrayList();
    java.util.List chunks_list = mBroker.getChunks(mContext);

    for(Iterator i = mChunkTypes.iterator() ; i.hasNext() ;)
    {
      String temp_type = (String)i.next();
      matches.addAll(ConfigUtilities.getChunksWithDescToken(chunks_list, temp_type));
    }

    for(int i = 0; i < matches.size() ; i++)
    {
      this.addElement(((ConfigChunk)matches.get(i)).getName());
    }
  }
}