#include <Input/InputManager/vjProxyDepChecker.h>

#include <Input/InputManager/vjAnalogProxy.h>
#include <Input/InputManager/vjDigitalProxy.h>
#include <Input/InputManager/vjPosProxy.h>
#include <Input/InputManager/vjGloveProxy.h>
#include <Input/InputManager/vjGestureProxy.h>
#include <Input/InputManager/vjKeyboardProxy.h>
#include <Input/InputManager/vjProxyDepChecker.h>


bool vjProxyDepChecker::canHandle(vjConfigChunk* chunk)
{
   std::string chunk_type = (std::string)chunk->getType();

   bool ret_val;
   ret_val = ((chunk_type == vjAnalogProxy::getChunkType()) ||
              (chunk_type == vjDigitalProxy::getChunkType()) ||
              (chunk_type == vjGestureProxy::getChunkType()) ||
              (chunk_type == vjGloveProxy::getChunkType()) ||
              (chunk_type == vjKeyboardProxy::getChunkType()) ||
              (chunk_type == vjPosProxy::getChunkType()));

   return ret_val;
}


