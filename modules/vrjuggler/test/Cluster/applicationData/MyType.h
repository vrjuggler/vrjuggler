#include <vpr/IO/SerializableObject.h>

struct MyType
{
   int something;
   float otherStuff;
   char stupid;
   bool drawBool;
};

template<class MyType>
vpr::ReturnStatus vpr::SerializableObjectMixin<MyType>::writeObject(vpr::ObjectWriter* writer)
{ 
   writer->writeUint16(something);  
   writer->writeBool(drawBool);
}

template<class MyType>
vpr::ReturnStatus vpr::SerializableObjectMixin<MyType>::readObject(vpr::ObjectReader* reader)
{
   something = reader->readUint16();
   drawBool = reader->readBool();
}

