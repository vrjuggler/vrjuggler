package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import javax.swing.JLabel;

import org.vrjuggler.vrjconfig.commoneditors.event.UnitInfoEvent;
import org.vrjuggler.vrjconfig.commoneditors.event.UnitInfoListener;


/**
 * A small extension to <code>javax.swing.JLabel</code> that automatically
 * changes its text whenever the device unit associated with the label changes.
 */
public class UnitLabel
   extends JLabel
   implements UnitInfoListener
{
   public UnitLabel(UnitInfo unitInfo)
   {
      this.unitInfo = unitInfo;
      this.unitInfo.addUnitInfoListener(this);
      setUnitText();
   }

   /**
    * Tells this object to clean up after itself when it is no longer needed.
    */
   public void release()
   {
      this.unitInfo.removeUnitInfoListener(this);
      this.unitInfo = null;
   }

   /**
    * Responds to the unit number change by updating the text property of
    * this label.
    */
   public void unitNumberChanged(UnitInfoEvent evt)
   {
      setUnitText();
   }

   private void setUnitText()
   {
      setText(UnitTypeHelpers.getUnitTypeName(unitInfo.getUnitType()) +
              " Unit " + unitInfo.getUnitNumber());
   }

   private UnitInfo unitInfo = null;
}
