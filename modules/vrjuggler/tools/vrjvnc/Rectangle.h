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
