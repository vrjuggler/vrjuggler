/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#ifndef _VJ_PF_UTIL_H_
#define _VJ_PF_UTIL_H_

/**************************************************
* Performer helper routines                       *
***************************************************/

#include <vjConfig.h>

#include <Performer/pr/pfLinMath.h>
#include <Math/vjMatrix.h>


//: Convert Performer matrix to Juggler matrix
vjMatrix vjGetVjMatrix(pfMatrix& perfMat);

#define featureChange(var)\
   std::cout<<"    !!!!!!!!!!!!  \n"<<std::flush;\
   std::cout<<"   !!!!!!!!!!!!!! \n"<<std::flush;\
   std::cout<<"  !!!!!!!!!!!!!!!!\n"<<std::flush;\
   std::cout<<"                  \n"<<std::flush;\
   std::cout<<"FEATURE CHANGED:  "<<var<<"\n"<<std::flush;\
   std::cout<<"                  \n"<<std::flush;\
   std::cout<<"  !!!!!!!!!!!!!!!!\n"<<std::flush;\
   std::cout<<"   !!!!!!!!!!!!!! \n"<<std::flush;\
   std::cout<<"    !!!!!!!!!!!!  \n"<<std::flush;\
   std::cout<<"Exiting...\n"<<std::flush;\
   exit(-1);
   
//inline vjMatrix vjGetVJMatrix(pfMatrix& perfMat)
//{
//   featureChange("Please change all references to vjGetVJMatrix to vjGetVjMatrix (lower case j)")
//   return vjMatrix();
//}

//: Convert Juggler Matrix to Pf Matrix
pfMatrix vjGetPfMatrix(vjMatrix& mat);


vjVec3 vjGetVjVec(pfVec3& vec);
pfVec3 vjGetPfVec(vjVec3& vec);


#endif
