#ifndef MACROS_AND_DEFINES_FOR_GLOVE_APP
#define MACROS_AND_DEFINES_FOR_GLOVE_APP

// Macros and defines:
namespace gloveDefined
{
    //: MAX macro
    //  returns - the maximum of a, b, and c.
    template<class T> 
    inline const T& nMax( const T& a, const T& b, const T& c )
	     { return (a>=b) ? ((a>=c)?a:c) : ((b>=c)?b:c); }
    
    //: MIN macro
    //  returns - the minimum of a, b, and c.
    template<class T> 
    inline const T& nMin( const T& a, const T& b, const T& c )
	     { return (a<=b) ? ((a<=c)?a:c) : ((b<=c)?b:c); }
    
    // Converts a number in radians to a number in degrees
    const double    TO_DEG_D         = 57.2957795131;
    
    // Converts a number in radians to a number in degrees
    const float     TO_DEG_F         = 57.2957795131f;
    
    // Converts a number in degrees to a number in radians
    const double    TO_RAD_D         = 0.01745329252;
    
    // Converts a number in degrees to a number in radians
    const float     TO_RAD_F         = 0.01745329252f;
};

using namespace gloveDefined;

#endif
