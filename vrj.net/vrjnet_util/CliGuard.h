// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#include <vpr/vpr.h>
#include <vpr/Thread/TSObjectProxy.h>

#if defined(USE_MONO)
extern "C"
{
 
#include <mono/metadata/appdomain.h>
 
}
#endif


namespace vrjnet
{

class CliGuard
{
public:
   CliGuard();

   ~CliGuard();

private:
   struct State
   {
      State();

      ~State()
      {
      }

#if defined(USE_MONO)
      MonoDomain* mDomain;
#endif
   };

   static vpr::TSObjectProxy<State> mState;
//   bool mMyLock;
};

}
