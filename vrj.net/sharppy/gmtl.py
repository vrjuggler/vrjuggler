# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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

mod = 'gmtl'

Matrix = ReferenceTemplate(mod, 'gmtl::Matrix', 'gmtl/Matrix.h')
Matrix44f = Matrix('float 4 4', rename = 'gmtl::Matrix44f')
Matrix33f = Matrix('float 3 3', rename = 'gmtl::Matrix33f')

# XXX: The gmtl::VecBase<> instantiations should cannot be renamed until
# Sharppy supports renaming without requiring existing typedefs.
VecBase = ReferenceTemplate(mod, 'gmtl::VecBase', 'gmtl/VecBase.h')
VecBase3f = VecBase('float 3')
VecBase4f = VecBase('float 4')

Vec = ReferenceTemplate(mod, 'gmtl::Vec', 'gmtl/Vec.h')
Vec3f = Vec('float 3', rename = 'gmtl::Vec3f')
Vec4f = Vec('float 4', rename = 'gmtl::Vec4f')

Quat = ReferenceTemplate(mod, 'gmtl::Quat', 'gmtl/Quat.h')
Quatf = Quat('float', rename = 'gmtl::Quatf')

