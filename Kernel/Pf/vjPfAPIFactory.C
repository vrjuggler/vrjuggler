#include <config.h>

#include <Kernel/Pf/vjPfAPIFactory.h>
#include <Kernel/Pf/vjPfDrawManager.h>

       
//: Get a Performer DrawManager
vjDrawManager* vjPfAPIFactory::getDrawManager()
{ return vjPfDrawManager::instance();}

vjPfAPIFactory* vjPfAPIFactory::_instance = NULL;
