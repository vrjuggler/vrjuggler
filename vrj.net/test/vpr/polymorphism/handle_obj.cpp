#include <iostream>
#include <vpr/IO/SerializableObject.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>

extern "C"
{

void handleSerializableObject(vpr::SerializableObject* obj)
{
   std::cout << vpr::System::getCallStack() << std::endl;
   vpr::ReturnStatus status;

   std::vector<vpr::Uint8> data(8);
   vpr::BufferObjectReader reader = vpr::BufferObjectReader(&data);
   std::cout << "[C++] Address of reader: " << std::hex << &reader << std::dec
             << std::endl;
   std::cout << "[C++] Reading from object " << std::hex << obj << std::dec
             << " ..." << std::endl;
   status = obj->readObject(&reader);

   if ( status.success() )
   {
      std::cout << "[C++] vpr::SerializableObject::readObject() reported success"
                << std::endl;
   }
   else
   {
      std::cout << "[C++] vpr::SerializableObject::readObject() failed!"
                << std::endl;
   }

   std::cout << "[C++] Data size: " << data.size() << std::endl;

   vpr::BufferObjectWriter writer = vpr::BufferObjectWriter(&data);
   std::cout << "[C++] Address of writer: " << std::hex << &writer << std::dec
             << std::endl;
   std::cout << "[C++] Writing to object ..." << std::endl;
   status = obj->writeObject(&writer);

   if ( status.success() )
   {
      std::cout << "[C++] vpr::SerializableObject::writeObject() reported success"
                << std::endl;
   }
   else
   {
      std::cout << "[C++] vpr::SerializableObject::writeObject() failed!"
                << std::endl;
   }
}

}
