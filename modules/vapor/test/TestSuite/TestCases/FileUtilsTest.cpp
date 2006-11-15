#include <TestCases/FileUtilsTest.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>


/*****************************************************************
 tests out the functionality expected of VPR file utility functions
*******************************************************************/

namespace vprTest
{

   CPPUNIT_TEST_SUITE_REGISTRATION( FileUtilsTest );

   void FileUtilsTest::ReplaceEnvVarsTest()
   {
      const std::string env1_name("REP_ENV_VAL1");
      const std::string env1_value("Val1");
      const std::string env2_name("REP_ENV_VAL2");
      const std::string env2_value("Val2");
      std::string result_value;

      // Setup environment
      vpr::System::setenv(env1_name, env1_value);
      vpr::System::setenv(env2_name, env2_value);

      //vpr::System::getenv(env_name, result_value);
      //CPPUNIT_ASSERT(set_value == result_value);

      std::string test_str, expected_str, result_str;

      // Test ${}
      test_str = std::string("/path/${REP_ENV_VAL1}/more_path");
      expected_str = std::string("/path/Val1/more_path");
      result_str = vpr::replaceEnvVars(test_str);
      CPPUNIT_ASSERT(expected_str == result_str);

      test_str = std::string("${REP_ENV_VAL1}/more_path");
      expected_str = std::string("Val1/more_path");
      result_str = vpr::replaceEnvVars(test_str);
      CPPUNIT_ASSERT(expected_str == result_str);

      test_str = std::string("abcdef${REP_ENV_VAL1}");
      expected_str = std::string("abcdefVal1");
      result_str = vpr::replaceEnvVars(test_str);
      CPPUNIT_ASSERT(expected_str == result_str);

      test_str = std::string("abc${REP_ENV_VAL1}def${REP_ENV_VAL1}ghi${REP_ENV_VAL1}");
      expected_str = std::string("abcVal1defVal1ghiVal1");
      result_str = vpr::replaceEnvVars(test_str);
      CPPUNIT_ASSERT(expected_str == result_str);

      // Test $()
      test_str = std::string("/path/$(REP_ENV_VAL1)/more_path");
      expected_str = std::string("/path/Val1/more_path");
      result_str = vpr::replaceEnvVars(test_str);
      CPPUNIT_ASSERT(expected_str == result_str);

      test_str = std::string("$(REP_ENV_VAL1)/more_path");
      expected_str = std::string("Val1/more_path");
      result_str = vpr::replaceEnvVars(test_str);
      CPPUNIT_ASSERT(expected_str == result_str);

      test_str = std::string("abcdef$(REP_ENV_VAL1)");
      expected_str = std::string("abcdefVal1");
      result_str = vpr::replaceEnvVars(test_str);
      CPPUNIT_ASSERT(expected_str == result_str);

      test_str = std::string("abc$(REP_ENV_VAL1)def$(REP_ENV_VAL1)ghi$(REP_ENV_VAL1)");
      expected_str = std::string("abcVal1defVal1ghiVal1");
      result_str = vpr::replaceEnvVars(test_str);
      CPPUNIT_ASSERT(expected_str == result_str);

      // Test ${} and $()
      test_str = std::string("abc$(REP_ENV_VAL1)${REP_ENV_VAL2}ghi${REP_ENV_VAL2}$(REP_ENV_VAL1)");
      expected_str = std::string("abcVal1Val2ghiVal2Val1");
      result_str = vpr::replaceEnvVars(test_str);
      CPPUNIT_ASSERT(expected_str == result_str);

   }

}
