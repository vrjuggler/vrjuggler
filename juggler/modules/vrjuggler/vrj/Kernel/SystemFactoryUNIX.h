#ifndef _VJ_SGISYSTEMFACTORY_
#define _VJ_SGISYSTEMFACTORY_

#include <vjConfig.h>
#include <Kernel/vjSystemFactory.h>

class vjDisplayManager;

//---------------------------------------------------
//: Concrete class for SGI System specific Factory.
//
//	Concrete class to create SGI system specific objects.
//
// @author Allen Bierbaum
//  Date: 9-9-97
//----------------------------------------------------
class vjSGISystemFactory : public vjSystemFactory
{
public:

   /**
	 * POST: Returns an SGI OpenGL window
    */
    virtual vjGlWindow* getGLWindow();


      // --- Singleton stuff --- //
protected:
   /// Constructor:  Hidden, so no instantiation is allowed
   vjSGISystemFactory()   // Do nothing, but hide
   {;}

public:
   /// Get instance of singleton object
   static vjSGISystemFactory* instance()
   {
      if (_instance == NULL)
         _instance = new vjSGISystemFactory;
      return _instance;
   }

private:
   /// The instance
   static vjSGISystemFactory* _instance;
};

#endif
