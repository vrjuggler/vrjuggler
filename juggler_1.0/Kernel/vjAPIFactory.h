#ifndef _VJ_APIFACTORY_
#define _VJ_APIFACTORY_

#include <config.h>

class vjDrawManager;

//-------------------------------------------------------------
//: Abstract base class for API specific Factory.
//
//	In order to create API specific objects, Concrete
//  derived classes must be created. 
//
// @author Allen Bierbaum
//  Date: 9-7-97
//-------------------------------------------------------------
class vjAPIFactory
{
public:
      //: Get the DrawManager to use
   virtual vjDrawManager*    getDrawManager() = 0;
};

#endif
