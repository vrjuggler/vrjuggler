#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <vpr/IO/Socket/NoPushWriter.h>
#include <vpr/IO/Socket/SimpleAllocationStrategies.h>

#include <TestCases/IO/Socket/NoPushWriterTest.h>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(NoPushWriterTest);

void NoPushWriterTest::testWrite()
{
   const std::string expected("The quick brown fox jumped over the lazy dog");
   vpr::NoPushWriter w(vpr::minimalAllocationStrategy);
   const vpr::Uint32 expected_size(expected.length());
   const vpr::Uint32 written = w.write(&expected[0], expected.length());
   CPPUNIT_ASSERT_EQUAL(expected_size, written);

   CPPUNIT_ASSERT(w.getBuffer() != NULL);

   CPPUNIT_ASSERT_EQUAL(expected.size(), w.getBufferUse());
   CPPUNIT_ASSERT_EQUAL(expected.size(), w.getBufferSize());

   const std::string buffer(reinterpret_cast<const char*>(w.getBuffer()),
                            w.getBufferUse());
   CPPUNIT_ASSERT(! buffer.empty());
   CPPUNIT_ASSERT_EQUAL(expected.size(), buffer.size());
   CPPUNIT_ASSERT_EQUAL(expected, buffer);
}

void NoPushWriterTest::testMinimalAlloc()
{
   testWriter(vpr::minimalAllocationStrategy);
}

void NoPushWriterTest::testDoublingAlloc()
{
   testWriter(vpr::minimalAllocationStrategy);
}

void NoPushWriterTest::testWriter(const vpr::NoPushAllocStrategy& strategy)
{
   const std::string expected("The quick brown fox jumped over the lazy dog");
   vpr::NoPushWriter w(strategy);

   // Break up the input string into its component words.
   std::vector<std::string> split_string;
   boost::split(split_string, expected, boost::is_any_of(" "));

   std::string cur_string;
   size_t cur_size(0);

   // Rebuild the string in the no-push buffer.
   typedef std::vector<std::string>::iterator iter_type;
   for ( iter_type i = split_string.begin(); i != split_string.end(); ++i )
   {
      // Store this so that we can pass it into the strategy along with
      // written to determine how big the buffer should be after the write.
      const size_t cur_buffer_size(w.getBufferSize());

      const vpr::Uint32 expected_len((*i).size());
      const vpr::Uint32 written = w.write(&(*i)[0], (*i).size());
      CPPUNIT_ASSERT_EQUAL(expected_len, written);

      cur_string += *i;
      cur_size   += written;

      // Use the allocation strategy to figure out how big the allocation
      // buffer ought to be at this point.
      const size_t expected_size = strategy(cur_buffer_size, written);

      CPPUNIT_ASSERT_EQUAL(cur_size, w.getBufferUse());
      CPPUNIT_ASSERT_EQUAL(expected_size, w.getBufferSize());

      const std::string written_str(
         reinterpret_cast<const char*>(w.getBuffer()), w.getBufferUse()
      );
      CPPUNIT_ASSERT_EQUAL(cur_string, written_str);

      // Append a space to the buffer as part of the string reconstruction as
      // long as we are not on the last word in split_string.
      if ( i + 1 != split_string.end() )
      {
         w.write(" ", 1);

         cur_string += " ";
         cur_size   += 1;
      }
   }

   CPPUNIT_ASSERT_EQUAL(expected.size(), w.getBufferUse());

   const std::string result(reinterpret_cast<const char*>(w.getBuffer()),
                            w.getBufferUse());
   CPPUNIT_ASSERT_EQUAL(expected.size(), result.size());
   CPPUNIT_ASSERT_EQUAL(expected, result);
}

}
