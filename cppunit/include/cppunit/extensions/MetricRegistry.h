#ifndef CPPUNIT_EXTENSIONS_METRICREGISTRY_H_
#define CPPUNIT_EXTENSIONS_METRICREGISTRY_H_

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
      if(mInstance == NULL)
      { mInstance = new MetricRegistry; }
      return mInstance;
   }
protected:
   static MetricRegistry* mInstance;

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

   /** Assert that we are greater then or equal to the cur_value */
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
                      << test_key << "  value:" << cur_value << std::endl;
            CPPUNIT_ASSERT(false);
         }
         else if(!(cur_value >= soft_limit_value))
         {
            std::cout << "SoftLimit [" << soft_limit_value << "] failed on metric: "
                      << test_key << "  value:" << cur_value << std::endl;
         }
         else if(cur_value > old_sample)
         {
            std::cout << "New metric for [" << test_key << "]  new value:" << cur_value
                      << " --- increase of [" << (cur_value-old_sample)/old_sample << "] percent." << std::endl;
            setMetric(test_key, cur_value);
         }
      }
   }

   /** Assert that we are less then or equal to the cur_value */
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
                      << test_key << "  value:" << cur_value << std::endl;
            CPPUNIT_ASSERT(false);
         }
         else if(!(cur_value <= soft_limit_value))
         {
            std::cout << "SoftLimit [" << soft_limit_value << "] failed on metric: "
                      << test_key << "  value:" << cur_value << std::endl;
         }
         else if(cur_value < old_sample)
         {
            std::cout << "Changed metric for [" << test_key << "]  new value:" << cur_value
                      << " --- decrease of [" << (old_sample-cur_value)/old_sample << "] percent." << std::endl;
            setMetric(test_key, cur_value);
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

            std::cout << "[r] metric: " << key << " -- " << value << std::endl;
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
         output_file << std::setw(50) << (*i).first << "    " << (*i).second << "\n";

         std::cout << "[w] metric: " << (*i).first << " -- " << (*i).second << std::endl;
      }

      output_file.close();
      return true;
   }


private:
   std::string mFilename;
   bool        mHaveOpenedFile;

   std::string mPrefix;

   typedef std::map<std::string, double> MetricMapType;
   MetricMapType mMetricMap;
};

MetricRegistry* MetricRegistry::mInstance = NULL;

}; // namespace

#define CPPUNIT_ASSERT_METRIC_GE(test_key, cur_value, soft_limit, hard_limit)\
         (CppUnit::MetricRegistry::instance()->assertMetricGE(test_key, cur_value, soft_limit, hard_limit))

#define CPPUNIT_ASSERT_METRIC_LE(test_key, cur_value, soft_limit, hard_limit)\
         (CppUnit::MetricRegistry::instance()->assertMetricLE(test_key, cur_value, soft_limit, hard_limit))


#endif
