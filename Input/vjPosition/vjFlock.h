/////////////////////////////////////////////////////////////////////////
// vjFlock.h (a Wrapper for aFlock)
//
// VR Juggler Ascention Flock of birds tracking class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_ASCENSION_FLOCK_OF_BIRDS_H_
#define _VJ_ASCENSION_FLOCK_OF_BIRDS_H_

#include <vjConfig.h>
#include <Input/vjPosition/vjPosition.h>
#include <Threads/vjThread.h>
#include <Input/vjPosition/aFlock.h>

//----------------------------------------------------------------------------
//: Position derived class for running a Flock of Birds.
//: , also a wrapper class for the real ascension flock class "aFlock"
//
//  vjFlock adds to the aFlock class shared memory and threading.<br>
//  vjFlock is a positional device driver for the Flock of Birds, the config
//  chunk in the constructor should set up all the settings, for these to be
//  changed the Flock has to be deleted and a new instance created with an
//  updated configchunk.
//  <br>
//  Note: Some functions still remain for changing the options of
//    the flock when its not in Sampling mode, but in order to stay
//    consistent with the vjInput/vjPosition functionality these
//    are only left for building apps without vjConfigChunks
//---------------------------------------------------------------------------
//!PUBLIC_API:
class vjFlock : public vjPosition {
    public:
        //: Configure Constructor
	// Give:                                                 <BR>
	//   port - such as "/dev/ttyd3"                         <BR>
	//   baud - such as 38400, 19200, 9600, 14400, etc...    <BR>
	//   sync - sync type.                                   <BR>
	//   block - blocking                                    <BR>
	//   numBrds - number of birds in flock,                 <BR>
	//   transmit - transmitter unit number,                 <BR>
	//   hemi - hemisphere to track from,                    <BR>
	//   filt - filtering type,                              <BR>
	//   report - flock report rate.                         <BR>
	//   calfile - a calibration file, if "", then use none. <BR>
	//                                                       <BR>
	// Result: configures internal data members, 
	//         doesn't actually talk to the FOB yet.
	vjFlock(const char* const port = "/dev/ttyd3", 
		const int& baud = 38400, 
		const int& sync = 1, 
		const int& block = 0, 
		const int& numBrds = 3, 
		const int& transmit = 3, 
		const BIRD_HEMI& hemi = LOWER_HEM, 
		const BIRD_FILT& filt = AC_NARROW, 
		const char& report = 'R', 
		const char* const calfile = "");
	~vjFlock();


   virtual bool config(vjConfigChunk* c);

	/** @name vjInput pure virtual functions
	 *
	 *  pure virtual functions required from vjInput
	 */
	//@{
	int StartSampling();
	int StopSampling();
	int Sample();
	void UpdateData();
	//@}
	
	/** @name vjInput virtual functions
	 *
	 *  virtual functions that inherited members should
	 *  override but are not required to
	 */
	//@{
	char* GetDeviceName() { return "vjFlock"; }
	//@}
	
   static string getChunkType() { return string("Flock");}

	/** @name vjPosition pure virtual functions
	 *
	 *  pure virtual functions required by vjPosition
	 */
	vjMatrix* GetPosData( int dev = 0); // 0 base
		
	

	//: see if the flock is active or not
	inline const bool& isActive() const { return mFlockOfBirds.isActive(); }

	//: set the port to use
	//  this will be a string in the form of the native OS descriptor <BR>
	//  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setPort( const char* const serialPort );
	
	//: get the port used
	//  this will be a string in the form of the native OS descriptor <BR>
	//  ex: unix - "/dev/ttyd3", win32 - "COM3"
	inline const char* const getPort() const { return mFlockOfBirds.getPort(); }
	
	
	//: set the baud rate
	//  this is generally 38400, consult flock manual for other rates. <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setBaudRate( const int& baud );
	
	//: get the baud rate
	//  this is generally 38400, consult flock manual for other rates. <BR>
	inline const int&  getBaudRate()  const { return mFlockOfBirds.getBaudRate();}
   
