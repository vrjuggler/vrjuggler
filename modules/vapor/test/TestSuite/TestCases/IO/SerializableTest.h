#ifndef _PLXTEST_SERIALIZABLE_TEST_H_
#define _PLXTEST_SERIALIZABLE_TEST_H_

#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <vpr/IO/SerializableObject.h>


namespace vprTest
{

class SerializableTest : public CppUnit::TestFixture
{
public:
   class Class1 : public vpr::SerializableObject
   {
   public:
      Class1() : charVal(0), shortVal(0), longVal(0), longlongVal(0), floatVal(0), doubleVal(0)
      {;}

      virtual void writeObject(vpr::ObjectWriter* writer)
         throw (vpr::IOException)
      {
         writer->writeUint8(charVal);
         writer->writeUint16(shortVal);
         writer->writeUint32(longVal);
         writer->writeUint64(longlongVal);
         writer->writeUint8(scharVal);
         writer->writeUint16(sshortVal);
         writer->writeUint32(slongVal);
         writer->writeUint64(slonglongVal);
         writer->writeFloat(floatVal);
         writer->writeDouble(doubleVal);
      }

      virtual void readObject(vpr::ObjectReader* reader)
         throw (vpr::IOException)
      {
         charVal = reader->readUint8();
         shortVal = reader->readUint16();
         longVal = reader->readUint32();
         longlongVal = reader->readUint64();
         scharVal = reader->readUint8();
         sshortVal = reader->readUint16();
         slongVal = reader->readUint32();
         slonglongVal = reader->readUint64();
         floatVal = reader->readFloat();
         doubleVal = reader->readDouble();
      }

      bool operator==(Class1& r) const
      {
         return ( (charVal == r.charVal) &&
                  (shortVal == r.shortVal) &&
                  (longVal == r.longVal) &&
                  (longlongVal == r.longlongVal) &&
                  (scharVal == r.scharVal) &&
                  (sshortVal == r.sshortVal) &&
                  (slongVal == r.slongVal) &&
                  (slonglongVal == r.slonglongVal) &&
                  (floatVal == r.floatVal) &&
                  (doubleVal == r.doubleVal) );
      }

   public:
      vpr::Uint8  charVal;
      vpr::Uint16 shortVal;
      vpr::Uint32 longVal;
      vpr::Uint64 longlongVal;

      vpr::Int8   scharVal;
      vpr::Int16  sshortVal;
      vpr::Int32  slongVal;
      vpr::Int64  slonglongVal;

      float       floatVal;
      double      doubleVal;
   };

   class Class2 : public vpr::SerializableObject
   {
   public:
      Class2() : mFlag(true)
      {;}

      virtual void writeObject(vpr::ObjectWriter* writer)
         throw (vpr::IOException)
      {
         mObj1.writeObject(writer);
         mObj2.writeObject(writer);
         writer->writeBool(mFlag);
      }

      virtual void readObject(vpr::ObjectReader* reader)
         throw (vpr::IOException)
      {
         mObj1.readObject(reader);
         mObj2.readObject(reader);
         mFlag = reader->readBool();
      }

      bool operator==(Class2& r)
      {
         return ( (mObj1 == r.mObj1) &&
                  (mObj2 == r.mObj2) &&
                  (mFlag == r.mFlag) );
      }

   public:
      Class1   mObj1;
      Class1   mObj2;
      bool     mFlag;
   };

public:
CPPUNIT_TEST_SUITE(SerializableTest);
CPPUNIT_TEST( testReaderWriter );
CPPUNIT_TEST( testDataOffsets );
CPPUNIT_TEST( testReadWriteSimple );
CPPUNIT_TEST( testReadWriteNested );
CPPUNIT_TEST_SUITE_END();

public:
   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}

   // Test reading and writing of data
   void testReaderWriter();

   // Test reading and writing data from many memory offsets
   void testDataOffsets();

   void testReadWriteSimple();

   void testReadWriteNested();
};

}

#endif
