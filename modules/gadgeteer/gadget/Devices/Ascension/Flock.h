/////////////////////////////////////////////////////////////////////////
// Birdy.h
//
// Flock of birds tracking class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_BIRDY_H_
#define _VJ_BIRDY_H_

#include <vjConfig.h>
#include <Input/vjPosition/vjPosition.h>
#include <Threads/vjThread.h>

#define POSITION_RANGE 12.0f
#define ANGLE_RANGE   180.0f
#define MAX_SENSORS    128

typedef struct {
        float xmin, ymin, zmin;
        float xmax, ymax, zmax;
        float xloc[15][10][15],yloc[15][10][15],zloc[15][10][15];
} CalStruct;

enum BIRD_HEMI  {FRONT_HEM, AFT_HEM, UPPER_HEM, LOWER_HEM, LEFT_HEM, RIGHT_HEM};
enum BIRD_FILT  {AC_NARROW, AC_WIDE, DC_FILTER};
//----------------------------------------------------------------------------
//: Position derived class for running a Flock of Birds.
//
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
//!PUBLIC_API
class vjFlock : public vjPosition {
    public:
        /** @name Construction/Destruction */
	//@{
	vjFlock();
	vjFlock(int sync, int block, int numBrds, int transmit, BIRD_HEMI
		hemi, BIRD_FILT filt, char report, char* calfile);
	~vjFlock();
	//@}

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
	vjMatrix* GetPosData(int dev = 0); // 0 base
		
	
	/** @name vjFlock settings functions
	 *
	 *  Functions for chaning the Flock settings, set functions can only be
	 *  called when the device is not in sampling mode.  These functions
	 *  shouldn't be used by apps using configchunks.
	 */
	//@{
	void SetTransmitter(int Transmit);
	int  GetTransmitter()
                { return theTransmitter; }

	void SetNumBirds(int n);
	int GetNumBirds() { return numBirds; }

	void SetSync(int sync);
	int  GetSync()
                {return syncStyle; }

	void SetBlocking(int blVal);
	int  GetBlocking()
                { return blocking; }

	void SetFilters(BIRD_FILT f);
	int  GetFilters()
                { return filter; }

	void SetHemisphere(BIRD_HEMI h);
	int  GetHemisphere()
                {return hemisphere; }

	void SetReportRate(char rRate);
        char GetReportRate()
                {return repRate; }
	//@}

     private:
	void Position_Correct(float&x,float&y,float&z);
	void InitCorrectionTable(char*);
	vjThread*   myThread;      // The thread doing the flock sampling
	CalStruct caltable;
   int theTransmitter, syncStyle, blocking;
	int numBirds;
	char repRate;
	BIRD_HEMI hemisphere;
	BIRD_FILT filter;
};

#endif

	
