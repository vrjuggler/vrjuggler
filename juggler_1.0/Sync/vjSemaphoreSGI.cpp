/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
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


#include <vjConfig.h>

#include <Sync/vjSemaphore.h>
#include <Sync/vjSemaphoreSGI.h>

vjMemPoolSGI* vjSemaphoreSGI::semaphorePool = NULL;
int* vjSemaphoreSGI::attachedCounter = NULL;
