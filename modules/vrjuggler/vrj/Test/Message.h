#ifndef _VRJ_TEST_MESSAGE_H_
#define _VRJ_TEST_MESSAGE_H_

#include <vector>
#include <string>
#include <stringstream>

namespace vrj::test
{

/* Message with extra details.
*
* Really just a wrapper around a vector of std::strings
* that describe the type of problem encountered
*/
class Message
{
public:
  Message()
  {;}

  explicit Message( const std::string& shortDesc )
      : mShortDesc(shortDesc)
  {
     mShortDesc = shortDesc;
  }

  Message( const std::string &shortDesc,
           const std::string &detail1 )
      : mShortDesc(shortDesc)
  { addDetail(detail1); }

  Message( const std::string &shortDesc,
           const std::string &detail1,
           const std::string &detail2 )
      : mShortDesc(shortDesc)
  { addDetail(detail1, detail2); }

  Message( const std::string &shortDesc,
           const std::string &detail1,
           const std::string &detail2,
           const std::string &detail3 )
  : mShortDesc(shortDesc)
  { addDetail(detail1, detail2, detail3); }

  /*! \brief Returns the short description.
   * \return Short description.
   */
  const std::string& shortDesc() const
  { return mShortDesc; }

  /** Returns a string that represents a list of the detail strings.
   *
   * Example:
   * \code
   * Message message( "not equal", "Expected: 3", "Actual: 7" );
   * std::string details = message.details();
   * // details contains:
   * // "- Expected: 3\n- Actual: 7\n"  \endcode
   *
   * @return A string that is a concatenation of all the detail strings. Each detail
   *         string is prefixed with '- ' and suffixed with '\n' before being
   *         concatenated to the other.
   */
  std::string details() const
  {
     std::stringstream oss;

     for(unsigned i=0;i<mDetails.size();i++)
        oss << "- " << mDetails[i] << "\n";

     return oss.str();
  }

  /*! \brief Removes all detail strings.
   */
  void clearDetails()
  {  mDetails.clear(); }

  /** Return a list of the details */
  std::vector<std::string> getDetails()
  { return mDetails;}

  /** Add a detail */
  void addDetail( const std::string &detail )
  { mDetails.push_back(detail); }

  /** Add two details */
  void addDetail( const std::string& detail1,
                  const std::string& detail2 )
  { addDetail(detail1); addDetail(detail2); }

  /** Add three details */
  void addDetail( const std::string &detail1,
                  const std::string &detail2,
                  const std::string &detail3 )
  { addDetail(detail1); addDetail(detail2); addDetail(detail3); }

  /** Set the short descriptio */
  void setShortDesc( const std::string& shortDesc )
  { mShortDesc = shortDesc; }

private:
  std::string              mShortDesc;    /**< The short description of the message */
  std::vector<std::string> mDetails;      /**< The associated details */
};

}

#endif


