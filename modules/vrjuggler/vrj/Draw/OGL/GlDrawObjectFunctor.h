#ifndef GL_DRAW_OBJECT_FUNCTOR_H
#define GL_DRAW_OBJECT_FUNCTOR_H

namespace vrj
{

/** Base interface for an object drawing functor
* 
* Any GL draw objects should be derived from this class.
* This is used for drawing things like wands and gloves
*/
class GlDrawObjectFunctor
{
public:   
   /* Called to initialize any context specific information
   */
   virtual void contextInit() = 0;
   
   /** Callback function for drawing
   * Called when the object should be drawn
   * @pre GL context is set and ready to go
   */
   virtual void draw() = 0;
};

/** Base class for glove drawing
* Adds a function for setting glove parameters
*/
class GlDrawGloveObjectFunctor : public GlDrawObjectFunctor
{
public:
   virtual void setGlove() = 0;
};


};

#endif

