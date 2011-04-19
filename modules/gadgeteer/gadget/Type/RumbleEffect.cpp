/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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

#include <gadget/Type/RumbleEffect.h>
#include <boost/concept_check.hpp>   /* for ignore_unused_variable_warning */

using namespace gadget;

RumbleEffect::~RumbleEffect()
{
}

void RumbleEffect::setDirection(DirectionType dirType,
				const gmtl::Vec3f &direction)
{
   boost::ignore_unused_variable_warning(dirType);
   boost::ignore_unused_variable_warning(direction);
}

gmtl::Vec3f RumbleEffect::getDirection(DirectionType dirType)
{
   boost::ignore_unused_variable_warning(dirType);
   return gmtl::Vec3f(0.0f, 0.0f, 0.0f);
}

RumbleEffect::DirectionType RumbleEffect::getDirectionType()
{
   return POLAR;
}

void RumbleEffect::setLength(int milliseconds)
{
   boost::ignore_unused_variable_warning(milliseconds);
}

int RumbleEffect::getLength()
{
   return 0;
}

void RumbleEffect::setDelay(int milliseconds)
{
   boost::ignore_unused_variable_warning(milliseconds);
}

int RumbleEffect::getDelay()
{
   return 0;
}

void RumbleEffect::setLevel(float val)
{
   boost::ignore_unused_variable_warning(val);
}

float RumbleEffect::getLevel()
{
   return 0.0f;
}

void RumbleEffect::setPeriod(int milliseconds)
{
   boost::ignore_unused_variable_warning(milliseconds);
}

int RumbleEffect::getPeriod()
{
   return 0;
}

void RumbleEffect::setMagnitude(float val)
{
   boost::ignore_unused_variable_warning(val);
}

float RumbleEffect::getMagnitude()
{
   return 0;
}

void RumbleEffect::setPhase(float val)
{
   boost::ignore_unused_variable_warning(val);
}

float RumbleEffect::getPhase()
{
   return 0;
}

void RumbleEffect::setAttackLength(int milliseconds)
{
   boost::ignore_unused_variable_warning(milliseconds);
}

int RumbleEffect::getAttackLength()
{
   return 0;
}

void RumbleEffect::setAttackLevel(float val)
{
   boost::ignore_unused_variable_warning(val);
}

float RumbleEffect::getAttackLevel()
{
   return 0.0f;
}

void RumbleEffect::setFadeLength(int milliseconds)
{
   boost::ignore_unused_variable_warning(milliseconds);
}

int RumbleEffect::getFadeLength()
{
   return 0;
}

void RumbleEffect::setFadeLevel(float val)
{
   boost::ignore_unused_variable_warning(val);
}

float RumbleEffect::getFadeLevel()
{
   return 0.0f;
}

void RumbleEffect::setStart(float val)
{
   boost::ignore_unused_variable_warning(val);
}

float RumbleEffect::getStart()
{
   return 0.0f;
}

void RumbleEffect::setEnd(float val)
{
   boost::ignore_unused_variable_warning(val);
}

float RumbleEffect::getEnd()
{
   return 0.0f;
}

void RumbleEffect::setOffset(float val)
{
   boost::ignore_unused_variable_warning(val);
}

float RumbleEffect::getOffset()
{
   return 0;
}

