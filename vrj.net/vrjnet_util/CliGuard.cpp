// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#include <stdlib.h>

#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

#if defined(USE_MONO)
extern "C"
{

#include <mono/metadata/threads.h>

}
#endif

#include "CliGuard.h"


namespace vrjnet
{

vpr::TSObjectProxy<CliGuard::State> CliGuard::mState;

CliGuard::CliGuard()
{
#if defined(USE_MONO)
   if ( NULL == mState->mDomain )
   {
      mState->mDomain = mono_domain_create();
      mono_thread_attach(mState->mDomain);
   }
#endif
}

CliGuard::~CliGuard()
{
   ;
}

CliGuard::State::State()
#if defined(USE_MONO)
   : mDomain(NULL)
#endif
{
}

}
