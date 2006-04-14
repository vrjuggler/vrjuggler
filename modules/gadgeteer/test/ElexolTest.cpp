#include <iostream>
#include <Ether24Standalone.h>
#include <string>
#include <vpr/Util/Debug.h>

void printValue(Ether24Standalone& elexol)
{
   vpr::Uint8 porta_value = elexol.getValue(Elexol::PortA);
   vpr::Uint8 portb_value = elexol.getValue(Elexol::PortB);
   vpr::Uint8 portc_value = elexol.getValue(Elexol::PortC);
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Value           [" << (int)porta_value << "][" << (int)portb_value
      << "][" << (int)portc_value << "]"
      << std::endl << vprDEBUG_FLUSH;
}
void printDirection(Ether24Standalone& elexol)
{
   vpr::Uint8 porta_value = elexol.getDirection(Elexol::PortA);
   vpr::Uint8 portb_value = elexol.getDirection(Elexol::PortB);
   vpr::Uint8 portc_value = elexol.getDirection(Elexol::PortC);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Direction       [" << (int)porta_value << "][" << (int)portb_value
      << "][" << (int)portc_value << "]"
      << std::endl << vprDEBUG_FLUSH;
}
void printPullUp(Ether24Standalone& elexol)
{
   vpr::Uint8 porta_value = elexol.getPullUp(Elexol::PortA);
   vpr::Uint8 portb_value = elexol.getPullUp(Elexol::PortB);
   vpr::Uint8 portc_value = elexol.getPullUp(Elexol::PortC);
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "PullUp          [" << (int)porta_value << "][" << (int)portb_value
      << "][" << (int)portc_value << "]"
      << std::endl << vprDEBUG_FLUSH;
}
void printThreshold(Ether24Standalone& elexol)
{
   vpr::Uint8 porta_value = elexol.getThreshold(Elexol::PortA);
   vpr::Uint8 portb_value = elexol.getThreshold(Elexol::PortB);
   vpr::Uint8 portc_value = elexol.getThreshold(Elexol::PortC);
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Threshold       [" << (int)porta_value << "][" << (int)portb_value
      << "][" << (int)portc_value << "]"
      << std::endl << vprDEBUG_FLUSH;
}
void printSchmittTrigger(Ether24Standalone& elexol)
{
   vpr::Uint8 porta_value = elexol.getSchmittTrigger(Elexol::PortA);
   vpr::Uint8 portb_value = elexol.getSchmittTrigger(Elexol::PortB);
   vpr::Uint8 portc_value = elexol.getSchmittTrigger(Elexol::PortC);
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Schmitt Trigger [" << (int)porta_value << "][" << (int)portb_value
      << "][" << (int)portc_value << "]"
      << std::endl << vprDEBUG_FLUSH;
}


int main()
{
   Elexol::device_map_t devices = Ether24Standalone::getDevicesByMacAddress();

   //vpr::InetAddr my_addr = devices["00:11:ba:02:01:bd"];
   vpr::InetAddr my_addr = devices["00:11:ba:00:00:01"];
   Ether24Standalone elexol;
   elexol.open(my_addr);
   printValue(elexol);

   elexol.setDirection(Elexol::PortA, 0);
   elexol.setValue(Elexol::PortA, 128);
   elexol.setDirection(Elexol::PortA, 255);
   printValue(elexol);

   printDirection(elexol);
   printPullUp(elexol);
   printThreshold(elexol);
   printSchmittTrigger(elexol);

   vpr::InetAddr new_addr;
   new_addr.setAddress("192.168.1.200");

   elexol.setFixedIpAddress(new_addr);
   elexol.setEnableFixedIpAddress(true);
   elexol.setEnableFixedIpAddress(false);

   vpr::InetAddr fixed_addr = elexol.getFixedIpAddress();
   std::cout << "Fixed Address: [" << fixed_addr.getAddressString() << ":"
      << fixed_addr.getPort() << "]" << std::endl;
   elexol.close();
}
