// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _SHARPPY_H_
#define _SHARPPY_H_

#ifdef _MSC_VER
#define SHARPPY_API __declspec(dllexport)
#define SHARPPY_CLASS_API __declspec(dllexport)
#else
#define SHARPPY_API
#define SHARPPY_CLASS_API
#endif

#endif /* _SHARPPY_H_ */
