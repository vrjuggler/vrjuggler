#include <string>

#include <vpr/DynLoad/LibraryFinder.h>
#include <vpr/System.h>
#include <boost/filesystem/exception.hpp>

#include <TestCases/DynLoad/LibraryFinderTest.h>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( LibraryFinderTest );

static const std::string C_MOD("libcmod.so");

LibraryFinderTest::LibraryFinderTest()
   :  mModuleDir(MODULE_DIR)
{
   ;
}

void LibraryFinderTest::setUp()
{
   std::string c_lib_path(MODULE_DIR);
   c_lib_path += "/" + C_MOD;
   mCModuleName = c_lib_path;

   std::string cxx_lib_path(MODULE_DIR);
   cxx_lib_path += "/libcxxmod.so";
   mCxxModuleName = cxx_lib_path;
}

void LibraryFinderTest::scanTest()
{
   vpr::LibraryFinder finder1(mModuleDir, std::string(".so"), false);
   CPPUNIT_ASSERT(finder1.getLibraries().size() == 0 && "Should not have any libraries yet");

   finder1.rescan();
   CPPUNIT_ASSERT(finder1.getLibraries().size() == 2 && "Wrong number of libraries found");

   try
   {
      finder1.setLibraryDirectory(std::string("bogusDir"));
   }
   catch(boost::filesystem::filesystem_error fsException)
   {
      /* This is expected. */ ;
   }

   CPPUNIT_ASSERT(finder1.getLibraries().size() == 0 && "Should not have found any libraries");

   finder1.setLibraryDirectory(mModuleDir);
   CPPUNIT_ASSERT(finder1.getLibraries().size() == 2 && "Wrong number of libraries found");

   finder1.setLibraryExtension(".bad");
   CPPUNIT_ASSERT(finder1.getLibraries().size() == 0 && "Should not have found any libraries");

   finder1.setLibraryExtension(".so");
   CPPUNIT_ASSERT(finder1.getLibraries().size() == 2 && "Wrong number of libraries found");

   try
   {
      finder1.setDirAndExt("bogus", ".bad");
   }
   catch(boost::filesystem::filesystem_error fsException)
   {
      /* This is expected. */ ;
   }

   CPPUNIT_ASSERT(finder1.getLibraries().size() == 0 && "Should not have found any libraries");

   finder1.setDirAndExt(mModuleDir, ".so");
   CPPUNIT_ASSERT(finder1.getLibraries().size() == 2 && "Wrong number of libraries found");

   vpr::LibraryFinder finder2(mModuleDir, std::string(".so"));
   CPPUNIT_ASSERT(finder2.getLibraries().size() == 2 && "Wrong number of libraries found");
}

void LibraryFinderTest::scanAndLoadTest()
{
   vpr::LibraryFinder finder(mModuleDir, ".so");

   vpr::LibraryFinder::LibraryList libs;
   libs = finder.getLibraries();
   CPPUNIT_ASSERT(libs.size() == 2 && "Wrong number of libraries found");

   vpr::ReturnStatus status;
   status = libs[0]->load();
   CPPUNIT_ASSERT(status.success() && "Load failed");
}

}
