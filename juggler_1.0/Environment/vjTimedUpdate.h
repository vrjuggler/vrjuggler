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
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_TIMED_UPDATE_H_
#define _VJ_TIMED_UPDATE_H_

#include <vjConfig.h>


//------------------------------------------------
//: Parent class for any object whose value can be read periodically by EM
//
//       Right now, the only such objects are vjPerfDataBuffer
//       Additional classes may include an Input reader object...
//
// @author  Christopher Just
//
// Date: 6-9-98
//------------------------------------------------

class vjTimedUpdate {

public:

    //: writes the current contents of this vjTimedUpdate to out
    virtual void write (ostream& out);

    //: gets a name associated with this object
    virtual std::string getName ();
};


#endif