	//: Set the unit number of the transmitter
	//  give - an integer that is the same as the dip switch 
	//         setting on the transmitter box (for the unit number) <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setTransmitter( const int& Transmit );
	
	//: Get the unit number of the transmitter
	//  returns - an integer that is the same as the dip switch 
	//         setting on the transmitter box (for the unit number) <BR>
	inline const int&  getTransmitter() const { return mFlockOfBirds.getTransmitter(); }


	//: Set the number of birds to use in the flock.
	//  give - an integer number not more than the number of 
	//         birds attached to the system <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setNumBirds( const int& n );
	
	//: Get the number of birds to use in the flock.
	//  returns - an integer number not more than the number of 
	//         birds attached to the system <BR>
	inline const int&  getNumBirds() const { return mFlockOfBirds.getNumBirds(); }


	//: set the video sync type
	//  this option allows the Flock to syncronize its pulses with 
	//  your video display.  This will eliminate most flicker caused 
	//  by the magnetic distortion. <BR>
	//  - Refer to your flock manual for what number to use.
	//  NOTE: flock.isActive() must be false to use this function
	void	    setSync( const int& sync );
	
	//: Get the video sync type
	//  this option allows the Flock to syncronize its pulses with 
	//  your video display.  This will eliminate most flicker caused 
	//  by the magnetic distortion. <BR>
	//  - Refer to your flock manual for what the return number means
	inline const int&  getSync() const {return mFlockOfBirds.getSync(); }


	//: Set blocking of flock
	//  see flock manual for details.
	//  NOTE: flock.isActive() must be false to use this function
	void	    setBlocking( const int& blVal );
	
	//: Get blocking flock's blocking type
	//  see flock manual for details.
	inline const int&  getBlocking() const { return mFlockOfBirds.getBlocking(); }


	//: Set the type of filtering that the flock uses
	//  NOTE: flock.isActive() must be false to use this function
	void	    setFilterType( const BIRD_FILT& f );
	
	//: Set the type of filtering that the flock uses
	inline const BIRD_FILT&  getFilterType() const { return mFlockOfBirds.getFilterType(); }


	//: Set the hemisphere that the transmitter transmits from.
	//  NOTE: flock.isActive() must be false to use this function
	void	    setHemisphere( const BIRD_HEMI& h );
	
	//: Set the hemisphere that the transmitter transmits from.
	inline const BIRD_HEMI&  getHemisphere() const {return mFlockOfBirds.getHemisphere(); }


	//: Set the report rate that the flock uses
	//  NOTE: flock.isActive() must be false to use this function
	void	    setReportRate( const char& rRate );
        
	//: Set the report rate that the flock uses
	inline const char& getReportRate() const {return mFlockOfBirds.getReportRate(); }



	//: get the x position of the i'th reciever
	inline float&	    xPos( const int& i ) { return mFlockOfBirds.xPos( i ); }
	
	//: get the y position of the i'th reciever
	inline float&	    yPos( const int& i ) { return mFlockOfBirds.yPos( i ); }
	
	//: get the z position of the i'th reciever
	inline float&	    zPos( const int& i ) { return mFlockOfBirds.zPos( i ); }
	
	//: get the z rotation of the i'th reciever
	inline float&	    zRot( const int& i ) { return mFlockOfBirds.zRot( i ); };
	
	//: get the y rotation of the i'th reciever
	inline float&	    yRot( const int& i ) { return mFlockOfBirds.yRot( i ); }
	
	//: get the x rotation of the i'th reciever
	inline float&	    xRot( const int& i ) { return mFlockOfBirds.xRot( i ); }



     private:
	void Position_Correct(float&x,float&y,float&z);
	void InitCorrectionTable(const char*);
	vjThread*   myThread;      // The thread doing the flock sampling
	
	aFlock mFlockOfBirds;
};

#endif

	
