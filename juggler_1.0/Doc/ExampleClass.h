/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



//-------------------------------------
//: This is an example class.
//
// It is an example of how the VR Juggler
// classes should be written
//---------------------------------------
class ExampleClass
{
public:
   //  Example constructor
   //
   //! PRE: None
   //! POST: Object is constructed
   ExampleClass()
   {
      mIntVariable = 0;
      mFloatVariable = 0.0f;
   }

public:
   //:  Brief header comment
   //  More detailed header comment here.  This more detailed
   // comment tells more information about what the function
   // is doing and maybe how it does it.
   //
   //! PRE: No Header Precondition
   //! POST: No Header Post conditions
   //
   //! ARGS: junkValue - This is a vlaue that passes in
   //+                     some junk of some sort
   //! ARGS: junkValue2 - This is some other value that
   //+                     we really don't need to use.
   //
   //! RETURNS: 1 - Successfully did it's thing
   //! RETURNS: 0 - Failed miserably
   //
   //! NOTE: This is a small notice of some exceptional
   //+       thing this function does.
   //
   //! MODIFIES: mIntVariable will be modified
   int doSomething(int junkValue, int junkValue2);

private:
   int   mIntVariable;     // Example integer variable
   float mFloatVariable;   // Example float variable
};
