/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */

#include <Input/vjSim/vjSimRelativePosition.h>
#include <Config/vjConfigChunk.h>

bool vjSimRelativePosition::config(vjConfigChunk* chunk)
{
    if(!vjPosition::config(chunk))
      return false;

   // Initialize the positional devices
   std::string base_frame_proxy = chunk->getProperty("base_frame_proxy");
   std::string relative_pos_proxy = chunk->getProperty("relative_proxy");

   mBaseFrame.init(base_frame_proxy);
   mRelativePos.init(relative_pos_proxy);
   return true;
}


void vjSimRelativePosition::updateData()
{
   mPos.mult( *(mBaseFrame->getData()),
              *(mRelativePos->getData()) );
}


