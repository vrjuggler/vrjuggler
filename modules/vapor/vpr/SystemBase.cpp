/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <cstdlib>
#include <sstream>
#include <string>

#if defined(HAVE_BACKTRACE)
#  include <sys/types.h>
#  include <unistd.h>
#  include <execinfo.h>
#elif defined(VPR_OS_Darwin)
#  include <iomanip>
#  include <vector>
#  include <boost/algorithm/string/predicate.hpp>

extern "C"
{

#  include <mach-o/dyld.h>
#  include <mach-o/loader.h>
#  include <mach-o/nlist.h>
#  include <mach-o/stab.h>

}

#elif defined(VPR_OS_Windows)
#  if _MSC_VER >= 1400
#     include <intrin.h>
#  endif

#  include <dbghelp.h>
#  include <iomanip>
#  include <boost/format.hpp>
#endif


#if (! defined(__INTEL_COMPILER) && defined(__GNUC__) && !defined(__PATHCC__) && \
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

#if defined(VPR_OS_Windows) && defined(VPR_DEBUG)
#if _MSC_VER >= 1400
#  pragma intrinsic(_ReturnAddress)
#endif
#pragma auto_inline(off)
DWORD_PTR getProgramCounter()
{
#if _MSC_VER >= 1400
   // The program counter for the caller will be the return address of this
   // stack frame.
   return (DWORD_PTR) _ReturnAddress();
#else
   DWORD_PTR program_counter;

   // Get the return address from the current stack frame
   // and store it in program_counter.
   __asm mov eax, [ebp + 4]
   __asm mov [program_counter], eax

   return program_counter;
#endif
}
#pragma auto_inline(on)

inline DWORD_PTR getFrameOffset()
{
   DWORD_PTR frame_offset;
   EXCEPTION_POINTERS* exp_ptr(NULL);

   __try
   {
      throw 0;
   }
   __except(((exp_ptr = GetExceptionInformation()) ? EXCEPTION_EXECUTE_HANDLER
                                                   : EXCEPTION_EXECUTE_HANDLER))
   {
      /* Do nothing. */ ;
   }

   if ( NULL != exp_ptr )
   {
#if defined(_M_IX86)
      frame_offset = exp_ptr->ContextRecord->Esp;
#elif defined(_M_X64)
      frame_offset = exp_ptr->ContextRecord->Rsp;
#elif defined(_M_IA64)
      frame_offset = exp_ptr->ContextRecord->RsBSP;
#endif
   }

   return frame_offset;
}

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

