#ifndef GADGET_NET_DEVICE_H
#define GADGET_NET_DEVICE_H


#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Position.h>
#include <gadget/RemoteInputManager/NetUtils.h>
//#include <vpr/IO/ObjectReader.h>
//#include <vpr/IO/ObjectWriter.h>
#include <gadget/RemoteInputManager/StreamWriter.h>

namespace gadget {

    class Proxy;
    
    class GADGET_CLASS_API NetDevice {
    protected:
        Input* mRealInput;      //Pointer to the real Input device
        std::string mDeviceName;
        std::string mSrcName;
		  int mNumDependencies;
        bool mAckInitialization;
        bool mWaitingForData;
        bool mNeedToResendRequest;
		  //vpr::ObjectWriter* writer;

        VJ_NETID_TYPE mLocalId; // unsigned short
        VJ_NETID_TYPE mRemoteId;

        //std::vector <char> mSendBuffer;
		  std::vector <vpr::Uint8> mSendBuffer;
        const uint DATA_SIZE; // sizeof data: 2 for NetDigital
        const uint DATA_TIME_SIZE; // size of Data + Timestamp: 2 + 4 for NetDigital

        // Constructor to use when a device is being REQUESTED
        //  NetInput(const std::string & src_device_name, Input * input_ptr, VJ_NETID_TYPE local_device_id,
        //  VJ_NETID_TYPE rmt_device_id, uint data_size);


    public:
      NetDevice(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id, uint p_data_size) : DATA_SIZE(p_data_size), DATA_TIME_SIZE(p_data_size + sizeof(float))
		{
			mLocalId = local_device_id;
			mRemoteId = rmt_device_id;
			mDeviceName = src_device_name;
			mSrcName = src_device_name;
			mAckInitialization = false;
			mNeedToResendRequest = false;
		}

		 
		 
		 
		 
		 NetDevice(std::string dev_name, VJ_NETID_TYPE local_device_id, uint p_data_size) : DATA_SIZE(p_data_size), DATA_TIME_SIZE(p_data_size + sizeof(float))
		  { 
			  //mSendBuffer.resize(1); // not used;
				//mNetworkShortDigitalValues.resize(1);  // a digital proxy only points to a single data item
				//mSendBuffer.resize(3 + DATA_TIME_SIZE * mNetworkShortDigitalValues.size());

			   mLocalId = local_device_id;
				mRemoteId = 0;
				mAckInitialization = false;
				mNeedToResendRequest = false;
				mSrcName = dev_name;
				//mInstName = mSrcName;
				//mInstName += "_NET_";
			  }
        ~NetDevice() { ; }

        // The following two functions help us keep track if this net_input is being used.
        // We just count them so we'll know when it's not being used anymore -- there's
        // currently no reason to store pointers to each dependency,
        void addDependency() { mNumDependencies++; }
        void removeDependency() { mNumDependencies--; }
		  void setRealDevice(Input* new_device){mRealInput = new_device;}
		  VJ_NETID_TYPE getLocalId() const { return mLocalId; }
        char* getDataPtr(){  return (char*) &(mSendBuffer[0]); }
		  int getDataByteLength() const { return mSendBuffer.size(); }
        VJ_NETID_TYPE getRemoteId() const { return mRemoteId; }
		  void setRemoteId(VJ_NETID_TYPE remote_id){ mRemoteId = remote_id; }
		  void setWasInitializedTrue(){ mAckInitialization = true; }

