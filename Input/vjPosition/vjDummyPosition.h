// vjDummyPosition.h

#ifndef _VJ_DUMMYTRACKER_H_
#define _VJ_DUMMYTRACKER_H_

#include <vjConfig.h>
#include <Input/vjPosition/vjPosition.h>

//: Position derived class for impersonating a position device.
//
// vjDummyPosition is a dummy positional device for holding semi-static
// device data.
class vjDummyPosition: public vjPosition {

  public:
	/** @name Construction/Destruction */
	//@{
	vjDummyPosition(vjConfigChunk *c);
	vjDummyPosition() { active = 1;
			    instName = NULL;}
	~vjDummyPosition();
	//@}

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
	char* GetDeviceName() { return "vjDummyPosition"; }
	//@}
	
	/** @name vjPosition pure virtual functions
	 *
	 *  pure virtual functions required by vjPosition
	 */
	//@{
	vjMatrix* GetPosData (int d = 0);
	//@}
	
	//: Function to get access to the position matrix
   vjMatrix& posData()
   {
      return mydata;
   }

  private:
   vjMatrix mydata;     // Location data
	
};

#endif
