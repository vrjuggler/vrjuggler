#include <fstream>
#include <string>
#include <boost/filesystem/path.hpp>

#include <vpr/vpr.h>
#include <vpr/DynLoad/Library.h>
#include <vpr/DynLoad/LibraryLoader.h>
#include <vpr/System.h>
#include <vpr/Util/Debug.h>
#include <TestCases/DynLoad/modules/TestInterface.h>

#include <TestCases/DynLoad/LibraryLoaderTest.h>


namespace fs = boost::filesystem;

namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(LibraryLoaderTest);

#if defined(VPR_OS_Win32)
static const std::string MODULE1("loadermod1.dll");
static const std::string MODULE2("loadermod2.dll");
#elif defined(VPR_OS_Darwin)
static const std::string MODULE1("loadermod1.dylib");
static const std::string MODULE2("loadermod2.dylib");
#else
static const std::string MODULE1("loadermod1.so");
static const std::string MODULE2("loadermod2.so");
#endif

#if defined(VPR_OS_Win32)
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

void LibraryLoaderTest::loadAndInitTestRaw()
{
   vpr::LibraryLoader loader;
   vpr::ReturnStatus status;

   vpr::LibraryPtr dso(new vpr::Library(mModules[0].first));
   status = loader.loadAndInitDSO(dso, mModules[0].second,
                                  (bool(*)(void*)) rawCallback);
   CPPUNIT_ASSERT(status.success());
}

void LibraryLoaderTest::loadAndInitTestFunctor()
{
   vpr::LibraryLoader loader;
   vpr::ReturnStatus status;

   LoaderFunctor functor;
   vpr::LibraryPtr dso(new vpr::Library(mModules[1].first));
   status = loader.loadAndInitDSO(dso, mModules[1].second, functor);
   CPPUNIT_ASSERT(status.success());
}

void LibraryLoaderTest::findAndInitTest()
{
   vpr::LibraryLoader loader1, loader2;
   vpr::ReturnStatus status;
   std::vector<std::string> path1(1);

   path1[0] = std::string(MODULE_DIR);
   status = loader1.findAndInitDSO("loadermod1", path1, mModules[0].second,
                                   (bool(*)(void*)) rawCallback);
   CPPUNIT_ASSERT(status.success());

   std::vector<fs::path> path2(1);
   path2[0] = fs::path(std::string(MODULE_DIR), fs::native);
   status = loader2.findAndInitDSO("loadermod1", path2, mModules[0].second,
                                   (bool(*)(void*)) rawCallback);
   CPPUNIT_ASSERT(status.success());
}

void LibraryLoaderTest::multiLoadTest()
{
   vpr::LibraryLoader loader;
   vpr::ReturnStatus status;

   vpr::LibraryPtr dso1(new vpr::Library(mModules[0].first));
   status = loader.loadAndInitDSO(dso1, mModules[0].second,
                                  (bool(*)(void*)) rawCallback);
   CPPUNIT_ASSERT(status.success());

   LoaderFunctor functor;
   vpr::LibraryPtr dso2(new vpr::Library(mModules[1].first));
   status = loader.loadAndInitDSO(dso2, mModules[1].second, functor);
   CPPUNIT_ASSERT(status.success());
}

void LibraryLoaderTest::loadFailureTest()
{
   vpr::LibraryLoader loader;
   vpr::ReturnStatus status;

   vpr::Debug::instance()->disableOutput();
      vpr::LibraryPtr dso(new vpr::Library("loadFailureTest"));
      status = loader.loadAndInitDSO(dso, "notThere",
                                     (bool(*)(void*)) rawCallback);
      CPPUNIT_ASSERT(status.failure());
   vpr::Debug::instance()->enableOutput();
}

void LibraryLoaderTest::lookupFailureTest()
{
   vpr::LibraryLoader loader;
   vpr::ReturnStatus status;

   vpr::Debug::instance()->disableOutput();
      vpr::LibraryPtr dso1(new vpr::Library(mModules[0].first));
      status = loader.loadAndInitDSO(dso1, "lookupFailureTest",
                                     (bool(*)(void*)) rawCallback);
      CPPUNIT_ASSERT(status.failure());
   vpr::Debug::instance()->enableOutput();
}

void LibraryLoaderTest::findFailureTest()
{
   vpr::LibraryLoader loader;
   vpr::ReturnStatus status;
   std::vector<std::string> path(0);

   vpr::Debug::instance()->disableOutput();
      status = loader.findAndInitDSO("findFailureTest", path, "notThere",
                                     (bool(*)(void*)) rawCallback);
      CPPUNIT_ASSERT(status.failure());
   vpr::Debug::instance()->enableOutput();
}

}
