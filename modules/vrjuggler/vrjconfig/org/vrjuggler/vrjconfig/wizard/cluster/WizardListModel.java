package org.vrjuggler.vrjconfig.wizard.cluster;

import java.util.*;
import javax.swing.*;
import org.vrjuggler.jccl.config.*;

public class WizardListModel
   extends DefaultListModel
   implements ConfigListener
{
  String mFileSource = null;
  ConfigBroker mBroker = null;
  ConfigContext mContext = null;
  List mElementTypes = new ArrayList();

  public WizardListModel()
  {
    mBroker = new ConfigBrokerProxy();
    mContext = new ConfigContext();
    mBroker.addConfigListener(this);
  }
  public void setFileSource(String filesrc)
  {
    mFileSource = filesrc;
    mContext.add(mFileSource);
    update();
  }
  public void addElementType(String element_type)
  {
    mElementTypes.add(element_type);
    update();
  }
  public void removeElementType(String element_type)
  {
    mElementTypes.remove(element_type);
    update();
  }
  public void removeAllElementTypes()
  {
    mElementTypes.clear();
    update();
  }
  public void configElementAdded(ConfigEvent evt)
  { update(); }
  public void configElementRemoved(ConfigEvent evt)
  { update(); }
  public void update()
  {
    this.removeAllElements();
    List matches = new ArrayList();
    List elts = mBroker.getElements(mContext);

    for(Iterator i = mElementTypes.iterator() ; i.hasNext() ;)
    {
      String temp_type = (String)i.next();
      matches.addAll(ConfigUtilities.getElementsWithDefinition(elts, temp_type));
    }

    for(int i = 0; i < matches.size() ; i++)
    {
      this.addElement(((ConfigElement)matches.get(i)).getName());
    }
  }
}
