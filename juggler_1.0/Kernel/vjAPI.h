#ifndef _VJ_API_
#define _VJ_API_

#include <config.h>
#include <iostream.h>
class vjAPIFactory;

//---------------------------------------------------------
// vjAPI: Encapulates interfaces to graphics APIs.
// 
// PURPOSE:
//	 This class is used for the application to tell the
//  Kernel what API it is using.  It is also used to get
//  the singleton instance of the Concrete APIFactory.
//
// @author Allen Bierbaum
//  Date: 9-7-97
//--------------------------------------------------------
class vjAPI
{
public:
   enum
   {UNKNOWN = 0, PERFORMER = 1, OPENGL = 2};

   //: Constructor
   //!ARGS: api - enum of the API to set too
   vjAPI(int api = UNKNOWN)
   { mApi = api;}

   // ---  API QUERY  --- //
public:      
   //: Sets API to Performer
   void setPerformer() 
   { mApi = PERFORMER;}

   //: Sets API to OpenGL
   void setOpenGL()
   { mApi = OPENGL;}

public:      
   //: Test if API is Performer
   //! RETURNS: True - API is Performer
   //! RETURNS: False - API is not Performer
   int Performer()
   { return (mApi == PERFORMER);}
   
   //: Test if API is OpenGL
   //! RETURNS: True - API is OpenGL
   //! RETURNS: False - API is not OpenGL
   int OpenGL()
   { return (mApi == OPENGL);}

private:
   int mApi;    //: The API we are using.

   // --- API Factory --- //
public:
   //: Get the concrete factory for the current API
   //! RETURNS: NULL - API is set incorrectly
   //! RETURNS: other - The API factory to use
   vjAPIFactory* getAPIFactory();
};
 

#endif
