#include <fstream>
#include <string>

#include <vpr/DynLoad/Library.h>
#include <vpr/System.h>
#include <TestCases/DynLoad/modules/TestInterface.h>

#include <TestCases/DynLoad/LibraryTest.h>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( LibraryTest );

static const std::string C_MOD("libcmod.so");

void LibraryTest::setUp()
{
   std::string c_lib_path(MODULE_DIR);
   c_lib_path += "/" + C_MOD;
   mCModuleName = c_lib_path;

   std::string cxx_lib_path(MODULE_DIR);
   cxx_lib_path += "/libcxxmod.so";
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

   vpr::ReturnStatus status;
   vpr::Library c_lib(mCModuleName);
   status = c_lib.load();
   CPPUNIT_ASSERT(status.success() && "Load failed");
}

void LibraryTest::unloadTest()
{
   // Test for library existence.  This could be done better.
   std::ifstream mod_file;
   mod_file.open(mCModuleName.c_str());
   CPPUNIT_ASSERT(mod_file.good() && "Module does not exist");
   mod_file.close();

   vpr::ReturnStatus status;
   vpr::Library c_lib(mCModuleName);
   status = c_lib.load();
   CPPUNIT_ASSERT(status.success() && "Load failed");

   status = c_lib.unload();
   CPPUNIT_ASSERT(status.success() && "Unload failed");
}

void LibraryTest::libraryPathTest()
{
   vpr::ReturnStatus status;
   vpr::Library lib(C_MOD);

   status = vpr::System::setenv(std::string("LD_LIBRARY_PATH"),
                                std::string(MODULE_DIR));
   CPPUNIT_ASSERT(status.success() && "Failed to extend library path");

   status = lib.load();
   CPPUNIT_ASSERT(status.success() && "Library load failed");
}

void LibraryTest::loadCSymbolTest()
{
   vpr::Library c_lib(mCModuleName);
   int (*test_func)();

   vpr::ReturnStatus status;
   status = c_lib.load();
   CPPUNIT_ASSERT(status.success() && "C module load failed");

   // This is the weirdest cast I have ever written.
   test_func = (int(*)()) c_lib.findSymbol("function");
   CPPUNIT_ASSERT(NULL != test_func && "Symbol lookup failed");

   int result = (*test_func)();
   CPPUNIT_ASSERT(1 == result && "Wrong result from loaded function");

   test_func = (int(*)()) c_lib.findSymbol("bogusFunc");
   CPPUNIT_ASSERT(NULL == test_func && "Found non-existent symbol");

   status = c_lib.unload();
   CPPUNIT_ASSERT(status.success() && "C module unload failed");
}

void LibraryTest::loadCxxSymbolTest()
{
   vpr::Library cxx_lib(mCxxModuleName);
   void* (*creator)();
   TestInterface* test_obj(NULL);

   vpr::ReturnStatus status;
   status = cxx_lib.load();
   CPPUNIT_ASSERT(status.success() && "C++ module load failed");

   // No, *this* is the weirdest cast I have ever written.
   creator = (void*(*)()) cxx_lib.findSymbol("entryFunc");
   CPPUNIT_ASSERT(NULL != creator && "Entry point lookup failed");

   void* object = (*creator)();
   CPPUNIT_ASSERT(NULL != object && "Object creation failed");

   // Is there a way to test that this cast was successful?
   test_obj = static_cast<TestInterface*>(object);
//   CPPUNIT_ASSERT(NULL != test_obj && "Dynamic casting of created object failed");

   CPPUNIT_ASSERT(test_obj->function() == true && "Unexpected results from TestInterface object");

   status = cxx_lib.unload();
   CPPUNIT_ASSERT(status.success() && "C++ module unload failed");
}

}
