#ifndef _GADGET_NETWORK_ADAPTOR_H
#define _GADGET_NETWORK_ADAPTOR_H

#include <gadget/AbstractNetworkManager.h>
#include <gadget/Util/Debug.h>

namespace gadget
{

template<typename A, typename C>
class NetworkManager : public AbstractNetworkManager
{
private:
   void operator= (const NetworkManager& na)
   {;}
public:
   NetworkManager() : mAcceptor(this)
   {;}
   virtual vpr::ReturnStatus attemptConnect(Node* node)
   {
      return(mConnector.attemptConnect(node));
   }
   virtual void startListening(int listen_port)
   {
      mAcceptor.startListening(listen_port);
   }
private:
   A mAcceptor;
   C mConnector;
};

} // End namespace gadget

#endif /* _GADGET_NETWORK_ADAPTOR_H */
