#include "InputGroup.h"
#include <iostream.h>

int main()
{
  MemPool* anSgiPool = new MemPoolSGI(1024*1024);
  C2POS_DATA* data;
  InputGroup* ig = new (anSgiPool) InputGroup ;
  
  C2DummyPosition* myDummy = new (anSgiPool) C2DummyPosition;
  myDummy->SetX(5.0);
  
  ig->FGetPosData(data,0);
  cout  << "  x:" << data->x 
	<< "  y:" << data->y 
	<< "  z:" << data->z << endl;

  int devNum = ig->FAddDevice(myDummy);
  ig->SetPosProxy(0,devNum,0);
  
  ig->FGetPosData(data,0);
  cout  << "  x:" << data->x 
	<< "  y:" << data->y 
	<< "  z:" << data->z << endl;
  cout << "\ntype something: " << flush ;
  cin.get();

  delete ig;
  delete anSgiPool;

  return 0;
} 
