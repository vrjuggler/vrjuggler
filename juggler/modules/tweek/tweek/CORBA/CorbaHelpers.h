#ifndef _TWEEK_CORBA_HELPERS_H_
#define _TWEEK_CORBA_HELPERS_H_

#include <tweek/tweekConfig.h>

#include <string>
#include TWEEK_INCLUDE_CORBA_H
#include <vpr/Util/ReturnStatus.h>


namespace tweek
{

/**
 * Gets the root context for the CORBA Naming Service using the ORB's "resolve
 * initial references" capabilities and therefore requires that the reference
 * be accessible to the ORB through appropriate means.
 *
 * @param orb The reference to the ORB in this memory space.
 *
 * @pre  The Naming Service reference is available through the ORB's "resolve
 *       initial referneces" feature.
 * @post The root context is retrieved through the given ORB pointer.
 */
CosNaming::NamingContext_var getRootNamingContextByInitRef(CORBA::ORB_ptr orb);

/**
 * Gets the root context for the CORBA Naming Service using the given URI.
 *
 * @param orb            The reference to the ORB in this memory space.
 * @param nameServiceURI The URI to be used for looking up the Naming Service
 *                       reference.  This can be a corbaname or a corbaloc URI.
 *
 * @post The root context is retrieved through the given ORB pointer.
 */
CosNaming::NamingContext_var getRootNamingContextByURI(CORBA::ORB_ptr orb,
                                                       const std::string& nameServiceURI);

/**
 * Binds a local Naming Context for this memory space.
 *
 * @param parentContext The parent context within which the local context will
 *                      be created.
 * @param localId       The user-specified identifier for the context.
 *
 * @post A sub-context is created within the given root naming context.
 */
CosNaming::NamingContext_var bindLocalNamingContext(CosNaming::NamingContext_ptr parentContext,
                                                    const std::string& localId);

} // End of tweek namespace


#endif /* _TWEEK_CORBA_HELPERS_H_ */
