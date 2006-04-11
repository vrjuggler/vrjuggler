#ifndef _MY_SUITES_H_
#define _MY_SUITES_H_

// MySuites.h
#include <string>

namespace jcclTest
{
   namespace MySuites 
   {
   inline std::string metric() 
   { 
     return "metric";
   }
   
   inline std::string noninteractive() 
   { 
     return "noninteractive";
   }

   inline std::string interactive() 
   { 
     return "interactive";
   }

 }
}

#endif

