/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef juggler_RumbleApp_h
#define juggler_RumbleApp_h

#include <vrj/Draw/OpenGL/App.h>

#include <gadget/Type/RumbleInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/AnalogInterface.h>

namespace juggler {
    class RumbleApp : public vrj::opengl::App
    {
      public:
	RumbleApp(vrj::Kernel* kern)
      : vrj::opengl::App(kern)
	{
	}
	virtual void init();
	virtual void preFrame();
	virtual void bufferPreDraw();
	virtual void draw();
      private:
	void initRumble();

	gadget::RumbleInterface _rumble;
	gadget::DigitalInterface _button[16];
	gadget::AnalogInterface _axes[10];

	gadget::RumbleEffectPtr _speed;
	gadget::RumbleEffectPtr _objectHit[4];
    };
}
#endif // juggler_RumbleApp_h
