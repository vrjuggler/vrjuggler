#ifndef CPPUNIT_EXTENSIONS_METRICREGISTRY_H_
#define CPPUNIT_EXTENSIONS_METRICREGISTRY_H_

#ifdef _WIN32
#  include <windows.h>
#endif

#include <cppunit/TestAssert.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>

#include <map>


namespace CppUnit
{

/**
 * This singleton class is used to register and check the use of numeric "metrics"
 * that can be used to test the performance of code
 *
 *
 * Example:
 *
 * CppUnit::MetricRegistry* metric_reg = CppUnit::MetricRegistry::instance();
 * std::string metric_prefix;    // Prefix for all metric labels (mode/hostname)
 *
 * std::string host_name = vpr::System::getHostname();
 * metric_prefix = host_name + "/";
 *
 * metric_reg->setPrefix(metric_prefix);
 * metric_reg->setFilename("vapor_metrics.txt");
 * metric_reg->setMetric("Main/MetricTest", 1221.75f);
 */
class MetricRegistry
{
public:
   MetricRegistry()
     : mFilename(""), mPrefix("")
   {;}
   ~MetricRegistry()
   {;}

   static MetricRegistry* instance()
   {
      static MetricRegistry* mInstance(NULL);

      if(mInstance == NULL)
      { mInstance = new MetricRegistry; }
      return mInstance;
   }

public:
   void setFilename(std::string filename)
   {
      mFilename = filename;
      readFile();
   }

   /** Set a prefix to be used on all labels */
   void setPrefix(std::string prefix)
   { mPrefix = prefix; }

   /** Get the metric for the given value.
    * returns 0 if value is not found
    */
   double getMetric(std::string name)
   {
      std::string key = mPrefix + name;
      MetricMapType::iterator i = mMetricMap.find(key);
      if(i != mMetricMap.end())
         return (*i).second;
      else
         return 0.0;
   }

   void setMetric(std::string name, double val)
   {
      std::string key = mPrefix + name;
      mMetricMap[key] = val;
      writeFile();
   }

   /** Assert that we are greater then or equal to the cur_value
   * @param test_key  String identifier of the current test
   * @param cur_value The currently observed value
   * @param soft_limit  Allowable limit that will just print a warning
   * @param hard_limit  Limit that will trigger an assertion
   */
   void assertMetricGE(std::string test_key, double cur_value, double soft_limit, double hard_limit)
   {
      CPPUNIT_ASSERT((soft_limit < hard_limit) && "The soft limit percentage should be less then hard_limit");

      double old_sample = getMetric(test_key);

      if(old_sample == 0.0f)
      {
         std::cout << "New metric for [" << test_key << "]  value:" << cur_value << std::endl;
         setMetric(test_key, cur_value);
      }
      else
      {
         //  hard_limit < softLimit < old_sample
         double soft_limit_value = (old_sample * (1.0 - soft_limit));
         double hard_limit_value = (old_sample * (1.0 - hard_limit));

         if(!(cur_value >= hard_limit_value))
         {
            std::cout << "HardLimit [" << hard_limit_value << "] failed on metric: "
                      << test_key << " cur value:" << cur_value << "  good value:" << old_sample
                      << "  difference of:" << 100.0f*(cur_value-old_sample)/old_sample << " percent" << std::endl;
            CPPUNIT_ASSERT(false);
         }
         else if(!(cur_value >= soft_limit_value))
         {
            std::cout << "SoftLimit [" << soft_limit_value << "] failed on metric: "
                      << test_key << " cur value:" << cur_value << "  good value:" << old_sample
                      << "  difference of:" << 100.0f*(cur_value-old_sample)/old_sample << " percent" << std::endl;
         }
         else if(cur_value > old_sample)
         {
            // Only note new one if we have a "significant" version
            float percent_diff = static_cast<float>((cur_value-old_sample)/old_sample);
            if(percent_diff > soft_limit)
            {
               std::cout << "New metric for [" << test_key << "]  new value:" << cur_value << "  old value:" << old_sample
                         << " --- increase of [" << 100.0f*percent_diff << "] percent.  (better then soft_limit)" << std::endl;
               setMetric(test_key, cur_value);
            }
         }
      }
   }

