// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Includes ====================================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <gadget/Type/Position/PositionUnitConversion.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj
{

class PositionUnitConversionWrapper
{
public:
   static float ConvertToFeet;
   static float ConvertToInches;
   static float ConvertToMeters;
   static float ConvertToCentimeters;
};

float PositionUnitConversionWrapper::ConvertToFeet(gadget::PositionUnitConversion::ConvertToFeet);
float PositionUnitConversionWrapper::ConvertToInches(gadget::PositionUnitConversion::ConvertToInches);
float PositionUnitConversionWrapper::ConvertToMeters(gadget::PositionUnitConversion::ConvertToMeters);
float PositionUnitConversionWrapper::ConvertToCentimeters(gadget::PositionUnitConversion::ConvertToCentimeters);

}


// Module ======================================================================
void _Export_PositionUnitConversion()
{
   class_<pyj::PositionUnitConversionWrapper>("PositionUnitConversion", no_init)
      .def_readonly("ConvertToFeet",
                    &pyj::PositionUnitConversionWrapper::ConvertToFeet
       )
      .def_readonly("ConvertToInches",
                    &pyj::PositionUnitConversionWrapper::ConvertToInches
       )
      .def_readonly("ConvertToMeters",
                    &pyj::PositionUnitConversionWrapper::ConvertToMeters
       )
      .def_readonly("ConvertToCentimeters",
                    &pyj::PositionUnitConversionWrapper::ConvertToCentimeters
       )
   ;
}
