#!/usr/bin/env python

# PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

import sys
from OpenGL.GL import *
import gmtl

# NOTE: If you import the individual modules from PyJuggler, make sure to
# import PyJuggler.vrj first.
from PyJuggler import *


class ContextApp(vrj.GlApp):
   def __init__(self):
      vrj.GlApp.__init__(self)

      self.mCubeDlHolder = vrj.GlContextData()
      self.mButton0      = gadget.DigitalInterface()
      self.mButton1      = gadget.DigitalInterface()
      self.mWand         = gadget.PositionInterface()
      self.mHead         = gadget.PositionInterface()

   def init(self):
      self.mButton0.init("VJButton0")
      self.mButton1.init("VJButton1")
      self.mWand.init("VJWand")
      self.mHead.init("VJHead")

   def contextInit(self):
      self.mCubeDlHolder.id = glGenLists(1)

      glNewList(self.mCubeDlHolder.id, GL_COMPILE)
      glScalef(0.5, 0.5, 0.5)
      self.drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS)
      glEndList()

      self.initGLState()

   def preFrame(self):
      if self.mButton0.getData():
         self.mGrabbed = True
      else:
         self.mGrabbed = False

   def bufferPreDraw(self):
      glClearColor(0.0, 0.0, 0.0, 0.0)
      glClear(GL_COLOR_BUFFER_BIT)

   def draw(self):
      glClear(GL_DEPTH_BUFFER_BIT)
      glMatrixMode(GL_MODELVIEW)
      glPushMatrix()
      glMultMatrixf(self.mWand.getData().getData())
      glColor3f(0.7, 0.7, 0.7)
      glCallList(self.mCubeDlHolder.id)
      glEnable(GL_LIGHTING)
      glPopMatrix()

   def drawbox(self, x0, x1, y0, y1, z0, z1, type):
      n = [ [-1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [1.0, 0.0, 0.0],
            [0.0, -1.0, 0.0], [0.0, 0.0, 1.0], [0.0, 0.0, -1.0] ]
      faces = [ [0, 1, 2, 3], [3, 2, 6, 7], [7, 6, 5, 4],
                [4, 5, 1, 0], [5, 6, 2, 1], [7, 4, 0, 3] ]

      if x0 > x1:
         tmp = x0
         x0 = x1
         x1 = tmp
      if y0 > y1:
         tmp = y0
         y0 = y1
         y1 = tmp
      if z0 > z1:
         tmp = z0
         z0 = z1
         z1 = tmp

      v = [ [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0],
            [0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0] ]
      v[0][0] = v[1][0] = v[2][0] = v[3][0] = x0
      v[4][0] = v[5][0] = v[6][0] = v[7][0] = x1
      v[0][1] = v[1][1] = v[4][1] = v[5][1] = y0
      v[2][1] = v[3][1] = v[6][1] = v[7][1] = y1
      v[0][2] = v[3][2] = v[4][2] = v[7][2] = z0
      v[1][2] = v[2][2] = v[5][2] = v[6][2] = z1

      for i in range(6):
         glBegin(type)
         glNormal3d(n[i][0], n[i][1], n[i][2])
         glVertex3d(v[faces[i][0]][0], v[faces[i][0]][1], v[faces[i][0]][2])
         glNormal3d(n[i][0], n[i][1], n[i][2])
         glVertex3d(v[faces[i][1]][0], v[faces[i][1]][1], v[faces[i][1]][2])
         glNormal3d(n[i][0], n[i][1], n[i][2])
         glVertex3d(v[faces[i][2]][0], v[faces[i][2]][1], v[faces[i][2]][2])
         glNormal3d(n[i][0], n[i][1], n[i][2])
         glVertex3d(v[faces[i][3]][0], v[faces[i][3]][1], v[faces[i][3]][2])
         glEnd()

   def initGLState(self):
      glLight(GL_LIGHT0, GL_AMBIENT, [0.1, 0.1, 0.1, 1.0])
      glLight(GL_LIGHT0, GL_DIFFUSE, [0.8, 0.8, 0.8, 1.0])
      glLight(GL_LIGHT0, GL_SPECULAR, [1.0, 1.0, 1.0, 1.0])
      glLight(GL_LIGHT0, GL_POSITION, [0.0, 0.75, 0.75, 0.0])

      glMaterial(GL_FRONT, GL_AMBIENT, [0.7, 0.7, 0.7, 1.0])
      glMaterial(GL_FRONT, GL_DIFFUSE, [1.0, 0.5, 0.8, 1.0])
      glMaterial(GL_FRONT, GL_SPECULAR, [1.0, 1.0, 1.0, 1.0])
      glMaterial(GL_FRONT, GL_SHININESS, [50.0])
      glMaterial(GL_FRONT, GL_EMISSION, [0.0, 0.0, 0.0, 1.0])

      glEnable(GL_DEPTH_TEST)
      glEnable(GL_NORMALIZE)
      glEnable(GL_LIGHTING)
      glEnable(GL_LIGHT0)
#      glEnable(GL_COLOR_MATERIAL)
      glShadeModel(GL_SMOOTH)

app    = ContextApp()
kernel = vrj.Kernel.instance()

for arg in sys.argv[1:]:
   kernel.loadConfigFile(arg)

kernel.start()
kernel.setApplication(app)
kernel.waitForKernelStop()
