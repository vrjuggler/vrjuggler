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

#include "RumbleApp.h"

#include <iostream>
#include <GL/gl.h>

#include <gadget/InputManager.h>
#include <gadget/Type/Rumble.h>
#include <gadget/Type/RumbleEffect.h>
#include <vrj/Kernel/Kernel.h>

using namespace juggler;
using gadget::RumbleEffect;
using std::cout;
using std::endl;

void RumbleApp::init()
{
    gadget::InputManager *input;
    input = vrj::Kernel::instance()->getInputManager();

    _button[0] .init("Joystick0_d0");
    _button[1] .init("Joystick0_d1");
    _button[2] .init("Joystick0_d2");
    _button[3] .init("Joystick0_d3");
    _button[4] .init("Joystick0_d4");
    _button[5] .init("Joystick0_d5");
    _button[6] .init("Joystick0_d6");
    _button[7] .init("Joystick0_d7");
    _button[8] .init("Joystick0_d8");
    _button[9] .init("Joystick0_d9");
    _button[10].init("Joystick0_d10");
    _button[11].init("Joystick0_d11");
    _button[12].init("Joystick0_d12");
    _button[13].init("Joystick0_d13");
    _button[14].init("Joystick0_d14");
    _button[15].init("Joystick0_d15");

    _axes[0].init("Joystick0_a0");
    _axes[1].init("Joystick0_a1");
    _axes[2].init("Joystick0_a2");
    _axes[3].init("Joystick0_a3");
    _axes[4].init("Joystick0_a4");
    _axes[5].init("Joystick0_a5");
    _axes[6].init("Joystick0_a6");
    _axes[7].init("Joystick0_a7");
    _axes[8].init("Joystick0_a8");
    _axes[9].init("Joystick0_a9");

    initRumble();
}

void RumbleApp::initRumble()
{
    _rumble.init("Rumble0");

    _speed = _rumble->createEffect(RumbleEffect::SINE);
    if (_speed) {
	   _speed->load();
    }
    else
    {
        std::cout << "RumbleEffect::SINE not supported" << std::endl;
    }

    _objectHit[0] = _rumble->createEffect(RumbleEffect::SQUARE);
    if (_objectHit[0]) {
	_objectHit[0]->setMagnitude(1);
	_objectHit[0]->setLength(5000);
	_objectHit[0]->load();
    }

    _objectHit[1] = _rumble->createEffect(RumbleEffect::SINE);
    if (_objectHit[1]) {
	_objectHit[1]->setMagnitude(0.25);
	_objectHit[1]->setLength(2000);
	_objectHit[1]->load();
    }

    _objectHit[2] = _rumble->createEffect(RumbleEffect::SINE);
    if (_objectHit[2]) {
	_objectHit[2]->setMagnitude(0.5);
	_objectHit[2]->setLength(500);
	_objectHit[2]->load();
    }

    _objectHit[3] = _rumble->createEffect(RumbleEffect::SINE);
    if (_objectHit[3]) {
	_objectHit[3]->setMagnitude(0.75);
	_objectHit[3]->setLength(2000);
	_objectHit[3]->load();
    }
}

void RumbleApp::preFrame()
{
    if (_speed) {
	_speed->setMagnitude(1.0-_axes[1]->getData());
	_speed->update();
	_speed->play(-1);
    }
    
    for (int i = 0 ; i < 4 ; ++i)
    {
	if (_button[i]->getData() == gadget::Digital::TOGGLE_ON && _objectHit[i])
    {
	    _objectHit[i]->play(1);
        std::cout << "\tButton " << i << " pressed" << std::endl;
    }
    }
}

void RumbleApp::bufferPreDraw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RumbleApp::draw()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    
    glBegin(GL_LINES);
    glVertex3f(-8, -1, -6); glVertex3f( 8, -1, -6);
    glVertex3f(-8,  0, -6); glVertex3f( 8,  0, -6);
    glVertex3f(-8, 10, -6); glVertex3f( 8, 10, -6);
    for (int i = -8 ; i <= 8 ; ++i)
    {
	glVertex3f(i, -1, -6); glVertex3f(i,10,-6);
    }
    glEnd();
    glColor3f(1.0, 0.0, 0.0);
    for (int i = 0 ; i < 16 ; ++i)
	if (_button[i]->getData() == gadget::Digital::ON)
	{
	    glBegin(GL_QUADS);
	    glVertex3f(i-7.75, -.75, -6);
	    glVertex3f(i-7.25, -.75, -6);
	    glVertex3f(i-7.25, -.25, -6);
	    glVertex3f(i-7.75, -.25, -6);
	    glEnd();
	}

    for (int i = 0 ; i < 10 ; ++i)
    {
	float f = _axes[i]->getData() * 10;
	glBegin(GL_LINES);
	glVertex3f(i-8,f,-6); glVertex3f(i-7,f,-6);
	glEnd();
    }
}


