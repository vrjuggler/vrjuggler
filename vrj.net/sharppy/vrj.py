# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

mod = 'vrj'

User = ReferenceType(mod, 'vrj::User', 'vrj/Kernel/User.h')
sealed(User)

App = ReferenceType(mod, 'vrj::App', 'vrj/Kernel/App.h')
#exclude(App.getDrawManager)
#exclude(App.getSoundManager)

GlApp = ReferenceType(mod, 'vrj::GlApp', 'vrj/Draw/OGL/GlApp.h')
#exclude(App.getDrawManager)

Kernel = ReferenceType(mod, 'vrj::Kernel', 'vrj/Kernel/Kernel.h')
sealed(Kernel)
exclude(Kernel.getUsers) # Returns a vector
exclude(Kernel.getThread)
exclude(Kernel.controlLoop)

#DisplayManager = ReferenceType(mod, 'vrj::DisplayManager',
#                               'vrj/Display/DisplayManager.h')
#exclude(DisplayManager.getActiveDisplays) # Returns a vector
#exclude(DisplayManager.getInActiveDisplays) # Returns a vector
#exclude(DisplayManager.getAllDisplays) # Returns a vector

Display = ReferenceType(mod, 'vrj::Display', 'vrj/Display/Display.h')
sealed(Display)
exclude(Display.getViewport)
exclude(Display.mViewports)

DrawManager = ReferenceType(mod, 'vrj::DrawManager', 'vrj/Draw/DrawManager.h')
exclude(DrawManager.setDisplayManager)
exclude(DrawManager.getDisplayManager)
exclude(DrawManager.outStream)

SoundManager = ReferenceType(mod, 'vrj::SoundManager',
                             'vrj/Sound/SoundManager.h')
