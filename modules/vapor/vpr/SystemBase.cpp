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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
#  include <sys/types.h>
#  include <unistd.h>
#  include <execinfo.h>
#  include <sstream>
#elif defined(VPR_OS_Windows)
#  include <stdlib.h>
#  include <dbghelp.h>
#  include <iomanip>
#  include <sstream>
#  include <boost/format.hpp>
#endif

#include <string>

#if (! defined(__INTEL_COMPILER) && defined(__GNUC__) && \
     ((__GNUC__ == 3 && __GNUC_MINOR__ >= 3) || __GNUC__ > 3)) || \
    (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 810 && defined(__GNUC__))

#define USE_CXA_DEMANGLE 1
#include <cxxabi.h>

#endif

#include <vpr/SystemBase.h>

// In case _M_X64 is not defined, define it so that we do not have to test
// for _M_AMD64 and _M_X64 repeatedly.
#if defined(_M_AMD64) && ! defined(_M_X64)
#  define _M_X64 _M_AMD64
#endif


// Right now, Boost.Format is only used by Windows-specific code in this file.
#if defined(VPR_OS_Windows)
using boost::format;
#endif

namespace
{
#if defined(VPR_OS_Windows) && defined(_DEBUG)
// NOTE: There is no IA64 version of this function since the IA64 version
// of Windows has been discontinued.
#if defined(_M_IX86) || defined(_M_X64)
#pragma auto_inline(off)
DWORD_PTR getProgramCounter()
{
#  if defined(_M_IX86)
#     define PTR_SIZE 4
#     define AXREG eax
#     define BPREG ebp
#  elif defined(_M_X64)
#     define PTR_SIZE 8
#     define AXREG rax
#     define BPREG rbp
#  endif   /* defined(_M_IX86) */

   DWORD_PTR program_counter;

   // Get the return address from the current stack frame
   // and store it in program_counter.
   __asm mov AXREG, [BPREG + PTR_SIZE]
   __asm mov [program_counter], AXREG

   return program_counter;

#  undef BPREG
#  undef AXREG
#  undef PTR_SIZE
}
#pragma auto_inline(on)
#endif   /* defined(_M_IX86) || defined(_M_X64) */

// This is based on WheatyExceptionReport::GetLogicalAddress().  The
// original can be found in the March 2002 issue of MSDN Magazine:
//
//   http://msdn.microsoft.com/msdnmag/issues/02/03/hood/default.aspx
bool getLogicalAddress(void* addr, char* szModule, const DWORD len,
                       DWORD& section, DWORD& offset)
{
   MEMORY_BASIC_INFORMATION mbi;

   if ( ! VirtualQuery(addr, &mbi, sizeof(mbi)) )
   {
      return false;
   }

   DWORD64 h_module = (DWORD64) mbi.AllocationBase;

   if ( ! GetModuleFileName((HMODULE) h_module, szModule, len) )
   {
      return false;
   }

   // Point to the DOS header in memory.
   PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER) h_module;

   // From the DOS header, find the NT (PE) header.
   PIMAGE_NT_HEADERS nt_header =
      (PIMAGE_NT_HEADERS) (h_module + dos_header->e_lfanew);

   PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(nt_header);

   // rva is offset from module load address.
   DWORD64 rva = (DWORD64) addr - h_module;

   // Iterate through the section table, looking for the one that encompasses
   // the linear address.
   for ( unsigned int i = 0;
         i < nt_header->FileHeader.NumberOfSections;
         ++i, ++pSection )
   {
      DWORD sectionStart = pSection->VirtualAddress;
      DWORD sectionEnd = sectionStart +
         max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

      // Is the address in this section???
      if ( rva >= sectionStart && rva <= sectionEnd )
      {
         // Yes, address is in the section.  Calculate section and offset,
         // and store in the "section" & "offset" params, which were
         // passed by reference.
         section = i + 1;
         offset = rva - sectionStart;
         return true;
      }
   }

   return false;   // Should never get here!
}
#endif   /* ifdef VPR_OS_Windows */

