#
#
#

import sys

class RemoteNavFacade:
   """ Base interface for remote nav facade """
   
   def setPosition(self,x,y,z):
      assert(false)

   def setOrientation(self,h,p,r):
      pass
      
   def getPosition(self):
      assert(false)
      return (1,2,3)
      
   def getOrientation(self):
      assert(false)
      return (1,2,3)
      
   def setVelocity(self,x,y,z):
      assert(false)
      
   def setRotationalVelocity(self,h,p,r):
      assert(false)
      
   def setWalkMode(self,walkMode):
      assert(false)
      

class TestRemoteNavFacade(RemoteNavFacade):
   def __init__(self):
      self.pos = (0.0,0.0,0.0)
      self.rot = (0.0,0.0,0.0)
      
   def setPosition(self,x,y,z):
      self.pos = (float(x),float(y),float(z))
      print "Set pos:", (x,y,z), " -- ", self.pos
      
   def setOrientation(self,h,p,r):
      self.rot = (float(h),float(p),float(r))
      print "Set rot:", (h,p,r), " -- ", self.pos

      
   def getPosition(self):
      return self.pos
            
   def getOrientation(self):
      return self.rot
      
   def setVelocity(self,x,y,z):
      p = self.pos
      self.pos = (p[0]+x, p[1]+y, p[2]+z)
      print "Set vel:", (x,y,z), " -- ", self.pos
      
   def setRotationalVelocity(self,h,p,r):
      rot = self.rot
      self.rot = (rot[0]+h, rot[1]+p, rot[2]+r)
      print "Set rotation:", (h,p,r), " -- ", self.rot

   def setWalkMode(self,walkMode):
      print "Set walk mode:", walkMode
      
