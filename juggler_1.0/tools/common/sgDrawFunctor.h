#ifndef _SG_DRAW_FUNCTOR_
#define _SG_DRAW_FUNCTOR_

class sgChan;

// Provide an interface to a draw function
class sgDrawFunctor
{
public:
   virtual void draw(sgChan* chan) = 0;
};

#endif

