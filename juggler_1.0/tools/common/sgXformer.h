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
