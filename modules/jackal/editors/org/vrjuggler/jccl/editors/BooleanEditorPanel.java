package org.vrjuggler.jccl.editors;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
   
public class BooleanEditorPanel extends PropertyEditorPanel
{
   public void set(Object value, PropertyDefinition prop_def, ConfigElement elm, int prop_num)
   {
      mPropName = prop_def.getToken();
      mConfigElement = elm;
      mPropNum = prop_num;

      fillEditorComponent(value, prop_def);
      
      this.setLayout(new BorderLayout());
      this.add(mEditorComponent, BorderLayout.CENTER);
   }
}
