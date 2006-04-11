#ifndef _ENUMERATIONS_TEST_H
#define _ENUMERATIONS_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkFactory.h>

/*****************************************************************
 tests out the functionality expected of enumerations in ConfigChunks
*******************************************************************/

namespace jcclTest
{

class EnumerationsTest : public CppUnit::TestCase
{
public:
   EnumerationsTest() : CppUnit::TestCase ()
   {
   }
    
   EnumerationsTest(std::string name) : CppUnit::TestCase (name)
   {
   }
   
   virtual ~EnumerationsTest()
   {
   }


   void ReadTests () {
      std::string file_path(TESTFILES_PATH);

      jccl::ChunkFactory::instance()->loadDescs (file_path + "EnumerationsTest/enumerations_test.desc");

      jccl::ConfigChunkDB chunkdb;
      chunkdb.load (file_path + "EnumerationsTest/enumerations_test.config");

      jccl::ConfigChunkPtr ch = chunkdb.get("Motion Star master");
      CPPUNIT_ASSERT(ch.get() != NULL);

      int port = ch->getProperty<int>("serverPort");
      CPPUNIT_ASSERT(port == 5000);

      int hemisphere = ch->getProperty<int>("hemisphere");
      CPPUNIT_ASSERT(hemisphere == 0);
      
      int format = ch->getProperty<int>("bformat");
      CPPUNIT_ASSERT(format == 4);
      
    }

        

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("EnumerationsTest");
      test_suite->addTest( new CppUnit::TestCaller<EnumerationsTest>("ReadTests", &EnumerationsTest::ReadTests));
      
      return test_suite;
   }
};

}

#endif
