#ifndef _DEFINES_H_
#define _DEFINES_H_

namespace kev
{
   // returns the maximum of a and b.
   template<class T> 
   inline static const T& Max( const T& a, const T& b )
   { 
      return (a>=b) ? a : b; 
   }
   // returns the minimum of a and b.
   template<class T> 
   inline static const T& Min( const T& a, const T& b )
   { 
      return (a<b) ? a : b; 
   }
};

#endif /* _DEFINES_H_ */
