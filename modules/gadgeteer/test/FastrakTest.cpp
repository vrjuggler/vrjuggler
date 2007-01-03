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
   fastrak.setUnits(Fastrak::CENTIMETERS);
   fastrak.setHemisphere(1, Fastrak::FORWARD_HEM);
   fastrak.setHemisphere(2, Fastrak::FORWARD_HEM);
   fastrak.setHemisphere(3, Fastrak::FORWARD_HEM);
   fastrak.setHemisphere(4, Fastrak::FORWARD_HEM);
   fastrak.setStylusButtonEnabled(1, false);
   fastrak.setStylusButtonEnabled(2, false);
   fastrak.setStylusButtonEnabled(3, false);
   fastrak.setStylusButtonEnabled(4, false);

   fastrak.init();
   fastrak.getStationStatus(1);
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
   fastrak.close();
}