std::string demangleTraceString(char* traceLine)
{
#ifdef USE_CXA_DEMANGLE
   // Try to extract the mangled name from the line (if it exists)
   // and replace it with a demangled version of the name.
   // Example: build.linux/stuff/classfile(_ZN4vpr11Someing33methodEv+0xd3) [0x80cfa29]

   std::string trace_line(traceLine);
   std::string mangled_name, demangled_name;

   std::string::size_type start(std::string::npos), end(std::string::npos);
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
#elif defined(VPR_OS_Windows)
#if defined(_DEBUG)
   // This will be used over and over again below.  In particular, we need
   // to be sure that SymInitialize() and SymCleanup() are called with the
   // same process handle value.
   const HANDLE proc = GetCurrentProcess();

   // Set up the symbol loading and handling options.
   SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);

   // Initialize the symbol loading.  If this fails, there is no way we can
   // put together a meaningful stack trace.
   if ( SymInitialize(proc, NULL, TRUE) )
   {
      DWORD arch;
      STACKFRAME64 sf;
      CONTEXT ctx;
      DWORD_PTR frame_offset;

      // NOTE: There is no IA64 case here because the IA64 version of Windows
      // has been discontinued.
#if defined(_M_IX86)
      arch = IMAGE_FILE_MACHINE_I386;
      __asm mov [frame_offset], ebp
#elif defined(_M_X64)
      arch = IMAGE_FILE_MACHINE_AMD64;
      __asm mov [frame_offset], rbp
#endif

      // Initialize the stack frame for this frame.  If this is not done,
      // StackWalk64() will not work.
      memset(&sf, NULL, sizeof(sf));
      sf.AddrPC.Offset    = getProgramCounter();
      sf.AddrPC.Mode      = AddrModeFlat;
      sf.AddrFrame.Offset = frame_offset;
      sf.AddrFrame.Mode   = AddrModeFlat;

      std::stringstream trace_stream;

      // Account for architecture-specific pointer value sizes.
#ifdef VPR_OS_Win32
      const unsigned int width(8);
#else
      const unsigned int width(16);
#endif

      trace_stream << "Address" << "  " << std::setw(width)
                   << "Frame" << "    " << std::setw(width)
                   << "Function            Source File" << std::endl;

      while ( 1 )
      {
         bool more = StackWalk64(arch, proc, GetCurrentThread(), &sf,
                                 &ctx, NULL, SymFunctionTableAccess64,
                                 SymGetModuleBase64, NULL);

         // End of the stack frame.
         if ( ! more || 0 == sf.AddrFrame.Offset )
         {
            break;
         }

         // Use appropriate pointer width formatting for 32- and 64-bit
         // architectures.
#ifdef VPR_OS_Win32
         format addr("%1$+08x    %2$+08x ");
#else
         format addr("%1$+016x    %2$+016x ");
#endif
         trace_stream << addr % sf.AddrPC.Offset % sf.AddrFrame.Offset;

         // Get the name of the function for this stack frame entry.
         BYTE symbol_buffer[sizeof(SYMBOL_INFO) + 1024];
         PSYMBOL_INFO symbol_info = (PSYMBOL_INFO) symbol_buffer;
         symbol_info->SizeOfStruct = sizeof(symbol_buffer);
         symbol_info->MaxNameLen = 1024;

         // Displacement of the input address relative to the start of the
         // symbol.
         DWORD64 sym_displacement(0);

         bool got_symbol = SymFromAddr(proc, sf.AddrPC.Offset,
                                       &sym_displacement, symbol_info);

         if ( got_symbol )
         {
            format sym_addr("%1%+%2$+llx");
            trace_stream << sym_addr % symbol_info->Name % sym_displacement;
         }
         // If no symbol was found, print out the logical address instead.
         // See getLogicalAddress() above.
         else
         {
            char sz_module[MAX_PATH] = "";
            DWORD section = 0, offset = 0;

            getLogicalAddress((void*) sf.AddrPC.Offset, sz_module,
                              sizeof(sz_module), section, offset);

            // Use appropriate pointer width formatting for 32- and 64-bit
            // architectures.
#ifdef VPR_OS_Win32
            format log_addr("%1$+04x:%2$+08x %3%");
#else
            format log_addr("%1$+04x:%2$+016x %3%");
#endif
            trace_stream << log_addr % section % offset % sz_module;
         }

         // Get the source line for this stack frame entry.
         IMAGEHLP_LINE64 line_info = { sizeof(IMAGEHLP_LINE64) };
         DWORD line_displacement;
         bool got_line = SymGetLineFromAddr64(proc, sf.AddrPC.Offset,
                                              &line_displacement, &line_info);

         if ( got_line )
         {
            trace_stream << "  " << line_info.FileName << " line "
                         << line_info.LineNumber;
         }

         trace_stream << std::endl;
/*
         // Write out the variables.
         // Use SymSetContext to get just the locals/params for this frame
         IMAGEHLP_STACK_FRAME imghlp_sf;
         imghlp_sf.InstructionOffset = sf.AddrPC.Offset;
         SymSetContext(proc, &imghlp_sf, 0);

         // Enumerate the locals/parameters
         SymEnumSymbols(proc, 0, 0, EnumerateSymbolsCallback, &sf);

         trace_stream << std::endl;
*/
      }

      // We only overwrite ret_stack if we were successful in building up
      // the stack trace text.
      ret_stack = trace_stream.str();

      SymCleanup(proc);
   }
#else   /* ! defined(_DEBUG) */
   ret_stack = "Stack trace:\n"
               "   <Call stack printing not supported>\n"
               "   Use a debug build to get a stack trace\n";
#endif  /* defined(_DEBUG) */
#endif  /* defined(HAVE_BACKTRACE) */

   return ret_stack;
}

}
