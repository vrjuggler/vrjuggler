#ifndef _VJ_GLAPIFACTORY_
#define _VJ_GLAPIFACTORY_

#include <config.h>
#include <Kernel/vjAPIFactory.h>
#include <Kernel/GL/vjGlDrawManager.h>

//---------------------------------------------------------
//: Singleton Concrete factory class for OpenGL API.
//
//	Responsible for creating API specific Manager objects. 
//
// @author Allen Bierbaum
//  Date: 1-13-98
//---------------------------------------------------------
class vjGlAPIFactory : public vjAPIFactory
{
public:           
   //: Get a Performer DrawManager
   virtual vjDrawManager*    getDrawManager()
   { return vjGlDrawManager::instance();}


   // --- Singleton stuff --- //
protected:
   //: Constructor:  Hidden, so no instantiation is allowed
   vjGlAPIFactory() // Do nothing, but hide
   { ;} 

public:
   //: Get instance of singleton object
   static vjGlAPIFactory* instance()
   {
      if (_instance == NULL)
         _instance = new vjGlAPIFactory;
      return _instance;
   }

private:
   static vjGlAPIFactory* _instance;   //: The instance
};

#endif
