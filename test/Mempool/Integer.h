#ifndef _Integer_h_
#define _Integer_h_

#include <config.h>
#include <SharedMem/C2Memory.h>

class Integer : public C2Memory {
public:
    Integer(int i)
    {
	integer = i;
    }

    int value()
    {
	return integer;
    }
private:
    int integer;
    float bogus[100];
};

#endif
