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
   int x, y;                 /**< upper left corner of rectangle */
   int width, height;        /**< dimensions of rectangle */
};

}


#endif /* _VRJVNC_RECTANGLE_H_ */
