
namespace kev
{
   // returns the maximum of a and b.
   template<class T> 
   inline static const T& max( const T& a, const T& b )
   { 
      return (a>=b) ? a : b; 
   }
   // returns the minimum of a and b.
   template<class T> 
   inline static const T& min( const T& a, const T& b )
   { 
      return (a<b) ? a : b; 
   }
};