std::string demangleTraceString(const std::string& traceLine)
{
#ifdef USE_CXA_DEMANGLE
   // Try to extract the mangled name from the line (if it exists)
   // and replace it with a demangled version of the name.
   // Example: build.linux/stuff/classfile(_ZN4vpr11Someing33methodEv+0xd3) [0x80cfa29]
   // For Mac OS X, things are a little different. A symbol may be of the
   // form _ZN3vpr9ExceptionC2ERKSsS2_:F(0,1), or it may be a simple function
   // name, probably with a leading underscore.

   std::string trace_line(traceLine);
   std::string mangled_name, demangled_name;

   std::string::size_type start(std::string::npos), end(std::string::npos);

#if defined(VPR_OS_Darwin) && ! defined(HAVE_BACKTRACE)
   end = trace_line.find(":F(");

   // If trace_line does not contain ":F(", then we set the start to be either
   // 0 or 1 depending on whether trace_line starts with "_", and we set end
   // to be the end of the string.
   if ( std::string::npos == end )
   {
      start = boost::algorithm::starts_with(trace_line, "_") ? 1 : 0;
      end   = trace_line.size();
   }
   else
   {
      start = 0;
   }
#else
   start = trace_line.find("(_");
   if(std::string::npos != start)
   {
      end = trace_line.find_first_of("+)",start);
   }
#endif

   if(std::string::npos != end)
   {
      std::string::size_type assign_start, assign_end;
#if defined(VPR_OS_Darwin) && ! defined(HAVE_BACKTRACE)
      assign_start = start;
      assign_end   = end;
#else
      assign_start = start + 1;
      assign_end   = end - start - 1;
#endif
      mangled_name.assign(trace_line, assign_start, assign_end);

      int status;
      char* demangled_buf = abi::__cxa_demangle(mangled_name.c_str(), NULL,
                                                NULL, &status);
      if(0==status)
      {
#if defined(VPR_OS_Darwin) && ! defined(HAVE_BACKTRACE)
         trace_line = demangled_buf;
#else
         trace_line.replace(start + 1, end - start - 1, demangled_buf);
#endif
         std::free(demangled_buf);
      }
      else if(-1==status)
      {
//         std::cerr << "vpr::SystemBase::dmangleTraceString: "
//                   << "A memory allocation failiure occurred.\n";
      }
      else if(-2 == status)
      {
//         std::cerr << "vpr::SystemBase::demangleTraceString: mangled_name "
//                   << "is not a valid name under the C++ ABI mangling "
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

#if defined(VPR_OS_Darwin) && ! defined(HAVE_BACKTRACE)
struct CrawlFrame
{
   unsigned int pc;
   size_t frame;
   std::string name;
   unsigned int offset;
};

const mach_header* findOwnerOfPC(const unsigned int pc)
{
   const unsigned int count = _dyld_image_count();
   for ( unsigned int index = 0; index < count; ++index )
   {
      const mach_header* header = _dyld_get_image_header(index);
      const unsigned int offset = _dyld_get_image_vmaddr_slide(index);
      const load_command* cmd =
         reinterpret_cast<const load_command*>(
            reinterpret_cast<const char*>(header) + sizeof(mach_header)
         );

      for ( unsigned int cmdex = 0;
            cmdex < header->ncmds;
            ++cmdex, cmd = reinterpret_cast<const load_command*>(reinterpret_cast<const char*>(cmd) + cmd->cmdsize))
      {
         switch (cmd->cmd)
         {
            case LC_SEGMENT:
               {
                  const segment_command* seg =
                     reinterpret_cast<const segment_command*>(cmd);
                  if ( pc >= (seg->vmaddr + offset) &&
                       pc < (seg->vmaddr + offset + seg->vmsize) )
                  {
                     return header;
                  }
               }
               break;
         }
      }
   }

   return NULL;
}

std::string getFunctionName(const unsigned int pc, unsigned int* offset)
{
   const mach_header* header = findOwnerOfPC(pc);
   if ( header != NULL )
   {
      const segment_command* seg_linkedit(NULL);
      const segment_command* seg_text(NULL);
      const symtab_command* symtab(NULL);

      const load_command* cmd =
         reinterpret_cast<const load_command*>(
            reinterpret_cast<const char*>(header) + sizeof(mach_header)
         );

      for ( unsigned int index = 0;
            index < header->ncmds;
            ++index, cmd = reinterpret_cast<const load_command*>(reinterpret_cast<const char*>(cmd) + cmd->cmdsize) )
      {
         switch (cmd->cmd)
         {
            case LC_SEGMENT:
               if ( ! strncmp(reinterpret_cast<const segment_command*>(cmd)->segname, SEG_TEXT, 16) )
               {
                  seg_text = reinterpret_cast<const segment_command*>(cmd);
               }
               else if ( ! strncmp(reinterpret_cast<const segment_command*>(cmd)->segname, SEG_LINKEDIT, 16) )
               {
                  seg_linkedit = reinterpret_cast<const segment_command*>(cmd);
               }
               break;
            
            case LC_SYMTAB:
               symtab = reinterpret_cast<const symtab_command*>(cmd);
               break;
         }
      }
      
      if ( seg_text == NULL || seg_linkedit == NULL || symtab == NULL )
      {
         *offset = 0;
         return NULL;
      }

      size_t vm_slide = reinterpret_cast<size_t>(header) - seg_text->vmaddr;
      const size_t file_slide = (seg_linkedit->vmaddr - seg_text->vmaddr) -
                                   seg_linkedit->fileoff;
      struct nlist* symbase =
         reinterpret_cast<struct nlist*>(
            reinterpret_cast<size_t>(header) + symtab->symoff + file_slide
         );
      char* strings =
         reinterpret_cast<char*>(
            reinterpret_cast<size_t>(header) + symtab->stroff + file_slide
         );
      
      // Look for a global symbol.
      unsigned int index;
      struct nlist* sym(NULL);
      for ( index = 0, sym = symbase; index < symtab->nsyms; ++index, ++sym )
      {
         if ( sym->n_type != N_FUN )
         {
            continue;
         }

         char* name = sym->n_un.n_strx ? (strings + sym->n_un.n_strx) : NULL;
         const unsigned int base = sym->n_value + vm_slide;

         for ( index += 1, sym += 1; index < symtab->nsyms; ++index, ++sym )
         {
            if ( sym->n_type == N_FUN )
            {
               break;
            }
         }

         if ( pc >= base  &&  pc <= (base + sym->n_value) && name != NULL &&
              std::strlen(name) > 0 )
         {
            *offset = pc - base;
            return std::string(name);
         }
      }

      // Look for a reasonably close private symbol.
      char* name;
      unsigned int base;
      for ( name = NULL, base = 0xFFFFFFFF, index = 0, sym = symbase;
            index < symtab->nsyms;
            ++index, ++sym )
      {
         if ( (sym->n_type & 0x0E) != 0x0E )
         {
            continue;
         }

         if ( (sym->n_value + vm_slide) > pc )
         {
            continue;
         }

         if ( base != 0xFFFFFFFF &&
              (pc - (sym->n_value + vm_slide)) >= (pc - base) )
         {
            continue;
         }

         name = sym->n_un.n_strx ? (strings + sym->n_un.n_strx) : NULL;
         base = sym->n_value + vm_slide;
      }

      *offset = pc - base;
      return (name != NULL) ? std::string(name) : std::string();
   }
   
   *offset = 0;
   return std::string();
}
#endif  /* VPR_OS_Darwin */

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

   size = backtrace(trace_syms, 100);
   strings = backtrace_symbols(trace_syms, size);

   std::ostringstream trace_stream;
   trace_stream << "Stack trace for process: " << getpid() << std::endl;

   for (size_t i = 0; i < size; ++i)
   {
      trace_stream << "   " << i << ":" << demangleTraceString(strings[i])
                   << std::endl;
   }

   free(strings);

   ret_stack = trace_stream.str();
#elif defined(VPR_OS_Darwin)
#if defined(__ppc__) || defined(__ppc64__)
   struct StackFrame
   {
      unsigned int savedSP;
      unsigned int savedCR;
      unsigned int savedLR;
      unsigned int reserved[2];
      unsigned int savedRTOC;
   };
#elif defined(__i386__)
   struct StackFrame
   {
      unsigned int savedSP;
      unsigned int savedLR;
   };
#else
#error Unknown platform
#endif

   std::ostringstream trace_stream;
   trace_stream << "Stack trace for process: " << getpid() << std::endl;

   std::vector<CrawlFrame> stack;
   StackFrame* frame(NULL);

#if defined(__ppc__) || defined(__ppc64__)
   frame = *(StackFrame**) __builtin_frame_address(0);
#elif defined(__i386__)
   frame = (StackFrame*) __builtin_frame_address(0);
#endif

   for ( ; frame != NULL; frame = (StackFrame*) frame->savedSP)
   {
      if ( (frame->savedLR & ~3) == 0 || (~(frame->savedLR) & ~3) == 0 )
      {
         break;
      }

      CrawlFrame cur_frame;
      cur_frame.pc    = frame->savedLR;
      cur_frame.frame = reinterpret_cast<size_t>(frame);

      const std::string func_name = getFunctionName(frame->savedLR,
                                                    &cur_frame.offset);

      if ( ! func_name.empty() )
      {
         cur_frame.name = demangleTraceString(func_name);
      }

      if ( cur_frame.pc != 0 )
      {
         cur_frame.pc -= 4;
      }

      stack.push_back(cur_frame);
   }

   typedef std::vector<CrawlFrame>::reverse_iterator iter_type;
   for ( iter_type i = stack.rbegin(); i != stack.rend(); ++i )
   {
      std::string func_name("(unknown)");

      if ( ! (*i).name.empty() )
      {
         typedef std::string::size_type size_type;
         const size_type name_len((*i).name.size());
         for ( size_type j = 0; j < name_len; ++j )
         {
            bool objc(false);
            if ( j == 1  &&  (*i).name[j] == '[' )
            {
               objc = true;
            }

            if ( objc && (*i).name[j] == ']' )
            {
               objc = false;
            }

            if ( ! objc && (*i).name[j] == ':' )
            {
               break;
            }
         }

         std::ostringstream func_stream;
         func_stream << (*i).name << "+" << std::hex << (*i).offset;
         func_name = func_stream.str();
      }

      const char fill_char(trace_stream.fill());
      const std::ios::fmtflags fmt_flags(trace_stream.flags());
      trace_stream.fill('0');
      trace_stream.setf(std::ios::uppercase);
      trace_stream.setf(std::ios::hex | std::ios::right,
                        std::ios::adjustfield | std::ios::basefield);
      trace_stream << "  " << std::setw(sizeof(void*) * 2) << (*i).frame
                   << "  " << std::setw(sizeof(void*) * 2) << (*i).pc;
      trace_stream.flags(fmt_flags);
      trace_stream.fill(fill_char);
      trace_stream << "  " << func_name << std::endl;
   }

   ret_stack = trace_stream.str();
#elif defined(VPR_OS_Windows)
// XXX: The code for getting the stack trace on amd64 and ia64 is not working.
// GetStackWalk64() always returns failure on the first call. This may be the
// result of frame_offset being set incorrectly. Without having inline
// assembly on amd64 or ia64 (as of Visual C++ 8.0), it is not clear how to
// deal with these architectures.
#if defined(VPR_DEBUG) && defined(_M_IX86)
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
      DWORD_PTR frame_offset(NULL);

#if defined(_M_IX86)
      arch = IMAGE_FILE_MACHINE_I386;
      __asm mov [frame_offset], ebp
#elif defined(_M_X64) || defined(_M_IA64)
#  if defined(_M_X64)
      arch = IMAGE_FILE_MACHINE_AMD64;
#  elif defined(_M_IA64)
      arch = IMAGE_FILE_MACHINE_IA64;
#  endif

      frame_offset = getFrameOffset();
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
#else   /* ! defined(VPR_DEBUG) */
   ret_stack = "Stack trace:\n"
               "   <Call stack printing not supported>\n"
               "   Use a debug build to get a stack trace\n";
#endif  /* defined(VPR_DEBUG) */
#endif  /* defined(HAVE_BACKTRACE) */

   return ret_stack;
}

}
