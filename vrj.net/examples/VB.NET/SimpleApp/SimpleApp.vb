'************** <auto-copyright.pl BEGIN do not edit this line> *************
'
' VRJ.NET is (C) Copyright 2004 by Patrick Hartling
'
' This library is free software; you can redistribute it and/or
' modify it under the terms of the GNU Library General Public
' License as published by the Free Software Foundation; either
' version 2 of the License, or (at your option) any later version.
'
' This library is distributed in the hope that it will be useful,
' but WITHOUT ANY WARRANTY; without even the implied warranty of
' MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
' Library General Public License for more details.
'
' You should have received a copy of the GNU Library General Public
' License along with this library; if not, write to the
' Free Software Foundation, Inc., 59 Temple Place - Suite 330,
' Boston, MA 02111-1307, USA.
'
' -----------------------------------------------------------------
' File:          $RCSfile$
' Date modified: $Date$
' Version:       $Revision$
' -----------------------------------------------------------------
'
'************** <auto-copyright.pl END do not edit this line> ***************

Imports System
Imports Tao.OpenGl


Public Class SimpleApp
   Inherits vrj.GlApp

   Public Overrides Sub init()
      Console.WriteLine("SimpleApp.init()")
      mButton0.init("VJButton0")
      mButton1.init("VJButton1")
      mWand.init("VJWand")
   End Sub

   Public Overrides Sub contextInit()
      Console.WriteLine("SimpleApp.contextInit()")
      initGLState()
   End Sub

   Public Overrides Sub preFrame()
      If (mButton0.getProxy().getData() <> gadget.Digital.State.OFF) Then
         mBoxGrabbed = True
      Else
         mBoxGrabbed = False
      End If

      If (mButton1.getProxy().getData() <> gadget.Digital.State.OFF) Then
         Console.WriteLine("Button 1 pressed")
      End If
   End Sub

   Public Overrides Sub bufferPreDraw()
      Gl.glClearColor(0.0F, 0.0F, 0.0F, 0.0F)
      Gl.glClear(Gl.GL_COLOR_BUFFER_BIT)
   End Sub

   Public Overrides Sub draw()
      Gl.glClear(Gl.GL_DEPTH_BUFFER_BIT)

      ' --- Setup for drawing ---
      Gl.glMatrixMode(Gl.GL_MODELVIEW)

      Dim box_offset As gmtl.Matrix44f

      ' If the box is grabbed, position it in the same place as the wand.
      If (mBoxGrabbed) Then
         box_offset = mWand.getProxy().getData()
      Else
         ' Otherwise, use a fixed position.
         ' Create box offset matrix.
         Dim euler_ang As gmtl.EulerAngleXYZf
         euler_ang = New gmtl.EulerAngleXYZf(gmtl.Math.deg2Rad(-90.0F), _
                                             gmtl.Math.deg2Rad(0.0F), _
                                             gmtl.Math.deg2Rad(0.0F))

         box_offset = gmtl.Gmtl.makeRotMatrix44(euler_ang)
         gmtl.Gmtl.setTrans(box_offset, New gmtl.Vec3f(0.0F, 1.0F, 0.0F))
      End If

      ' Create a color for the box.
      Dim box_color() As Single = {0.7F, 0.7F, 0.7F}

      Gl.glPushMatrix()

      ' Draw the box.
      Gl.glPushMatrix()
      ' Push the box offset matrix on the stack.
      Gl.glMultMatrixf(box_offset.getData())
      Gl.glColor3fv(box_color)
      Gl.glScalef(0.5F, 0.5F, 0.5F)
      drawCube()
      Gl.glPopMatrix()

      ' Draw a coordinate axis "on" the box.
      Gl.glLineWidth(2.0F)
      Gl.glDisable(Gl.GL_LIGHTING)
      Gl.glPushMatrix()
      Gl.glMultMatrixf(box_offset.getData())
      Dim x_axis As gmtl.Vec3f = New gmtl.Vec3f(7.0F, 0.0F, 0.0F)
      Dim y_axis As gmtl.Vec3f = New gmtl.Vec3f(0.0F, 7.0F, 0.0F)
      Dim z_axis As gmtl.Vec3f = New gmtl.Vec3f(0.0F, 0.0F, 7.0F)
      Dim origin As gmtl.Vec3f = New gmtl.Vec3f(0.0F, 0.0F, 0.0F)

      Gl.glBegin(Gl.GL_LINES)
      Gl.glColor3f(1.0F, 0.0F, 0.0F)
      Gl.glVertex3fv(origin.getData())
      Gl.glVertex3fv(x_axis.getData())

      Gl.glColor3f(0.0F, 1.0F, 0.0F)
      Gl.glVertex3fv(origin.getData())
      Gl.glVertex3fv(y_axis.getData())

      Gl.glColor3f(0.0F, 0.0F, 1.0F)
      Gl.glVertex3fv(origin.getData())
      Gl.glVertex3fv(z_axis.getData())
      Gl.glEnd()
      Gl.glPopMatrix()
      Gl.glEnable(Gl.GL_LIGHTING)
      Gl.glPopMatrix()
   End Sub

   Private Sub initGLState()
      Dim light0_ambient() As Single = {0.1F, 0.1F, 0.1F, 1.0F}
      Dim light0_diffuse() As Single = {0.8F, 0.8F, 0.8F, 1.0F}
      Dim light0_specular() As Single = {1.0F, 1.0F, 1.0F, 1.0F}
      Dim light0_position() As Single = {0.0F, 0.75F, 0.75F, 0.0F}

      Dim mat_ambient() As Single = {0.7F, 0.7F, 0.7F, 1.0F}
      Dim mat_diffuse() As Single = {1.0F, 0.5F, 0.8F, 1.0F}
      Dim mat_specular() As Single = {1.0F, 1.0F, 1.0F, 1.0F}
      Dim mat_shininess() As Single = {50.0F}
      Dim mat_emission() As Single = {1.0F, 1.0F, 1.0F, 1.0F}
      Dim no_mat() As Single = {0.0F, 0.0F, 0.0F, 1.0F}

      Gl.glLightfv(Gl.GL_LIGHT0, Gl.GL_AMBIENT, light0_ambient)
      Gl.glLightfv(Gl.GL_LIGHT0, Gl.GL_DIFFUSE, light0_diffuse)
      Gl.glLightfv(Gl.GL_LIGHT0, Gl.GL_SPECULAR, light0_specular)
      Gl.glLightfv(Gl.GL_LIGHT0, Gl.GL_POSITION, light0_position)

      Gl.glMaterialfv(Gl.GL_FRONT, Gl.GL_AMBIENT, mat_ambient)
      Gl.glMaterialfv(Gl.GL_FRONT, Gl.GL_DIFFUSE, mat_diffuse)
      Gl.glMaterialfv(Gl.GL_FRONT, Gl.GL_SPECULAR, mat_specular)
      Gl.glMaterialfv(Gl.GL_FRONT, Gl.GL_SHININESS, mat_shininess)
      Gl.glMaterialfv(Gl.GL_FRONT, Gl.GL_EMISSION, no_mat)

      Gl.glEnable(Gl.GL_DEPTH_TEST)
      Gl.glEnable(Gl.GL_NORMALIZE)
      Gl.glEnable(Gl.GL_LIGHTING)
      Gl.glEnable(Gl.GL_LIGHT0)
      Gl.glEnable(Gl.GL_COLOR_MATERIAL)
      Gl.glShadeModel(Gl.GL_SMOOTH)
   End Sub

   Private Sub drawCube()
      drawBox(-0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, Gl.GL_QUADS)
   End Sub

   Private Sub drawBox(ByVal x0 As Double, ByVal x1 As Double, _
                       ByVal y0 As Double, ByVal y1 As Double, _
                       ByVal z0 As Double, ByVal z1 As Double, _
                       ByVal type As Integer)
      Dim n(,) As Double = {{-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, _
                            {1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, _
                            {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}}

      Dim faces(,) As Integer = {{0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4}, _
                                 {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3}}

      Dim v(,) As Double = New Double(8, 3) {}
      Dim tmp As Double

      If (x0 > x1) Then
         tmp = x0
         x0 = x1
         x1 = tmp
      End If
      If (y0 > y1) Then
         tmp = y0
         y0 = y1
         y1 = tmp
      End If
      If (z0 > z1) Then
         tmp = z0
         z0 = z1
         z1 = tmp
      End If

      v(0, 0) = x0
      v(1, 0) = x0
      v(2, 0) = x0
      v(3, 0) = x0

      v(4, 0) = x1
      v(5, 0) = x1
      v(6, 0) = x1
      v(7, 0) = x1

      v(0, 1) = y0
      v(1, 1) = y0
      v(4, 1) = y0
      v(5, 1) = y0

      v(2, 1) = y1
      v(3, 1) = y1
      v(6, 1) = y1
      v(7, 1) = y1

      v(0, 2) = z0
      v(3, 2) = z0
      v(4, 2) = z0
      v(7, 2) = z0

      v(1, 2) = z1
      v(2, 2) = z1
      v(5, 2) = z1
      v(6, 2) = z1

      Dim i As Integer
      For i = 0 To 5 Step +1
         Gl.glBegin(type)
         Gl.glNormal3d(n(i, 0), n(i, 1), n(i, 2))
         Gl.glVertex3d(v(faces(i, 0), 0), v(faces(i, 0), 1), v(faces(i, 0), 2))
         Gl.glNormal3d(n(i, 0), n(i, 1), n(i, 2))
         Gl.glVertex3d(v(faces(i, 1), 0), v(faces(i, 1), 1), v(faces(i, 1), 2))
         Gl.glNormal3d(n(i, 0), n(i, 1), n(i, 2))
         Gl.glVertex3d(v(faces(i, 2), 0), v(faces(i, 2), 1), v(faces(i, 2), 2))
         Gl.glNormal3d(n(i, 0), n(i, 1), n(i, 2))
         Gl.glVertex3d(v(faces(i, 3), 0), v(faces(i, 3), 1), v(faces(i, 3), 2))
         Gl.glEnd()
      Next
   End Sub

   Private mButton0 As gadget.DigitalInterface = New gadget.DigitalInterface
   Private mButton1 As gadget.DigitalInterface = New gadget.DigitalInterface
   Private mWand As gadget.PositionInterface = New gadget.PositionInterface

   Private mBoxGrabbed As Boolean = False

End Class

Module SimpleAppModule

   Public Sub Main(ByVal args() As String)
      Dim kernel As vrj.Kernel
      kernel = vrj.Kernel.instance()

      Dim i As Integer
      For i = 0 To UBound(args)
         kernel.loadConfigFile(args(i))
      Next

      kernel.start()
      kernel.setApplication(New SimpleApp)
      kernel.waitForKernelStop()
   End Sub

End Module
