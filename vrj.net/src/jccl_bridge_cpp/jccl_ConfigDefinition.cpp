// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <jccl/Config/ConfigDefinition.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API boost::shared_ptr< jccl::ConfigDefinition >* jccl_ConfigDefinition_ConfigDefinition__0()
   {
      boost::shared_ptr< jccl::ConfigDefinition >* obj = new boost::shared_ptr< jccl::ConfigDefinition >(new jccl::ConfigDefinition());
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API boost::shared_ptr< jccl::ConfigDefinition >* jccl_ConfigDefinition_ConfigDefinition__std_string_cppdom_NodePtr2(const char* p0, cppdom::NodePtr* p1)
   {
      boost::shared_ptr< jccl::ConfigDefinition >* obj = new boost::shared_ptr< jccl::ConfigDefinition >(new jccl::ConfigDefinition(p0, *p1));
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API boost::shared_ptr< jccl::ConfigDefinition >* jccl_ConfigDefinition_ConfigDefinition__jccl_ConfigDefinition1(const jccl::ConfigDefinition* p0)
   {
      const jccl::ConfigDefinition& marshal_p0 = *p0;
      boost::shared_ptr< jccl::ConfigDefinition >* obj = new boost::shared_ptr< jccl::ConfigDefinition >(new jccl::ConfigDefinition(marshal_p0));
      return obj;
   }

   SHARPPY_API void delete_jccl_ConfigDefinition(boost::shared_ptr< jccl::ConfigDefinition >* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method jccl::ConfigDefinition::assertValid()
   SHARPPY_API void jccl_ConfigDefinition_assertValid__0(boost::shared_ptr< jccl::ConfigDefinition >* self_ptr)
   {
      (*self_ptr)->assertValid();
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::getName()
   SHARPPY_API char* jccl_ConfigDefinition_getName__0(boost::shared_ptr< jccl::ConfigDefinition >* self_ptr)
   {
      std::string temp_result;
      char* result;
      temp_result = (*self_ptr)->getName();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::getToken()
   SHARPPY_API char* jccl_ConfigDefinition_getToken__0(boost::shared_ptr< jccl::ConfigDefinition >* self_ptr)
   {
      std::string temp_result;
      char* result;
      temp_result = (*self_ptr)->getToken();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::getVersion()
   SHARPPY_API unsigned int jccl_ConfigDefinition_getVersion__0(boost::shared_ptr< jccl::ConfigDefinition >* self_ptr)
   {
      unsigned int result;
      result = (*self_ptr)->getVersion();
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::getHelp()
   SHARPPY_API char* jccl_ConfigDefinition_getHelp__0(boost::shared_ptr< jccl::ConfigDefinition >* self_ptr)
   {
      std::string temp_result;
      char* result;
      temp_result = (*self_ptr)->getHelp();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::operator ==()
   SHARPPY_API bool jccl_ConfigDefinition_equal__jccl_ConfigDefinition1(boost::shared_ptr< jccl::ConfigDefinition >* self_ptr, const jccl::ConfigDefinition* p0)
   {
      const jccl::ConfigDefinition& marshal_p0 = *p0;
      bool result;
      result = (*self_ptr)->operator==(marshal_p0);
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::operator !=()
   SHARPPY_API bool jccl_ConfigDefinition_not_equal__jccl_ConfigDefinition1(boost::shared_ptr< jccl::ConfigDefinition >* self_ptr, const jccl::ConfigDefinition* p0)
   {
      const jccl::ConfigDefinition& marshal_p0 = *p0;
      bool result;
      result = (*self_ptr)->operator!=(marshal_p0);
      return result;
   }


} // extern "C" for jccl::ConfigDefinition


