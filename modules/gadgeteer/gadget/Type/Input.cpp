#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>


vjInput::vjInput()
{
   deviceAbilities = 0;
   instName = NULL;
   sPort = NULL;
   myThread = NULL;
   active = 0;
}

vjInput::~vjInput()
{
    if (sPort != NULL)
        delete [] sPort;
    if (instName != NULL)
        delete [] instName;

}

bool vjInput::config( vjConfigChunk *c)
{
  sPort = NULL;
  char* t = c->getProperty("port").cstring();
  if (t != NULL)
  {
    sPort = new char[ strlen(t) + 1 ];
    strcpy(sPort,t);
  }
  t = c->getProperty("name").cstring();
  if (t != NULL)
  {
    instName = new char[ strlen(t) + 1];
    strcpy(instName,t);
  }

  baudRate = c->getProperty("baud");

  return true;
}


void vjInput::SetPort(const char* serialPort)
{
if (myThread != NULL) {
     cerr << "Cannot change the serial Port while active\n";
     return;
  }
  strncpy(sPort,serialPort,(size_t)30);
}

char* vjInput::GetPort()
{
  if (sPort == NULL) return "No port";
  return sPort;
}

void vjInput::SetBaudRate(int baud)
{
  if (myThread != NULL)
     baudRate = baud;
}

int vjInput::FDeviceSupport(int devAbility)
{
    return (deviceAbilities & devAbility);
}

//: Reset the Index Holders
// Sets to (0,1,2) in that order
void vjInput::resetIndexes()
{
    current = 0;
    valid = 1;
    progress = 2;
}

//: Swap the current and valid indexes (thread safe)
void vjInput::swapCurrentIndexes()
{
  lock.acquire();
  int tmp = current;
  current = valid;
  valid = tmp;
  lock.release();
}

//: Swap the valid and progress indexes (thread safe)
void vjInput::swapValidIndexes()
{
   lock.acquire();
	int tmp = valid;
	valid= progress;
	progress = tmp;
	lock.release();
}

