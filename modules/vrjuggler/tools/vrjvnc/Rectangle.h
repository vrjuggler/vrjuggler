/*********** <VRJ VNC auto-copyright.pl BEGIN do not edit this line> **********
 *
 * VRJ VNC is (C) Copyright 2003-2006 by Iowa State University
 *
 * Original Authors:
 *   Patrick Hartling, Allen Bierbaum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this application; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 ************ <VRJ VNC auto-copyright.pl END do not edit this line> **********/

#ifndef _VRJVNC_RECTANGLE_H_
#define _VRJVNC_RECTANGLE_H_


namespace vrjvnc
{

/**
 * Rectangle struct used for framebuffer updates. (I'd like to add
 * the merge() and isMergeable() members to this class, but this
 * will be impossible if it is to be CORBA-exposed.)
 */
struct Rectangle
{
public:
   int x, y;                 /**< upper left corner of rectangle */
   int width, height;        /**< dimensions of rectangle */

public:
   /** Set rect to the union of current and other */
   void merge(const Rectangle& other)
   {
      int x1(x+width);
      int y1(y+height);
      int x2(other.x+other.width);
      int y2(other.y+other.height);

      if(other.x < x)
         x = other.x;
      if(other.y < y)
         y = other.y;

      if(x1>x2)
         width = x1-x;
      else
         width = x2-x;

      if(y1>y2)
         height = y1-y;
      else
         height = y2-y;
   }
};

}


#endif /* _VRJVNC_RECTANGLE_H_ */
