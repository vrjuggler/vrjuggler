#ifndef _VRJTEST_PLEXUS_TEST_H_
#define _VRJTEST_PLEXUS_TEST_H_

namespace CppUnit
{
   class TestSuite;
}

namespace vrjTest
{

class JugglerTest
{
public:
   virtual void registerTests(CppUnit::TestSuite* sutie) = 0;
};

}

#endif
