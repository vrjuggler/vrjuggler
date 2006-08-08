#include <fstream>
#include <string>
#include <boost/filesystem/path.hpp>

#include <vpr/vpr.h>
#include <vpr/DynLoad/Library.h>
#include <vpr/DynLoad/LibraryLoader.h>
#include <vpr/DynLoad/LibraryException.h>
#include <vpr/DynLoad/SymbolLookupException.h>
#include <vpr/System.h>
#include <vpr/Util/Debug.h>
#include <TestCases/DynLoad/modules/TestInterface.h>

#include <TestCases/DynLoad/LibraryLoaderTest.h>


namespace fs = boost::filesystem;

namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(LibraryLoaderTest);

#if defined(VPR_OS_Windows)
static const std::string MODULE1("loadermod1.dll");
static const std::string MODULE2("loadermod2.dll");
#elif defined(VPR_OS_Darwin)
static const std::string MODULE1("loadermod1.dylib");
static const std::string MODULE2("loadermod2.dylib");
#else
static const std::string MODULE1("loadermod1.so");
static const std::string MODULE2("loadermod2.so");
#endif

#if defined(VPR_OS_Windows)
static const std::string PATH_SEP("\\");
#else
static const std::string PATH_SEP("/");
#endif

void LibraryLoaderTest::setUp()
{
   const std::string base_path(MODULE_DIR);

   std::string mod1_path = base_path + PATH_SEP + MODULE1;
   mModules[0].first     = mod1_path;
   mModules[0].second    = "sumFunc";

   std::string mod2_path = base_path + PATH_SEP + MODULE2;
   mModules[1].first     = mod2_path;
   mModules[1].second    = "otherFunc";
}

static bool rawCallback(void* creator)
{
   int (*init_func)(int, int);
   init_func = (int (*)(int, int)) creator;

   int sum = (*init_func)(4, 5);
   CPPUNIT_ASSERT(sum == 9);

   return true;
}

class LoaderFunctor
{
public:
   bool operator()(void* arg)
   {
      void (*init_func)(const char*);
      init_func = (void (*)(const char*)) arg;
      init_func("Hello");
      return true;
   }
};

void LibraryLoaderTest::callEntryPointTestRaw()
{
   vpr::LibraryPtr dso(new vpr::Library(mModules[0].first));
   try
   {
      vpr::LibraryLoader::callEntryPoint(dso, mModules[0].second,
                                         (bool(*)(void*)) rawCallback);
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false);
   }
}

void LibraryLoaderTest::callEntryPointTestFunctor()
{
   LoaderFunctor functor;
   vpr::LibraryPtr dso(new vpr::Library(mModules[1].first));

   try
   {
      vpr::LibraryLoader::callEntryPoint(dso, mModules[1].second, functor);
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false);
   }
}

void LibraryLoaderTest::findDSOTest()
{
   vpr::LibraryPtr dso1, dso2;
   std::vector<std::string> path1(1);

   path1[0] = std::string(MODULE_DIR);
   dso1 = vpr::LibraryLoader::findDSO("loadermod1", path1);
   CPPUNIT_ASSERT(dso1.get() != NULL);

   std::vector<fs::path> path2(1);
   path2[0] = fs::path(std::string(MODULE_DIR), fs::native);
   dso2 = vpr::LibraryLoader::findDSO("loadermod1", path2);
   CPPUNIT_ASSERT(dso2.get() != NULL);
}

void LibraryLoaderTest::findDSOAndCallEntryPointTest()
{
   try
   {
      std::vector<std::string> path1(1, std::string(MODULE_DIR));

      vpr::LibraryPtr dso;
      vpr::LibraryLoader::findDSOAndCallEntryPoint("loadermod1", path1,
                                                   mModules[0].second,
                                                   (bool(*)(void*)) rawCallback,
                                                   dso);
      CPPUNIT_ASSERT(dso.get() != NULL);
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false);
   }

   try
   {
      std::vector<fs::path> path2(1, fs::path(std::string(MODULE_DIR), fs::native));

      vpr::LibraryPtr dso;
      vpr::LibraryLoader::findDSOAndCallEntryPoint("loadermod1", path2,
                                                   mModules[0].second,
                                                   (bool(*)(void*)) rawCallback,
                                                   dso);
      CPPUNIT_ASSERT(dso.get() != NULL);
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false);
   }
}

