from omniORB import CORBA
import vrj
import RemoteNavFacade

class TweekFacade(RemoteNavFacade.RemoteNavFacade):
   def __init__(self, navSubjMgr):
#      RemoteNavFacade.RemoteNavFacade.__init__(self)
      self.navSubjMgr = navSubjMgr

      try:
         subject = self.navSubjMgr.getSubject("RemoteNavSubject")
         self.navSubj = subject._narrow(vrj.RemoteNavSubject)
      except CORBA.BAD_PARAM:
         print "WARNING: Could not narrow subject to RemoteNavSubject"
         self.navSubj = None

   def setPosition(self, xPos, yPos, zPos):
      self.navSubj.setPosition(xPos, yPos, zPos)

   def getPosition(self):
      return self.navSubj.getPosition()

   def setOrientation(self, heading, pitch, roll):
      self.navSubj.setOrientation(heading, pitch, roll)

   def getOrientation(self):
      return self.navSubj.getOrientation()

   def setVelocity(self, xDelta, yDelta, zDelta):
      self.navSubj.setVelocity(xDelta, yDelta, zDelta)

   def setRotationalVelocity(self, headingDelta, pitchDelta, rollDelta):
      self.navSubj.setRotationalVelocity(headingDelta, pitchDelta, rollDelta)

   def setWalkMode(self, walk):
      self.navSubj.setWalkMode(walk)
