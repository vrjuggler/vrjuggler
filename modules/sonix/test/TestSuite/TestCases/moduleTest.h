
//#include <assert.h>
#include <syn/AudioFormat.h>
#include <syn/Terminal.h>
#include <syn/OutputTerminal.h>
#include <syn/InputTerminal.h>
#include <syn/Module.h>
#include <syn/WavSourceModule.h>
#include <syn/PassthroughModule.h>
#include <syn/MixerModule.h>
#include <syn/SplitterModule.h>
#include <syn/OpenALOutputModule.h>

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace synTest
{

   class moduleTest : public CppUnit::TestCase
   {
   public:
      moduleTest() 
      {
      }

      moduleTest( std::string name ) : CppUnit::TestCase( name )
      {
      }

      virtual ~moduleTest()
      {
      }

      void testme()
      {
         syn::AudioFormat fmt;
         fmt.samplingRateHz = 44100;
         fmt.channels = 2;
         fmt.datatype = syn::AudioFormat::SIGNED16;

         // make sure passthrough can run unconnected
         {
            syn::PassthroughModule mod;
            mod.open( fmt );
            for (int x = 0; x < 1000; ++x)
               mod.update();
            mod.close();
         }

         // make sure WavSourceModule can run unconnected
         {
            syn::WavSourceModule mod;
            mod.open( fmt );
            for (int x = 0; x < 1000; ++x)
               mod.update();
            mod.close();
         }

         // make sure MixerModule can run unconnected
         {
            syn::MixerModule mod;
            mod.open( fmt );
            for (int x = 0; x < 1000; ++x)
               mod.update();
            mod.close();
         }

         // make sure SplitterModule can run unconnected
         {
            syn::SplitterModule mod;
            mod.open( fmt );
            for (int x = 0; x < 1000; ++x)
               mod.update();
            mod.close();
         }

         // make sure OpenALOutputModule can run unconnected
         {
            syn::OpenALOutputModule mod;
            mod.open( fmt );
            for (int x = 0; x < 1000; ++x)
               mod.update();
            mod.close();
         }

         // TODO add connectivity tests, examine the buffers, etc...
      }


      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("module tests");
         test_suite->addTest( new CppUnit::TestCaller<moduleTest>("module_test", &moduleTest::testme));

         return test_suite;
      }

   protected:

   };

};