   /** Assert that we are less then or equal to the cur_value
   * @param test_key  String identifier of the current test
   * @param cur_value The currently observed value
   * @param soft_limit  Allowable limit that will just print a warning
   * @param hard_limit  Limit that will trigger an assertion
   */
   void assertMetricLE(std::string test_key, double cur_value, double soft_limit, double hard_limit)
   {
      CPPUNIT_ASSERT((soft_limit < hard_limit) && "The soft limit percentage should be less then hard_limit");

      double old_sample = getMetric(test_key);

      if(old_sample == 0.0f)
      {
         std::cout << "New metric for [" << test_key << "]  value:" << cur_value << std::endl;
         setMetric(test_key, cur_value);
      }
      else
      {
         //  old_sample <  soft_limit < hard_limit
         double soft_limit_value = (old_sample * (1.0 + soft_limit));
         double hard_limit_value = (old_sample * (1.0 + hard_limit));

         if(!(cur_value <= hard_limit_value))
         {
            std::cout << "HardLimit [" << hard_limit_value << "] failed on metric: "
                      << test_key << " cur value:" << cur_value << "  good value:" << old_sample
                      << "  difference of:" << 100.0f*(old_sample-cur_value)/old_sample << " percent" << std::endl;
            CPPUNIT_ASSERT(false);
         }
         else if(!(cur_value <= soft_limit_value))
         {
            std::cout << "SoftLimit [" << soft_limit_value << "] failed on metric: "
                      << test_key << " cur value:" << cur_value << "  good value:" << old_sample
                      << "  difference of:" << 100.0f*(old_sample-cur_value)/old_sample << " percent" << std::endl;
         }
         else if(cur_value < old_sample)
         {
            // Only note new one if we have a "significant" version
            float percent_diff = static_cast<float>((old_sample-cur_value)/old_sample);
            if(percent_diff > soft_limit)
            {
               std::cout << "Changed metric for [" << test_key << "]  new value:" << cur_value
                         << " --- decrease of [" << 100.0f*percent_diff << "] percent. (better then soft_limit)" << std::endl;
               setMetric(test_key, cur_value);
            }
         }
      }
   }


protected:
   bool readFile()
   {
      std::ifstream input_file(mFilename.c_str());
      if(!input_file.is_open())
      {
         std::cerr << "Can't open metric file: " << mFilename << std::endl;
         return false;
      }

      while(input_file)
      {
         std::string key;
         double      value;

         input_file >> key >> value;
         //input_file >> value;

         if(input_file)
         {
            mMetricMap[key] = value;

            //std::cout << "[r] metric: " << key << " -- " << value << std::endl;
         }

         while( isspace(input_file.peek()) && (input_file))
            input_file.ignore(1);
      }

      input_file.close();
      return true;
   }

   bool writeFile()
   {
      std::ofstream output_file(mFilename.c_str());
      if(!output_file.is_open())
      {
         std::cerr << "Can't open metric file: " << mFilename << std::endl;
         return false;
      }

      MetricMapType::iterator i( mMetricMap.begin() );
      for(; i!=mMetricMap.end(); ++i)
      {
         int num_spaces = 65 - static_cast<int>((*i).first.length());
         output_file << (*i).first;

         for(;num_spaces >= 0; num_spaces--)
            output_file << " ";

         output_file << " " << (*i).second << "\n";   // note: make sure to put in at least one space

         //std::cout << "[w] metric: " << (*i).first << " -- " << (*i).second << std::endl;
      }

      output_file.close();
      return true;
   }

public:
#ifndef _WIN32
   typedef unsigned long long TimeStamp;  /**< Type for timestamp operations */
#else
   typedef unsigned __int64 TimeStamp;  /**< Type for timestamp operations */
#endif

   /** Return the current time value
   * @returns Microsecond timestamp
   */
   TimeStamp getCurTime();

private:
   std::string mFilename;
   bool        mHaveOpenedFile;

   std::string mPrefix;

   typedef std::map<std::string, double> MetricMapType;
   MetricMapType mMetricMap;
};

}; // namespace

