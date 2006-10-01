#include <fstream>
#include <string>

#include <vpr/vpr.h>
#include <vpr/DynLoad/Library.h>
#include <vpr/System.h>
#include <vpr/Util/Exception.h>
#include <TestCases/DynLoad/modules/TestInterface.h>

#include <TestCases/DynLoad/LibraryTest.h>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( LibraryTest );

#if defined(VPR_OS_Windows)
static const std::string C_MOD("cmod.dll");
static const std::string CXX_MOD("cxxmod.dll");
#elif defined(VPR_OS_Darwin)
static const std::string C_MOD("libcmod.dylib");
static const std::string CXX_MOD("libcxxmod.dylib");
#else
static const std::string C_MOD("libcmod.so");
static const std::string CXX_MOD("libcxxmod.so");
#endif

void LibraryTest::setUp()
{
   std::string c_lib_path(MODULE_DIR);
   c_lib_path += "/" + C_MOD;
   mCModuleName = c_lib_path;

   std::string cxx_lib_path(MODULE_DIR);
   cxx_lib_path += "/" + CXX_MOD;
   mCxxModuleName = cxx_lib_path;
}

void LibraryTest::namingTest()
{
   const std::string lib_name("libTest.so");

   vpr::Library lib(lib_name);
   CPPUNIT_ASSERT(lib.getName() == lib_name && "Names do not match");
}

void LibraryTest::loadTest()
{
   // Test for library existence.  This could be done better.
   std::ifstream mod_file;
   mod_file.open(mCModuleName.c_str());
   CPPUNIT_ASSERT(mod_file.good() && "Module does not exist");
   mod_file.close();

   try
   {
      vpr::Library c_lib(mCModuleName);
      c_lib.load();
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false && "Load failed");
   }
}

void LibraryTest::unloadTest()
{
   // Test for library existence.  This could be done better.
   std::ifstream mod_file;
   mod_file.open(mCModuleName.c_str());
   CPPUNIT_ASSERT(mod_file.good() && "Module does not exist");
   mod_file.close();

   try
   {
      vpr::Library c_lib(mCModuleName);
      c_lib.load();

      try
      {
         c_lib.unload();
      }
      catch (vpr::Exception&)
      {
         CPPUNIT_ASSERT(false && "Unload failed");
      }
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false && "Load failed");
   }
}

void LibraryTest::libraryPathTest()
{
   bool status;
   status = vpr::System::setenv(std::string("LD_LIBRARY_PATH"),
                                std::string(MODULE_DIR));
   CPPUNIT_ASSERT(status && "Failed to extend library path");

   try
   {
      vpr::Library lib(C_MOD);
      lib.load();
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false && "Library load failed");
   }
}

void LibraryTest::loadCSymbolTest()
{
   try
   {
      int (*test_func)();

      vpr::Library c_lib(mCModuleName);
      c_lib.load();

      // This is the weirdest cast I have ever written.
      test_func = (int(*)()) c_lib.findSymbol("function");
      CPPUNIT_ASSERT(NULL != test_func && "Symbol lookup failed");

      int result = (*test_func)();
      CPPUNIT_ASSERT(1 == result && "Wrong result from loaded function");

      test_func = (int(*)()) c_lib.findSymbol("bogusFunc");
      CPPUNIT_ASSERT(NULL == test_func && "Found non-existent symbol");

      try
      {
         c_lib.unload();
      }
      catch (vpr::Exception&)
      {
         CPPUNIT_ASSERT(false && "C module unload failed");
      }
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false && "C module load failed");
   }
}

void LibraryTest::loadCxxSymbolTest()
{
   try
   {
      void* (*creator)();

      vpr::Library cxx_lib(mCxxModuleName);
      cxx_lib.load();

      // No, *this* is the weirdest cast I have ever written.
      creator = (void*(*)()) cxx_lib.findSymbol("entryFunc");
      CPPUNIT_ASSERT(NULL != creator && "Entry point lookup failed");

      void* object = (*creator)();
      CPPUNIT_ASSERT(NULL != object && "Object creation failed");

      // Is there a way to test that this cast was successful?
      TestInterface* test_obj(NULL);
      test_obj = static_cast<TestInterface*>(object);
//      CPPUNIT_ASSERT(NULL != test_obj &&
//                     "Dynamic casting of created object failed");

      CPPUNIT_ASSERT(test_obj->function() == true &&
                     "Unexpected results from TestInterface object");

      try
      {
         cxx_lib.unload();
      }
      catch (vpr::Exception&)
      {
         CPPUNIT_ASSERT(false && "C++ module unload failed");
      }
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false && "C++ module load failed");
   }
}

}
