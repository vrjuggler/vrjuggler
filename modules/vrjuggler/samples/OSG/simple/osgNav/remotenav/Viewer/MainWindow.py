# -*- coding: utf-8 -*-

import sys
from qt import *
from MainWindowBase import MainWindowBase
import math
      
class MainWindow(MainWindowBase):

   def __init__(self,parent = None,name = None,fl = 0):
      MainWindowBase.__init__(self,parent,name,fl)
      self.mFacade = None
      
      # Install event filters
      self.mNavTransPix.installEventFilter(self)
      self.mNavRotPix.installEventFilter(self)

   # public slot
   def helpAbout(self):
      self.dbg("MainWindowBaseImplEx.helpAbout(): Not implemented yet")

   # public slot
   def resetClicked(self):
      self.dbg("Resetting model.")
      self.mFacade.setPosition(0.0,0.0,0.0)
      self.mFacade.setOrientation(0.0,0.0,0.0)
      self.reloadGuiView()
      
   # public slot
   def factorChanged(self,newVal):
      factor_val = float(newVal)/10.0
      self.mFactorValLabel.setText(str(factor_val))


   # ------- NON-slot methods -------- #
   def check(self, cond, desc="no description"):
      """ Smarter assert that displays an error """
      if not cond:
         self.dbg("ASSERTION: " + desc)
      assert cond, desc
      
   def dbg(self, dbgstr):
      """ Print out debug output """
      print dbgstr
      self.txtDebugOutput.append(dbgstr)   # Append to debug output
   
   def setFacade(self, facade):
      self.check(facade, "Null facade")
      self.mFacade = facade;

   def updatePosRotLabel(self):
      pos = self.mFacade.getPosition()
      rot = self.mFacade.getOrientation()
      
      self.mPosLabel.setText("P:%9.1f,%9.1f,%9.1f"% pos)
      self.mRotLabel.setText("R:%9.1f,%9.1f,%9.1f"% rot)
      
   def reloadGuiView(self):
      """ Reload the entire gui view from the model. """
      self.check(self.mFacade, "Tried to reload with null facade.")
      
      # Update label information
      self.updatePosRotLabel()
         
   def eventFilter(self,obj,event):
      if hasattr(self, "mNavTransPix") and obj in (self.mNavTransPix,self.mNavRotPix):
         return self.coordinateReadersEventFilter(obj,event)
      else:
         #print "unhandled event.", event
         return False

   def coordinateReadersEventFilter(self, obj, event):
      """ Event filter used to filter all events from the coordinate readers. """
      if ((event.type() == QEvent.MouseButtonPress) or 
          (event.type() == QEvent.MouseButtonRelease) or 
          (event.type() == QEvent.MouseMove)):
         self.dbg("Mouse event: %s"%event)
         m_pos = (float(event.pos().x()), float(event.pos().y()))
         n_pos = self.getNormalizedWidgetPos(obj,m_pos)
         self.dbg("n pos: %s"%n_pos)

         if event.type() == QEvent.MouseButtonRelease:    # Clear it on release
            self.mFacade.setVelocity(0.0,0.0,0.0)
            self.mFacade.setRotationalVelocity(0.0,0.0,0.0)
         else:
            if obj == self.mNavTransPix:
               self.setTrans(n_pos[0],n_pos[1])
            elif obj == self.mNavRotPix:
               self.setRot(n_pos[0],n_pos[1])
            
         return True             # Consume event
      elif ((event.type() == QEvent.KeyPress) or
            (event.type() == QEvent.KeyRelease)):
         pass
      else:
         return False
      
   def getNormalizedWidgetPos(self,widget,pos):
      " Take pos (x,y) and normalize to widget size "
      w_size = (float(widget.width()), float(widget.height()))
      n_pos = [pos[0]/w_size[0], pos[1]/w_size[1]]
      if n_pos[0] < 0:
         n_pos[0] = 0.0
      elif n_pos[0] > 1.0:
         n_pos[0] = 1.0
      if n_pos[1] < 0:
         n_pos[1] = 0.0
      elif n_pos[1] > 1.0:
         n_pos[1] = 1.0
      return n_pos      
         

   def setTrans(self, mx, my):
      """ Set translation based on normalized values. """
      # Adjust coords to have (0,0) at center of pad
      mx = mx - 0.5
      my = my - 0.5

      dist = math.sqrt((mx*mx)+(my*my))    # Dist from center 
      print "dist: ", dist

      x_trans = 0.0
      y_trans = 0.0
      zin_trans = 0.0
      center_only_dist = 0.1               # Dist range for center only movement
      outside_dist = 0.45                  # Outside range for backwards
      forward_break = 0.34                 # Break dist for forward and back
      
      trans_angle_factor = 1.5             # Factor for x,y trans when at angle
      trans_back_fwd_factor = 3.5
      total_trans_factor = float(self.mFactorSlider.value())/10.0
      
      if (dist <= center_only_dist ):      # Center circle
         print "center"
         zin_trans = 1.0
      elif (dist >= outside_dist):
         print "outside"
         zin_trans = -1.0 ;

      else:     # Translating at an angle either in or out
         y_trans = trans_angle_factor * my                # Slide up
         x_trans = trans_angle_factor * mx                # Slide right

         if (dist <= forward_break):    # Moving forward
            print "Inside break"
            zin_trans = 1 - (trans_angle_factor * dist)      # slide forward
         else:
            print "outside break"
            zin_trans = -1 + (trans_back_fwd_factor * (outside_dist - dist)) ;
            
      # Apply overall scale
      x_trans *= total_trans_factor
      y_trans *= total_trans_factor
      zin_trans *= total_trans_factor
            
      # Check the allow check boxes
      if not self.mAllowXTransChk.isChecked():
         x_trans = 0.0
      if not self.mAllowYTransChk.isChecked():
         y_trans = 0.0
      if not self.mAllowZTransChk.isChecked():
         zin_trans = 0.0
         
      # Now call the update
      self.mFacade.setVelocity(x_trans, y_trans, -zin_trans)
      self.updatePosRotLabel()


   def setRot(self, mx, my):
      # Adjust coords to have (0,0) at center of pad
      mx = mx - 0.5
      my = my - 0.5

      dist = math.sqrt((mx*mx)+(my*my))    # Dist from center 
      print "dist: ", dist

      max_rate = 90.0 * 1.5;      # Max rate to rotate
      heading_only_band = 0.1
      roll_dist = 0.45                # Outside this dist, roll
    
      rot_heading = -mx * max_rate         # Neg for heading
      rot_pitch = my * max_rate
      rot_roll = 0.0
      
      if abs(my) < heading_only_band:
         print "heading only"
         rot_pitch = 0.0;
      else:
         if (dist >= roll_dist):
            if (mx > 0):
               mRotateRoll = -50.0;
            else:
               mRotateRoll = 50.0;
            rot_heading = 0;
            rot_pitch = 0;

      self.mFacade.setRotationalVelocity(rot_pitch, rot_heading, rot_roll)
      self.updatePosRotLabel()

         
if __name__ == "__main__":
   a = QApplication(sys.argv)
   QObject.connect(a,SIGNAL("lastWindowClosed()"),a,SLOT("quit()"))
   w = MainWindow()
   a.setMainWidget(w)
   w.show()
   a.exec_loop()
