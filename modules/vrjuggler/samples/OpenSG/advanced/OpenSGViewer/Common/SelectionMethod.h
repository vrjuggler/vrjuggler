#ifndef SELECTION_METHOD_H_
#define SELECTION_METHOD_H_

class SelectionMethod
{
public:
   enum SelectionState { NotSelected=0,   /**< The entity is not currently selected (no isect) */ 
                         IsHighlighted=1, /**< The entity is highlighted, but not selected (yet) */
                         IsSelected=2     /**< The entity is currently selected (does isect */
                        };
public:
   virtual OSG::NodePtr checkSelection(OpenSGViewer* viewer) = 0;

};

#endif

