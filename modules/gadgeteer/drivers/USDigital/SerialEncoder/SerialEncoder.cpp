#include <gmtl/Generate.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

#include "SerialEncoder.h"

void initDevice(gadget::InputManager* inputMgr){
	new gadget::DeviceConstructor<SerialEncoder>(inputMgr);
}

SerialEncoder::SerialEncoder()
{
   mExitFlag = false;
	mSampleThread=0;
	mBaudRate=0;
}

SerialEncoder::~SerialEncoder()
{
	this->stopSampling();
}

bool SerialEncoder::config(jccl::ConfigElementPtr e)
{
	if(!(gadget::Input::config(e) && gadget::Position::config(e)))
   {
		return false;
   }

	mPortStr=e->getProperty<std::string>("port");
	mBaudRate=e->getProperty<long>("baud");

	return true;
}

bool SerialEncoder::startSampling()
{
	mBus.initializeSEI(mPortStr.c_str());
	mExitFlag = false;
	mSampleThread=new vpr::Thread(threadedSampleFunction,(void*)this);

	if(!mSampleThread->valid())
   {
		return false; // thread creation failed
   }
	else
   {
		return true; // thread creation success
   }
}
    
bool SerialEncoder::sample()
{
	unsigned int tiltDevice=1;
	unsigned int heightDevice=2;

	std::vector<gadget::PositionData> positionSample(1);

	positionSample[0].setTime();

	static float tableRotation=0;
	mBus.ED2GetPosition1(tiltDevice,tableRotation);
	gmtl::EulerAngleXYZf eulerRot(gmtl::Math::deg2Rad(tableRotation),0,0);
	gmtl::Matrix44f rotation;
	gmtl::setRot(rotation,eulerRot);

	float tableHeight=0;
	mBus.ED2GetPosition1(heightDevice,tableHeight);
   tableHeight=tableHeight/gadget::PositionUnitConversion::ConvertToInches;
	gmtl::Vec3f vecTrans;
	vecTrans[1]+=tableHeight;
	gmtl::Matrix44f translation;
	gmtl::setTrans(translation,vecTrans);

   positionSample[0].mPosData=translation*rotation;
   
	addPositionSample(positionSample);

	return true;
}

bool SerialEncoder::stopSampling(){
	if(mSampleThread!=NULL)
   {
      mExitFlag = true;
		mSampleThread->join();
		delete mSampleThread;
		mSampleThread=NULL;
	}

	return true;
}

void SerialEncoder::threadedSampleFunction(void* classPointer){
	SerialEncoder *this_ptr=static_cast<SerialEncoder*>(classPointer);
	
	while(!mExitFlag)
   {
		this_ptr->sample();
		vpr::System::msleep(1);
	}
}

