/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#if defined(HAVE_BACKTRACE)
#include <sys/types.h>
#include <unistd.h>
#include <execinfo.h>

#include <string>
#include <sstream>
#endif

#if (! defined(__INTEL_COMPILER) && defined(__GNUC__) && \
     ((__GNUC__ == 3 && __GNUC_MINOR__ >= 3) || __GNUC__ > 3)) || \
    (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 810 && defined(__GNUC__))

#define USE_CXA_DEMANGLE 1
#include <cxxabi.h>

#endif

#include <vpr/SystemBase.h>

namespace
{

std::string demangleTraceString(char* traceLine)
{
#ifdef USE_CXA_DEMANGLE
   // Try to extract the mangled name from the line (if it exists)
   // and replace it with a demangled version of the name.
   // Example: build.linux/stuff/classfile(_ZN4vpr11Someing33methodEv+0xd3) [0x80cfa29]

   std::string trace_line(traceLine);
   std::string mangled_name, demangled_name;

   unsigned start(std::string::npos), end(std::string::npos);
   start = trace_line.find("(_");
   if(std::string::npos != start)
   {
      end = trace_line.find_first_of("+)",start);
   }

   if(std::string::npos != end)
   {
      mangled_name.assign(trace_line, start+1, end-start-1);
      int status;
      char* demangled_buf = abi::__cxa_demangle(mangled_name.c_str(), NULL,
                                                NULL, &status);
      if(0==status)
      {
         demangled_name = std::string(demangled_buf);
         free(demangled_buf);

         trace_line.replace(start+1, (end-start-1), demangled_name);
      }
      else if(-1==status)
      {
//         std::cerr << "vpr::SystemBase::dmangleTraceString: "
//                   << "A memory allocation failiure occurred.\n";
      }
      else if(-2 == status)
      {
//         std::cerr << "vpr::SystemBase::demangleTraceString: mangled_name "
//                   << "is not a valid name under the C++ ABI mangling '
//                   << "rules.\n";
      }
      else if(-3 == status)
      {
//         std::cerr << "vpr::SystemBase::demangleTraceString: One of the "
//                   << "arguments is invalid.\n";
      }
   }

//   trace_line += std::string(" mangle:") + mangled_name +
//                 std::string(" demangled:") + demangled_name;
   return trace_line;

#else
   return std::string(traceLine);
#endif
}

}

namespace vpr
{

std::string SystemBase::getCallStack()
{
   std::string ret_stack("Stack trace:\n   <Call stack printing not supported>\n");

#if defined(HAVE_BACKTRACE)
   void* trace_syms[100];
   size_t size;
   char** strings;

   pid_t cur_pid = getpid();
   size = backtrace(trace_syms, 100);
   strings = backtrace_symbols(trace_syms, size);

   std::ostringstream trace_stream;
   trace_stream << "Stack trace: thread: " << cur_pid << std::endl;

   for (size_t i = 0; i < size; ++i)
   {
      trace_stream << "   " << i << ":" << demangleTraceString(strings[i])
                   << std::endl;
   }

   free(strings);

   ret_stack = trace_stream.str();
#endif

   return ret_stack;
}

}
