#ifndef _SharedType_h_
#define _SharedType_h_
//----------------------------------------------
// SharedType
//
//
// Author:
//	Allen Bierbaum
//
// Date: 1-20-97
//-----------------------------------------------

#include <vjConfig.h>
#include <SharedMem/C2Memory.h>

template <class T>
class SharedType : public C2Memory
{
public:
    SharedType(T val) : value(val) {}

    operator T()
	{ return value; }
private:
    T value;
};


#endif
