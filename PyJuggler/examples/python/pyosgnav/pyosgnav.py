#!/usr/bin/env python

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

import sys
import gmtl
from OpenGL.GL import *
from PyOSG import *

from PyJuggler import *


class PyOsgNav(vrj.OsgApp):
   def __init__(self, filename):
      vrj.OsgApp.__init__(self)

      self.mFileToLoad = filename

      self.mButton0 = gadget.DigitalInterface()
      self.mButton1 = gadget.DigitalInterface()
      self.mButton2 = gadget.DigitalInterface()
      self.mWand    = gadget.PositionInterface()

   def initScene(self):
      self.mButton0.init("VJButton0")
      self.mButton1.init("VJButton1")
      self.mButton2.init("VJButton2")
      self.mWand.init("VJWand")

      self.__myInit()

   def getScene(self):
      return self.mRootNode

   def configSceneView(self, newSceneViewer):
      newSceneVeiwer.setDefaults()
      newSceneViewer.setBackgroundColor(osg.Vec4(0.0, 0.0, 0.0, 0.0))
      newSceneViewer.getLight().setAmbient(osgVec4(0.3, 0.3, 0.3, 1.0))
      newSceneViewer.getLight().setDiffuse(osgVec4(0.9, 0.9, 0.9, 1.0))
      newSceneViewer.getLight().setSpecular(osgVec4(1.0, 1.0, 1.0, 1.0))

   def preFrame(self):
      # Speed up.
      if self.mButton0.getData() == gadget.Digital.State.ON:
         self.speed = self.speed + self.inc

      # Stop.
      if self.mButton1.getData() == gadget.Digital.State.ON:
         self.speed = 0.0

      # Slow down.
      if self.mButton2.getData() == gadget.Digital.State.ON:
         self.speed = self.speed - self.inc

      wand_matrix = self.mWand.getData()

      osg_wand_mat = osg.Matrix()
      osg_wand_mat.set(wand_matrix.getData())

      direction = gmtl.Vec3f()
      z_dir = gmtl.Vec3f(0.0, 0.0, self.speed)
      gmtl.xform(direction, wand_matrix, z_dir)
      self.mNavTrans.preMult(osg.Matrix.translate(direction[0], direction[1],
                                                  direction[2]))

   def bufferPreDraw(self):
      glClearColor(0.0, 0.0, 0.0)
      glClear(GL_COLOR_BUFFER_BIT)

   def draw(self):
      glClear(GL_DEPTH_BUFFER_BIT)
      vrj.OsgApp.draw(self)

   def __myInit(self):
      self.mRootNode = osg.Group()
      self.mNoNav    = osg.Group()
      self.mNavTrans = osg.MatrixTransform()

      self.mRootNode.addChild(self.mNoNav)
      self.mRootNode.addChild(self.mNavTrans)

      print "Attempting to load file:", self.mFileToLoad, "...",
      self.mModel = osgDB.readNodeFile(self.mFileToLoad)
      print "done."

      self.mModelTrans = osg.MatrixTransform()
      self.mModelTrans.preMult(osg.Matrix.rotate(gmtl.Math.deg2Rad(-90.0),
                                                 1.0, 0.0, 0.0))

      self.mModelTrans.addChild(self.mModel)
      self.mNavTrans.addChild(self.mModelTrans)

      optimizer = osgUtil.Optimizer()
      optimizer.optimize(self.mRootNode)

      self.speed = 0.0
      self.inc   = 0.005

if __name__ == "__main__":
   app    = PyOsgNav(sys.argv[1])
   kernel = vrj.Kernel.instance()

   for arg in sys.argv[2:]:
      kernel.loadConfigFile(arg)

   kernel.start()
   kernel.setApplication(app)
   kernel.waitForKernelStop()