void LibraryLoaderTest::multiLoadTest()
{
   try
   {
      vpr::LibraryPtr dso(new vpr::Library(mModules[0].first));
      vpr::LibraryLoader::callEntryPoint(dso, mModules[0].second,
                                         (bool(*)(void*)) rawCallback);
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false);
   }

   try
   {
      LoaderFunctor functor;
      vpr::LibraryPtr dso(new vpr::Library(mModules[1].first));
      vpr::LibraryLoader::callEntryPoint(dso, mModules[1].second, functor);
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false);
   }
}

void LibraryLoaderTest::loadFailureTest()
{
   vpr::Debug::instance()->disableOutput();
   try
   {
      vpr::LibraryPtr dso(new vpr::Library("loadFailureTest"));
      vpr::LibraryLoader::callEntryPoint(dso, "notThere",
                                         (bool(*)(void*)) rawCallback);
      // Execution should not reach this point.
      CPPUNIT_ASSERT(false);
   }
   // This is the exception type that we are expecting.
   catch (vpr::IOException&)
   {
      CPPUNIT_ASSERT(true);
   }
   catch (vpr::SymbolLookupException&)
   {
      CPPUNIT_ASSERT(false);
   }
   catch (vpr::LibraryException&)
   {
      CPPUNIT_ASSERT(false);
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false);
   }
   vpr::Debug::instance()->enableOutput();
}

void LibraryLoaderTest::lookupFailureTest()
{
   vpr::Debug::instance()->disableOutput();
   try
   {
      vpr::LibraryPtr dso(new vpr::Library(mModules[0].first));
      vpr::LibraryLoader::callEntryPoint(dso, "lookupFailureTest",
                                         (bool(*)(void*)) rawCallback);

      // Execution should not reach this point.
      CPPUNIT_ASSERT(false);
   }
   // This is the exception type that we are expecting.
   catch (vpr::SymbolLookupException&)
   {
      CPPUNIT_ASSERT(true);
   }
   catch (vpr::LibraryException&)
   {
      CPPUNIT_ASSERT(false);
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false);
   }
   vpr::Debug::instance()->enableOutput();
}

void LibraryLoaderTest::findFailureTest()
{
   std::vector<std::string> path(0);
   vpr::LibraryPtr dso1;

   CPPUNIT_ASSERT(dso1.get() == NULL);
   vpr::Debug::instance()->disableOutput();
      dso1 = vpr::LibraryLoader::findDSO("findFailureTest", path);
   vpr::Debug::instance()->enableOutput();
   CPPUNIT_ASSERT(dso1.get() == NULL);

   vpr::LibraryPtr dso2;
   CPPUNIT_ASSERT(dso2.get() == NULL);
   vpr::Debug::instance()->disableOutput();
   try
   {
      vpr::LibraryLoader::findDSOAndCallEntryPoint("findFailureTest",
                                                   path, "notThere",
                                                   (bool(*)(void*)) rawCallback,
                                                   dso2);

      // Execution should not reach this point.
      CPPUNIT_ASSERT(false);
   }
   // This is the exception type that we are expecting.
   catch (vpr::IOException&)
   {
      CPPUNIT_ASSERT(true);
   }
   catch (vpr::SymbolLookupException&)
   {
      CPPUNIT_ASSERT(false);
   }
   catch (vpr::LibraryException&)
   {
      CPPUNIT_ASSERT(false);
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false);
   }
   vpr::Debug::instance()->enableOutput();
   CPPUNIT_ASSERT(dso2.get() == NULL);
}

}
