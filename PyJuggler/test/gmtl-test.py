#!/usr/bin/env python

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
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

from PyJuggler import gmtl

mat1 = gmtl.Matrix44f()
mat2 = gmtl.Matrix44f()

gmtl.zero(mat1)
gmtl.identity(mat1)

mat3 = gmtl.Matrix44f()
gmtl.mult(mat3, mat1, mat2)

mat4 = mat1 * mat2

if gmtl.isEqual(mat3, mat4):
   print "mat1 * mat2 pass"
else:
   print "mat1 * mat2 fail"

gmtl.sub(mat3, mat2, mat1)
print mat3
gmtl.add(mat3, mat2, mat1)
print mat3

gmtl.preMult(mat3, mat1)
gmtl.postMult(mat3, mat1)

gmtl.mult(mat3, mat1, 555.5)

mat3 = gmtl.transpose(mat1)
gmtl.transpose(mat3, mat1)

gmtl.invertFull(mat3, mat1)

mat3 = gmtl.invert(mat1)
gmtl.invert(mat3, mat1)

gmtl.identity(mat1)
gmtl.zero(mat2)

if not gmtl.isEqual(mat1, mat2):
   print "gmtl.isEqual(mat1, mat2) pass"
else:
   print "gmtl.isEqual(mat1, mat2) fail"

if gmtl.isEqual(mat1, mat1):
   print "gmtl.isEqual(mat1, mat1) pass"
else:
   print "gmtl.isEqual(mat1, mat1) fail"

if mat1 == mat1:
   print "mat1 == mat1 pass"
else:
   print "mat1 == mat1 fail"

print "mat1[1][1] =", mat1[1][1]
mat1[1][1] = 5

if mat1[1][1] == 5:
   print "gmtl.Matrix44f element accessor pass"
else:
   print "gmtl.Matrix44f element accessor fail"

vec1 = gmtl.Vec3f()
vec2 = gmtl.Vec3f()
vec3 = gmtl.Vec3f()

vec1.set(1, 2, 3)
vec2.set(4, 5, 6)
vec3.set(7, 8, 9)

print "vec1: %s" % vec1
print "vec2: %s" % vec2
print "vec3: %s" % vec3

if vec1 != vec2:
   print "vec1 != vec2 pass"
else:
   print "vec1 != vec2 fail"

if vec1[0] == 1.0:
   print "gmtl.Vec3f element accessor pass"
else:
   print "gmtl.Vec3f element accessor fail"

if vec1[1] != vec1[2]:
   print "gmtl.Vec3f element comparison pass"
else:
   print "gmtl.Vec3f element comparison fail"

print "vec1[0] =", vec1[0]

vec1[0] = 5.0
if vec1[0] == 5.0:
   print "gmtl.Vec3f element accessor pass"
else:
   print "gmtl.Vec3f element accessor fail"

dot_prod = gmtl.dot(vec1, vec2)
print "Dot product =", dot_prod

print "Length of vec1 =", gmtl.length(vec1)
print "Squared length of vec1 =", gmtl.lengthSquared(vec1)

vec3_orig_length = gmtl.length(vec3)
vec3_old_length = gmtl.normalize(vec3)

if vec3_orig_length == vec3_old_length:
   print "gmtl.normalize(vec3) return pass"
else:
   print "gmtl.normalize(vec3) return fail"

if not gmtl.isNormalized(vec1):
   print "gmtl.isNormalized(vec1) pass"
else:
   print "gmtl.isNormalized(vec1) fail"

if gmtl.isNormalized(vec3):
   print "gmtl.isNormalized(vec3) pass"
else:
   print "gmtl.isNormalized(vec3) fail"

if gmtl.length(vec3) == 1.0:
   print "gmtl.length(vec3) pass"
else:
   print "gmtl.length(vec3) fail"
   print "Normalized length of vec3 = ", gmtl.length(vec3)

vec4 = gmtl.makeCross(vec1, vec2)

# Create a translation matrix and then create a vector from it.
trans_mat = gmtl.makeTransMatrix44(vec2)
trans_vec = gmtl.makeTransVec3(trans_mat)

if gmtl.isEqual(vec2, trans_vec, 0.0):
   print "gmtl.makeTransMatrix44()/gmtl.makeTransVec3() pass"
else:
   print "gmtl.makeTransMatrix44()/gmtl.makeTransVec3() fail"

point1 = gmtl.Point2i(4, 5)
point1[0] = point1[1]

if point1[0] == point1[1]:
   print "gmtl.Point2i element assignment pass"
else:
   print "gmtl.Point2i element assignment fail"

euler1 = gmtl.EulerAngleXYZf(90.0, 0.0, 0.0)
euler2 = gmtl.EulerAngleXYZf(0.0, 90.0, 0.0)

if not gmtl.isEqual(euler1, euler2):
   print "gmtl.isEqual(euler1, euler2) pass"
else:
   print "gmtl.isEqual(euler1, euler2) fail"

if euler1[0] == 90.0:
   print "gmtl.EulerAngleXYZf element accessor pass"
else:
   print "gmtl.EulerAngleXYZf element accessor fail"

rot_mat   = gmtl.makeRotMatrix44(euler1)
mat_angle = gmtl.makeRotEulerAngleXYZ(rot_mat)

# XXX: This is questionable...
if gmtl.isEqual(euler1, mat_angle):
   print "gmtl.makeRot() pass"
else:
   print "gmtl.makeRot() fail"

gmtl.identity(mat1)
mat3 = gmtl.makeInverse(mat1)

if gmtl.isEqual(mat1, mat3):
   print "gmtl.makeInverse() pass (lame test!)"
else:
   print "gmtl.makeInverse() fail"

sphere = gmtl.Spheref(gmtl.Point3f(5.0, 5.0, 5.0), 2.0)

if sphere.getRadius() == 2.0:
   print "gmtl.Sphere.getRadius() psas"
else:
   print "gmtl.Sphere.getRadius() fail"

aabox = gmtl.AABoxf(gmtl.Point3f(4.0, 4.0, 4.0),
                    gmtl.Point3f(6.0, 6.0, 6.0))

if gmtl.intersect(sphere, aabox):
   print "gmtl.intersect(Spheref, AABoxf) pass"
else:
   print "gmtl.intersect(Spheref, AABoxf) fail"
