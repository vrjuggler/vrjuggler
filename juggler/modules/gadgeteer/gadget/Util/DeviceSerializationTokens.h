#ifndef _GADGET_DEV_SERIALIZATION_TOKENS_H_
#define _GADGET_DEV_SERIALIZATION_TOKENS_H_

namespace gadget
{

namespace tokens
{
   /** Id for the attribute containing a unique data type id */
   const std::string DataTypeAttrib("data_type_id");

   /** Id for the attrib containing the length of the entire sample buffer */
   const std::string SampleBufferLenAttrib("sample_buffer_len");

   /** Id for the Tag of a buffer sample */
   const std::string BufferSampleTag("BufferSample");

   /** Id for the attrib containing the length of the sample */
   const std::string BufferSampleLenAttrib("sample_len");

   const std::string PosValue("PosValue");

   const std::string TimeStamp("timestamp");
}

}

#endif
