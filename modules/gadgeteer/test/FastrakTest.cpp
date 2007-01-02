#include <iostream>
#include <FastrakStandalone.h>
#include <string>
#include <vpr/Util/Debug.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>

int main()
{
   FastrakStandalone fastrak("/dev/ttyS0", 115200);
   fastrak.open();
   fastrak.init();
   fastrak.getStationStatus(1);
   fastrak.getStationStatus(2);
   fastrak.getStationStatus(3);
   fastrak.getStationStatus(4);
   while(true)
   {
      fastrak.readData();
      for (unsigned int i = 1; i < 5; i ++)
      {
         gmtl::Matrix44f& matrix = fastrak.getStationPosition(i);
         gmtl::Vec3f pos = gmtl::makeTrans<gmtl::Vec3f>(matrix);
         std::cout << i << ": Position: " << pos << std::endl;
      }
      std::cout << std::endl;
   }
   //fastrak.close();
}