        bool getWasInitialized() const { return mAckInitialization; }
		  void setNeedToResendRequestFlag() { mNeedToResendRequest = true; }
		  void clearNeedToResendRequestFlag() { mNeedToResendRequest = false; }
		  bool needsToResendRequest() { return mNeedToResendRequest ; }
        void updateFromRemoteSource(char* recv_buffer, int recv_buff_len)
		  {

			 /* if(recv_buff_len < (int)getDataByteLength()){  // make sure the data length is long enough
				vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
					<< "ERROR NetDigital: recv_buff_len is not big enough to receive message."
					<< std::endl << vprDEBUG_FLUSH;
				return;
				}

			std::vector<DigitalData> digital_data_sample(mNetworkShortDigitalValues.size());

			vprASSERT(mNetworkShortDigitalValues.size() == 1);

			// get the data out of the buffer
			for(unsigned int k = 0; k < mNetworkShortDigitalValues.size(); k++){
				for(unsigned int i = 0; i < DATA_SIZE; i++)
					((char*)( &(mNetworkShortDigitalValues[k]) ))[i] = recv_buffer[2 + DATA_TIME_SIZE*k + i];

				float dev_time;
				bytes4ToFloat(dev_time, recv_buffer + 2 + DATA_TIME_SIZE*k + DATA_SIZE );
				vpr::Interval dev_time_interval;
				dev_time_interval.usecf(vj_ntohf(dev_time));
				digital_data_sample[k].setTime(dev_time_interval);
			//       digital_data_sample[k].setTime(vpr::Interval((vpr::Uint64)vj_ntohf(dev_time)));       // get timestamp from buffer

				// convert network data to local data
				digital_data_sample[k] = (short) ntohs(mNetworkShortDigitalValues[k]);
				vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
					<< "NetDigital: src_name: " << mSrcName << ",  DeviceName"
					<< mDeviceName << ".  Receiving : __________________________ : "
					<< digital_data_sample[k].getDigital() << std::endl << vprDEBUG_FLUSH;
			}

			mDigitalSamples.lock();
			mDigitalSamples.addSample(digital_data_sample);
			mDigitalSamples.unlock();

			mDigitalSamples.swapBuffers();
			vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
				<< "NetDigital: New Data available: -------------------------- : "
				<< this->getDigitalData(0).getDigital() << std::endl << vprDEBUG_FLUSH;
				*/
		  }


		  void updateFromLocalSource(StreamWriter* streamWriter)
		  {
				//ushortTo2Bytes((char*)(&(mSendBuffer[0])), htons(mRemoteId));  // put the 2 byte id in the buffer
	
			   //if(writer != NULL)
				//{
				//	delete writer;
				//}
				//writer = new vpr::ObjectWriter;
				
            streamWriter->writeUint16(mRemoteId);
            
				//std::vector <char> mSendBuffer;
				//mSendBuffer = *(writer->getData());

				//mRealInput->updateFromNetwork(writer, NULL);

				/*if ( dynamic_cast<Digital*> (mRealInput) )
            {
                mRealInput->Digital::writeObject(writer);
				}
            if ( dynamic_cast<Analog*> (mRealInput) )
            {
                mRealInput->Analog::writeObject(writer);
            }
            if ( dynamic_cast<Position*> (mRealInput) )
            {
                mRealInput->Position::writeObject(writer);
            }
				*/


				/*unsigned int j = 0;
				for( ; j < mNetworkShortDigitalValues.size(); j++){
						for(unsigned int i = 0; i < DATA_SIZE; i++)  // 
						mSendBuffer[2 + DATA_TIME_SIZE*j + i] = ((char*)(& (mNetworkShortDigitalValues[j]) ))[i];   // copy the number byte by byte
					float net_float = vj_htonf(digital_data_sample[j].getTime().usecf());
					floatTo4Bytes((char*)(&(mSendBuffer[2 + DATA_TIME_SIZE*j + DATA_SIZE])), net_float);   // copy the timestamp
					}
				*/
				
				
				
				//VVVVVVVVVVVVVVVV
				
				//mSendBuffer[2 + DATA_TIME_SIZE * j] = ';';                            // terminate the message

			 
        }

		  //void setWasInitializedTrue() { mAckInitialization = true; }

        //bool getWasInitialized() const { return mAckInitialization; }

        //void setNeedToResendRequestFlag() { mNeedToResendRequest = true; }

        //void clearNeedToResendRequestFlag() { mNeedToResendRequest = false; }

        //bool needsToResendRequest() { return mNeedToResendRequest; }

        std::string getSourceName() const { return mSrcName; }

        //virtual void updateFromLocalSource() { ; }

        //virtual void updateFromRemoteSource(char * recv_buffer, int recv_buff_len) { ; }


        //virtual int getDataByteLength() const { return mSendBuffer.size(); }

        //virtual VJ_NETID_TYPE getLocalId() const { return mLocalId; }

        //virtual VJ_NETID_TYPE getRemoteId() const { return mRemoteId; }

        //virtual void setRemoteId(VJ_NETID_TYPE remote_id) { mRemoteId = remote_id; }

        //int sample() { return 1; }
        //int startSampling() { return 1; }
        //int stopSampling() { return 1; }
        //void updateData() { ; }
        //void refresh() { ; }

    };

} // end namespace gadget

#endif




