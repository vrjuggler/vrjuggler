/////////////////////////////////////////////////////////////////////////
// vjBird.h
//
// Standaolne bird tracking class
//
// Note: untested with a standalone configuration, probably doesn't work
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_BIRD_H_
#define _VJ_BIRD_H_

#include <vjConfig.h>
#include <Input/vjPosition/vjPosition.h>
#include <Threads/vjThread.h>
#include <Input/vjPosition/vjFlock.h>

//: A start at a standalone bird derice driver.
//
//  vjBird is based off of vjFlock, but for one bird only, the
//  group mode things have been stripped, and should be close to
//  a working implementation, but has not been tested a standalone
//  bird.
class vjBird : public vjPosition
{
public:
   /** @name Construction/Destruction */
   //@{
   vjBird();
   ~vjBird();
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
   char* GetDeviceName() { return "vjBird";}
   //@}

   /** @name vjPosition pure virtual functions
    *
    *  pure virtual functions required by vjPosition
    */
   //@{
   //old function
   //void GetData(vjPOS_DATA* &data);
   // XXX: Bad things live here
   vjMatrix* GetPosData(int dev = 0); // 0 base
   //@}


   /** @name vjBird settings functions
    *
    *  Functions for chaning the Bird settings, set functions can only be
    *  called when the device is not in sampling mode.  These functions
    *  shouldn't be used by apps using configchunks.
    */
   //@{
   void SetSync(int sync);
   int  GetSync()
   {return syncStyle;}

   void SetBlocking(int blVal);
   int  GetBlocking()
   { return blocking;}

   void SetFilters(BIRD_FILT f);
   int  GetFilters()
   { return filter;}

   void SetHemisphere(BIRD_HEMI h);
   int  GetHemisphere()
   {return hemisphere;}

   void SetReportRate(char rRate);
   char GetReportRate()
   {return repRate;}
   //@}

private:
   void Position_Correct(float&x,float&y,float&z);
   void InitCorrectionTable();
   vjThread*   myThread;      // ptr to our thread object
   CalStruct   caltable;
   int  syncStyle, blocking;
   char repRate;
   BIRD_HEMI hemisphere;
   BIRD_FILT filter;
};

#endif

	
