#ifndef _VJ_PFAPIFACTORY_
#define _VJ_PFAPIFACTORY_

#include <config.h>
#include <Kernel/vjAPIFactory.h>
#include <Kernel/Pf/vjPfDrawManager.h>

//-------------------------------------------------------
//: Singleton Concrete factory class for Performer API.
//
//	Responsible for creating API specific Manager objects. 
//
// @author Allen Bierbaum
//  Date: 9-7-97
//--------------------------------------------------------
class vjPfAPIFactory : public vjAPIFactory
{
public:           
   //: Get a Performer DrawManager
   virtual vjDrawManager* getDrawManager();


   // --- Singleton stuff --- //
protected:
   //: Constructor:  Hidden, so no instantiation is allowed
   vjPfAPIFactory() // Do nothing, but hide
   { ;} 

public:
   //: Get instance of singleton object
   static vjPfAPIFactory* instance()
   {
      if (_instance == NULL)
         _instance = new vjPfAPIFactory;
      return _instance;
   }

private: 
   static vjPfAPIFactory* _instance;   //: The instance
};

#endif
