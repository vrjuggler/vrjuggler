/////////////////////////////////////////////////////////////////////////
// aFlock.h
//
// Flock of birds tracking class
//
////////////////////////////////////////////////////////////////////////
#ifndef _ASCENSION_FLOCKOFBIRD_H_
#define _ASCENSION_FLOCKOFBIRD_H_

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
//: class for running a Flock of Birds.
//
//  aFlock is a positional device driver for the Flock of Birds, the config
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
class aFlock
{
public:
	//: Configure Constructor
	// Give:
	//   port - such as "/dev/ttyd3"
	//   baud - such as 38400, 19200, 9600, 14400, etc...
	//   sync - sync type.
	//   block - blocking
	//   numBrds - number of birds in flock,
	//   transmit - transmitter unit number,
	//   hemi - hemisphere to track from,
	//   filt - filtering type,
	//   report -
	//   calfile - a calibration file, if "", then use none.
	// 
	// Result: configures internal data members, 
	//         doesn't actually talk to the FOB yet.
	aFlock(const char* const port = "/dev/ttyd3", 
		const int& baud = 38400, 
		const int& sync = 1, 
		const int& block = 0, 
		const int& numBrds = 3, 
		const int& transmit = 3, 
		const BIRD_HEMI& hemi = LOWER_HEM, 
		const BIRD_FILT& filt = AC_NARROW, 
		const char& report = 'R', 
		const char* const calfile = "");
	~aFlock();

	
	int	    StartSampling();
	int	    StopSampling();
	int	    Sample();
	void	    UpdateData();
	
	vjMatrix&   GetMatrix( int dev = 0 );
		
	void	    SetPort( const char* const serialPort );
	const char* const GetPort();
	
	void	    SetBaudRate( const int& baud );
	inline int  GetBaudRate() { return baudRate;}
   
	void	    SetTransmitter( const int& Transmit );
	inline int  GetTransmitter() { return theTransmitter; }

	void	    SetNumBirds( const int& n );
	inline int  GetNumBirds() { return numBirds; }

	void	    SetSync( const int& sync );
	inline int  GetSync() {return syncStyle; }

	void	    SetBlocking( const int& blVal );
	inline int  GetBlocking() { return blocking; }

	void	    SetFilters( const BIRD_FILT& f );
	inline int  GetFilters() { return filter; }

	void	    SetHemisphere( const BIRD_HEMI& h );
	inline int  GetHemisphere() {return hemisphere; }

	void	    SetReportRate( const char& rRate );
        inline char GetReportRate() {return repRate; }

private:
	void        Position_Correct( float& x, float& y, float& z );
	void        InitCorrectionTable( const char* const );
	CalStruct   caltable;
	int         theTransmitter, syncStyle, blocking;
	int         numBirds;
	char        repRate;
	BIRD_HEMI   hemisphere;
	BIRD_FILT   filter;
};


inline void vjInput::SetPort(const char* serialPort)
{
    if (myThread != NULL) {
     cerr << "Cannot change the serial Port while active\n";
     return;
  }
  strncpy(sPort,serialPort,(size_t)30);
}

inline char* vjInput::GetPort()
{
  if (sPort == NULL) return "No port";
  return sPort;
}

inline void vjInput::SetBaudRate(int baud)
{
  if (myThread != NULL)
     baudRate = baud;
}

#endif

	
