/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _SG_XFORMER_H_
#define _SG_XFORMER_H_
#pragma once

#include <sg.h>

//: Base class for all xformers
class sgDllExport sgXformer
{
public:
	sgXformer()
	{
		xformDCS = NULL;
		mouseInput = NULL;
	}
	
	void setRootNode(sgGroup* _root)
	{rootNode = _root; }
	
	void setDCS(sgDCS* _theDCS)
	{ xformDCS = _theDCS; }
	
	void setMouseInput(sgInput* _input)
	{ mouseInput = _input; }
	
	
	/**
	* Funcion to call each frame to update the xform model.
	*/
	virtual void update() = 0;
	
public:
	sgGroup*	rootNode;
	sgDCS*		xformDCS;
	sgInput*	mouseInput;
};

#endif
