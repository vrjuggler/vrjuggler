#include <config.h>
#include <Kernel/vjDisplayManager.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjDrawManager.h>
#include <Kernel/vjKernel.h>
#include <Math/vjCoord.h>

vjDisplayManager* vjDisplayManager::_instance = NULL;

void vjDisplayManager::setDrawManager(vjDrawManager* drawMgr)
{ drawManager = drawMgr; }

void vjDisplayManager::setupHeadIndices()
{
   headProxyIndex = vjKernel::instance()->getInputManager()->GetProxyIndex("vjHead");
   vjDEBUG(5) << "      headIndex: " << headProxyIndex << endl;
}

    // notifyDrawMgr = 0; Defaults to 0
int vjDisplayManager::addDisplay(vjDisplay* disp, int notifyDrawMgr)
{
	// For now just do this
    displays.push_back(disp);
    
	// --- Update Local Display structures
    //Open new window object;
    //Assign it the correct size,  position,  and system specific data???;
    //Place it in the vector;
    
    //if(notifyDrawMgr)
    //    Tell Draw Manager to add dislay;

    return 1;	
}
    
int vjDisplayManager::closeDisplay(int dispId)
{
    //Tell draw manager to kill Display;
    //Update local data structures;

    return 1;
}
    
vjDisplay* vjDisplayManager::getDisplay(int dispId)
{
    return displays[dispId];
}
    
void vjDisplayManager::updateProjections()
{
   vjKernel::instance()->getInputManager()->GetPosData(curHeadPos, headProxyIndex);
   vjCoord  head_coord(curHeadPos);       // Create a user readable version
   
   vjDEBUG(1) << "vjDisplayManager::updateProjections: Getting head position" << endl;
   vjDEBUG(0) << "\tHeadPos:" << head_coord.pos << "\tHeadOr:" << head_coord.orient << endl;
   
   // Compute location of left and right eyes
   float interocularDist = 2.75/12.0f;
   float eye_offset = interocularDist/2.0f;      // Distance to move eye
   vjMatrix left_eye_pos;                            // The eye position to use
   vjMatrix right_eye_pos;
      // NOTE: Eye coord system is -z forward, x-right, y-up
   
   left_eye_pos.postTrans(curHeadPos, -eye_offset, 0, 0);
   right_eye_pos.postTrans(curHeadPos, eye_offset, 0, 0); 

   //**//left_eye_pos = curHeadPos;
   //**//right_eye_pos = curHeadPos;

   // for (all displays) update the projections
   for (vector<vjDisplay*>::iterator i = displays.begin(); i != displays.end(); i++)
      (*i)->updateProjections(left_eye_pos, right_eye_pos);
}

    
