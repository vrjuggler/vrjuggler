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

mod = 'vrj'

User = ReferenceType(mod, 'vrj::User', 'vrj/Kernel/User.h')

App = ReferenceType(mod, 'vrj::App', 'vrj/Kernel/App.h')
#exclude(App.getDrawManager)
#exclude(App.getSoundManager)

GlApp = ReferenceType(mod, 'vrj::GlApp', 'vrj/Draw/OGL/GlApp.h')
#exclude(App.getDrawManager)

Kernel = ReferenceType(mod, 'vrj::Kernel', 'vrj/Kernel/Kernel.h')
exclude(Kernel.getUsers) # Returns a vector
exclude(Kernel.getThread)
exclude(Kernel.controlLoop)

#DisplayManager = ReferenceType(mod, 'vrj::DisplayManager',
#                               'vrj/Display/DisplayManager.h')
#exclude(DisplayManager.getActiveDisplays) # Returns a vector
#exclude(DisplayManager.getInActiveDisplays) # Returns a vector
#exclude(DisplayManager.getAllDisplays) # Returns a vector
#
#Display = ReferenceType(mod, 'vrj::Display', 'vrj/Display/Display.h')

DrawManager = ReferenceType(mod, 'vrj::DrawManager', 'vrj/Draw/DrawManager.h')
exclude(DrawManager.addDisplay)
exclude(DrawManager.removeDisplay)
exclude(DrawManager.setDisplayManager)
exclude(DrawManager.getDisplayManager)

SoundManager = ReferenceType(mod, 'vrj::SoundManager',
                             'vrj/Sound/SoundManager.h')
