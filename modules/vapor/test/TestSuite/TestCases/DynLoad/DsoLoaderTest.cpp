#include <fstream>
#include <string>

#include <vpr/vpr.h>
#include <vpr/DynLoad/DsoLoader.h>
#include <vpr/System.h>
#include <TestCases/DynLoad/modules/TestInterface.h>

#include <TestCases/DynLoad/DsoLoaderTest.h>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(DsoLoaderTest);

#if defined(VPR_OS_Win32)
static const std::string MODULE1("dsomod1.dll");
static const std::string MODULE2("dsomod2.dll");
#elif defined(VPR_OS_Darwin)
static const std::string MODULE1("libdsomod1.dylib");
static const std::string MODULE2("libdsomod2.dylib");
#else
static const std::string MODULE1("libdsomod1.so");
static const std::string MODULE2("libdsomod2.so");
#endif

static const std::string INIT_FUNC_NAME("function");

void DsoLoaderTest::setUp()
{
   const std::string base_path(MODULE_DIR);

   std::string mod1_path = base_path + "/" + MODULE1;
   mModuleName[0] = mod1_path;

   std::string mod2_path = base_path + "/" + MODULE2;
   mModuleName[1] = mod2_path;
}

void DsoLoaderTest::loadingConstructorTest()
{
   // Test for library existence.  This could be done better.
   std::ifstream mod_file;
   mod_file.open(mModuleName[0].c_str());
   CPPUNIT_ASSERT(mod_file.good() && "Module does not exist");
   mod_file.close();

   try
   {
      vpr::DsoLoader::CreatorArg arg("loadingConstructorTest", NULL);
      vpr::DsoLoader loader(INIT_FUNC_NAME, mModuleName[0], arg);
      CPPUNIT_ASSERT(true);
   }
   catch(vpr::LoaderError& loaderEx)
   {
      CPPUNIT_ASSERT(false);
   }
}

void DsoLoaderTest::loadTest()
{
   vpr::DsoLoader loader(INIT_FUNC_NAME);

   try
   {
      vpr::DsoLoader::CreatorArg arg("loadTest", NULL);
      loader.load(mModuleName[0], arg);
      CPPUNIT_ASSERT(true);
   }
   catch(vpr::LoaderError& loaderEx)
   {
      CPPUNIT_ASSERT(false);
   }
}

void DsoLoaderTest::setInitAndLoadTest()
{
   vpr::DsoLoader loader;
   loader.setInitFuncName(INIT_FUNC_NAME);

   try
   {
      vpr::DsoLoader::CreatorArg arg("setInitAndLoadTest", NULL);
      loader.load(mModuleName[0], arg);
      CPPUNIT_ASSERT(true);
   }
   catch(vpr::LoaderError& loaderEx)
   {
      CPPUNIT_ASSERT(false);
   }
}

void DsoLoaderTest::multiLoadTest()
{
   vpr::DsoLoader loader(INIT_FUNC_NAME);

   std::vector<std::string> mods(2);
   mods[0] = mModuleName[0];
   mods[1] = mModuleName[1];

   try
   {
      vpr::DsoLoader::CreatorArg arg("multiLoadTest", NULL);
      loader.load(mods, arg);
      CPPUNIT_ASSERT(loader.getDsoCount() == mods.size());
   }
   catch(vpr::LoaderError& loaderEx)
   {
      CPPUNIT_ASSERT(false);
   }
}

void DsoLoaderTest::loadFailureTest()
{
   vpr::DsoLoader loader(INIT_FUNC_NAME);

   try
   {
      vpr::DsoLoader::CreatorArg arg("loadFailureTest", NULL);
      loader.load("bogus.so", arg);
      CPPUNIT_ASSERT(false);
   }
   catch(vpr::LoaderError& loaderEx)
   {
      CPPUNIT_ASSERT(loaderEx.error() == vpr::LoaderError::FILE_LOAD_ERROR);
   }
}

void DsoLoaderTest::lookupFailureTest()
{
   vpr::DsoLoader loader("bogusFunc");

   try
   {
      vpr::DsoLoader::CreatorArg arg("lookupFailureTest", NULL);
      loader.load(mModuleName[0], arg);
      CPPUNIT_ASSERT(false);
   }
   catch(vpr::LoaderError& loaderEx)
   {
      CPPUNIT_ASSERT(loaderEx.error() == vpr::LoaderError::SYM_LOOKUP_ERROR);
   }
}

}
