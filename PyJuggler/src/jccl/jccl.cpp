// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Include =====================================================================
#include <boost/python.hpp>

// Exports =====================================================================
void _Export_ConfigDefinition();
void _Export_ConfigElement();
void _Export_ConfigElementHandler();
void _Export_ConfigManager();
void _Export_PropertyDefinition();
void _Export_VarType();

// Module ======================================================================
BOOST_PYTHON_MODULE(jccl)
{
    _Export_ConfigDefinition();
    _Export_ConfigElement();
    _Export_ConfigElementHandler();
    _Export_ConfigManager();
    _Export_PropertyDefinition();
    _Export_VarType();
}