/** Assert that we are greater then or equal to the cur_value
* @param test_key  String identifier of the current test
* @param cur_value The currently observed value
* @param soft_limit  Allowable limit that will just print a warning
* @param hard_limit  Limit that will trigger an assertion
*/
#define CPPUNIT_ASSERT_METRIC_GE(test_key, cur_value, soft_limit, hard_limit)\
         (CppUnit::MetricRegistry::instance()->assertMetricGE(test_key, cur_value, soft_limit, hard_limit))

/** Assert that we are less then or equal to the cur_value
* @param test_key  String identifier of the current test
* @param cur_value The currently observed value
* @param soft_limit  Allowable limit that will just print a warning
* @param hard_limit  Limit that will trigger an assertion
*/
#define CPPUNIT_ASSERT_METRIC_LE(test_key, cur_value, soft_limit, hard_limit)\
         (CppUnit::MetricRegistry::instance()->assertMetricLE(test_key, cur_value, soft_limit, hard_limit))


/**
* @example
* CPPUNIT_METRIC_START_TIMING();
*
* doSomething()
* CPPUNIT_METRIC_STOP_TIMING();
* CPPUNIT_ASSERT_METRIC_TIMING_LE("test/testname", iters, 0.05f, 0.1f);
*/

/** clause to start the CPPUNIT_METRIC timing
* The "{" is to hide the vars in a local scope so that multiple tests can be done in a single shared scope
*/
#define CPPUNIT_METRIC_START_TIMING() { CppUnit::MetricRegistry::TimeStamp mr_start_time, mr_stop_time, mr_diff_time;\
                                       mr_start_time = CppUnit::MetricRegistry::instance()->getCurTime();

/** clause to stop the CPPUNIT_METRIC timing */
#define CPPUNIT_METRIC_STOP_TIMING() mr_stop_time = CppUnit::MetricRegistry::instance()->getCurTime();\
                                     mr_diff_time = mr_stop_time - mr_start_time;

#define CPPUNIT_ASSERT_METRIC_TIMING_LE(test_key, num_iters, soft_limit, hard_limit)\
                                        double mr_per_iter = double(mr_diff_time) / double(num_iters);\
                                        CPPUNIT_ASSERT_METRIC_LE(test_key, mr_per_iter, soft_limit, hard_limit); }

#define CPPUNIT_ASSERT_METRIC_TIMING_GE(test_key, num_iters, soft_limit, hard_limit)\
                                        double mr_per_iter = double(mr_diff_time) / double(num_iters);\
                                        CPPUNIT_ASSERT_METRIC_GE(test_key, mr_per_iter, soft_limit, hard_limit); }

// ------- //
#ifdef _WIN32
#else
#  include<sys/time.h>
#endif

namespace CppUnit
{

// System specific implementation
// returns microseconds
inline CppUnit::MetricRegistry::TimeStamp MetricRegistry::getCurTime()
{
   TimeStamp ret_val(0);

#if defined(_WIN32)
   LARGE_INTEGER count;
   int _nt_bitShift  = 0;
   __int32 _nt_highMask = 0;

   // XXX: Implement this
   /* Sadly; nspr requires the interval to range from 1000 ticks per second
    * to only 100000 ticks per second; QueryPerformanceCounter is too high
    * resolution...
    */
   if (QueryPerformanceCounter(&count))
   {
      __int32 top = count.HighPart & _nt_highMask;
      top = top << (32 - _nt_bitShift);
      count.LowPart = count.LowPart >> _nt_bitShift;
      count.LowPart = count.LowPart + top;
      ret_val = count.LowPart;
   }
/*
   else
   {
#if defined(__MINGW32__)
      mMicroSeconds = time();
#elif defined(WIN16)
      mMicroSeconds = clock();        // milliseconds since application start
#else
      mMicroSeconds = GetTickCount();  // milliseconds since system start
#endif
   }
*/
#else    // Default to POSIX time setting

   timeval cur_time;
   gettimeofday(&cur_time, NULL);
   ret_val = (cur_time.tv_usec + (1000000 * cur_time.tv_sec));
#endif

   return ret_val;
}

}

#endif
