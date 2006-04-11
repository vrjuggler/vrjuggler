/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <jccl/jcclConfig.h>
#include <iomanip>

#include <jccl/PerfMonitor/LabeledPerfDataBuffer.h>

namespace jccl
{

LabeledPerfDataBuffer::LabeledPerfDataBuffer ()
{
   mName = "untitled";
   mBufferCount = 5000;
   mBuffer = new BufEntry[mBufferCount];
   mReadBegin = 0;
   mWritePos = 1;
   mLost = 0;
   PerformanceCategories::instance()->addBuffer (this);
}



LabeledPerfDataBuffer::LabeledPerfDataBuffer (const std::string& _name,
                                              int _mBufferCount)
{
   mName = _name;
   mBufferCount = _mBufferCount;
   mBuffer = new BufEntry[mBufferCount];
   mReadBegin = 0;
   mWritePos = 1;
   mLost = 0;
   PerformanceCategories::instance()->addBuffer (this);
}



LabeledPerfDataBuffer::~LabeledPerfDataBuffer ()
{
   PerformanceCategories::instance()->removeBuffer (this);
   delete[] mBuffer;
}



void LabeledPerfDataBuffer::setBufferSize (int n)
{
   delete[] mBuffer;
   mBufferCount = n;
   mBuffer = new BufEntry[mBufferCount];
   mReadBegin = 0;
   mWritePos = 1;
   mLost = 0;
}



void LabeledPerfDataBuffer::set (PerformanceCategory &category,
                                 const std::string& index_name)
{
#ifdef USE_JCCL_PERF
   if (mWritePos == mReadBegin)
   {
      //std::cout << "lost something!!!" << std::endl;
      if (mLostLock.acquire().success())
      {
         mLost++;
         mLostLock.release();
      }
      else
      {
         vprDEBUG(vprDBG_ALL,2)
            << "LabeledPerfDataBuffer: lock acquire "
            << "failed\n" << vprDEBUG_FLUSH;
      }

      int tw = (mWritePos + mBufferCount - 1) % mBufferCount;
      mBuffer[tw].mCategory = &category;
      mBuffer[tw].mIndexString = &index_name;
      mBuffer[tw].mIndexCString = 0;
      mBuffer[tw].mStamp.setNow();
   }
   else
   {
      mBuffer[mWritePos].mCategory = &category;
      mBuffer[mWritePos].mIndexString = &index_name;
      mBuffer[mWritePos].mIndexCString = 0;
      mBuffer[mWritePos].mStamp.setNow();
      mWritePos = (mWritePos+1)%mBufferCount;
   }
#endif
}



void LabeledPerfDataBuffer::set (PerformanceCategory &category,
                                 char* index_name)
{
#ifdef USE_JCCL_PERF
   int tw;

   if (PerformanceCategories::instance()->isCategoryActive (category))
   {
      if (mWritePos == mReadBegin)
      {
         if (mLostLock.acquire().success())
         {
            mLost++;
            mLostLock.release();
         }
         else
         {
            vprDEBUG(vprDBG_ALL,2)
               << "LabeledPerfDataBuffer: lock acquire "
               << "failed\n" << vprDEBUG_FLUSH;
         }
         tw = (mWritePos + mBufferCount - 1) % mBufferCount;
         mBuffer[tw].mCategory = &category;
         mBuffer[tw].mIndexString = 0;
         mBuffer[tw].mIndexCString = index_name;
         mBuffer[tw].mStamp.setNow();
      }
      else
      {
         mBuffer[mWritePos].mCategory = &category;
         mBuffer[mWritePos].mIndexString = 0;
         mBuffer[mWritePos].mIndexCString = index_name;
         mBuffer[mWritePos].mStamp.setNow();
         mWritePos = (mWritePos+1)%mBufferCount;
      }
   }
#endif
}



void LabeledPerfDataBuffer::set (PerformanceCategory &category,
                                 const std::string& index_name,
                                 vpr::Interval& value)
{
#ifdef USE_JCCL_PERF
   int tw;

   if (PerformanceCategories::instance()->isCategoryActive (category))
   {
      if (mWritePos == mReadBegin)
      {
         if (mLostLock.acquire().success())
         {
            mLost++;
            mLostLock.release();
         }
         else
         {
            vprDEBUG(vprDBG_ALL,2)
               << "LabeledPerfDataBuffer: lock acquire "
               << "failed\n" << vprDEBUG_FLUSH;
         }
         tw = (mWritePos + mBufferCount - 1) % mBufferCount;
         mBuffer[tw].mCategory = &category;
         mBuffer[tw].mIndexString = &index_name;
         mBuffer[tw].mIndexCString = 0;
         mBuffer[tw].mStamp = value;
      }
      else
      {
         mBuffer[mWritePos].mCategory = &category;
         mBuffer[mWritePos].mIndexString = &index_name;
         mBuffer[mWritePos].mIndexCString = 0;
         mBuffer[mWritePos].mStamp = value;
         mWritePos = (mWritePos+1)%mBufferCount;
      }
   }
#endif
}



void LabeledPerfDataBuffer::set (PerformanceCategory &category,
                                 char* index_name,
                                 vpr::Interval& value)
{
#ifdef USE_JCCL_PERF
   int tw;

   if (PerformanceCategories::instance()->isCategoryActive (category))
   {

      if (mWritePos == mReadBegin)
      {
         if (mLostLock.acquire().success())
         {
            mLost++;
            mLostLock.release();
         }
         else
         {
            vprDEBUG(vprDBG_ALL,2)
               << "LabeledPerfDataBuffer: lock acquire "
               << "failed\n" << vprDEBUG_FLUSH;
         }
         tw = (mWritePos + mBufferCount - 1) % mBufferCount;
         mBuffer[tw].mCategory = &category;
         mBuffer[tw].mIndexString = 0;
         mBuffer[tw].mIndexCString = index_name;
         mBuffer[tw].mStamp = value;
      }
      else
      {
         mBuffer[mWritePos].mCategory = &category;
         mBuffer[mWritePos].mIndexString = 0;
         mBuffer[mWritePos].mIndexCString = index_name;
         mBuffer[mWritePos].mStamp = value;
         mWritePos = (mWritePos+1)%mBufferCount;
      }
   }
#endif
}



//  void LabeledPerfDataBuffer::setBeginCycle (const vpr::GUID &category) {
//      int tw;

//      if (PerformanceCategories::instance()->isCategoryActive (category)) {

//          if (mWritePos == mReadBegin) {
//              if (mLostLock.acquire().success()) {
//                  lost++;
//                  mLostLock.release();
//              }
//              else
//                  vprDEBUG(vprDBG_ALL,2)
//                      << "LabeledPerfDataBuffer: lock acquire "
//                      << "failed\n" << vprDEBUG_FLUSH;
//              tw = (mWritePos + mBufferCount - 1) % mBufferCount;
//              mBuffer[tw].category = category;
//              mBuffer[tw].index = 0;
//              mBuffer[tw].index_cstring = 0;
//              mBuffer[tw].stamp.setNow();
//          }
//          else {
//              mBuffer[mWritePos].category = category;
//              mBuffer[mWritePos].index = 0;
//              mBuffer[mWritePos].index_cstring = 0;
//              mBuffer[mWritePos].stamp.setNow();
//              mWritePos = (mWritePos+1)%mBufferCount;
//          }
//      }

//  }



// for below: need a version w/ max # mBuffers to write

//: writes mBuffer contents to an ostream
//! POST: As many mBuffers as available are written to
//+       the ostream out and released so they can be
//+       used again by the writer.
//! ARGS: out - an ostream to write contents to.
//! NOTE: The format for a buffer is 'ind timestamp\n',
//+       e.g.: (for four buffers, say we have 3 indices)
//+       <br>1 15
//+       <br>2 25
//+       <br>3 27
//+       <br>1 42
void LabeledPerfDataBuffer::write (std::ostream& out, const std::string& pad)
{
   // the only tricky part of this is that the region we
   // want to print out might wrap back around to the
   // beginning of the list.  That's what the 2nd big
   // case is for.
   int begin, end, i, tlost;
   BufEntry* b;

   //out.width(13);

   begin = mReadBegin;
   end = (mWritePos - 1 + mBufferCount)%mBufferCount;
//     //cout << "begin/end are " << begin <<' '<< end << endl;
   if (begin == end)
   {
      return;
   }

   out << pad << "<labeledbuffer name=\"" << mName << "\" >\n";

   if (begin < end)
   {
      for (i = begin; i < end; i++)
      {
         b = &(mBuffer[i]);
         out << pad << pad << "<stamp label=\"";
         if (b->mIndexString)
         {
            out << b->mIndexString;
         }
         else
         {
            if (b->mIndexCString)
            {
               out << b->mIndexCString;
            }
         }
         out << "\" time=\""
             << std::setiosflags(std::ios::fixed) << b->mStamp.usec() << "\" />\n";

      }
   }
   else
   { /* wraparound */
      for (i = begin; i < mBufferCount; i++)
      {
         b = &(mBuffer[i]);
         out << pad << pad << "<stamp label=\"";
         if (b->mIndexString)
         {
            out << b->mIndexString;
         }
         else {
            if (b->mIndexCString)
            {
               out << b->mIndexCString;
            }
         }
         out << "\" time=\""
             << std::setiosflags(std::ios::fixed) << b->mStamp.usec() << "\" />\n";
      }
      for (i = 0; i < end; i++)
      {
         b = &(mBuffer[i]);
         out << pad << pad << "<stamp label=\"";
         if (b->mIndexString)
         {
            out << b->mIndexString;
         }
         else
         {
            if (b->mIndexCString)
            {
               out << b->mIndexCString;
            }

         }
         out << "\" time=\""
             << std::setiosflags(std::ios::fixed) << b->mStamp.usec() << "\" />\n";
      }
   }

   mLostLock.acquire();
   tlost = mLost;
   mLost = 0;
   mLostLock.release();
   mReadBegin = end;

   //out << -1 << ' ' << tlost << std::endl;
   if (tlost > 0)
   {
      out << pad << pad << "<lost num=\"" << tlost << "\" />\n";
   }

   out << pad << "</labeledbuffer>\n";
}

}
