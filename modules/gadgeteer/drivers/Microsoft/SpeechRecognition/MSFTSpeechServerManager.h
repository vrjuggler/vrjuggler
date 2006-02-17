/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef MSFT_SPEECH_SERVER_MANAGER_H
#define MSFT_SPEECH_SERVER_MANAGER_H

#include <sapi.h>
#include <sphelper.h>
#include <string>
#include <vector>
#include <map>
#include <vpr/Util/Singleton.h>
#include <vpr/Sync/Semaphore.h>
#include <vpr/Thread/Thread.h>
#include <gadget/Type/Input.h>

SPNOTIFYCALLBACK NotifyCallbackFunction;

class MSFTSpeechServerManager : public vpr::Singleton<MSFTSpeechServerManager>
{
public:
   friend class vpr::Singleton<MSFTSpeechServerManager>;

   std::string getStringState(gadget::Input* RefDevice)
   {
      std::map<gadget::Input*, ReferencedDevice>::iterator RefDeviceIter;
      RefDeviceIter = mDeviceMap.find( RefDevice );
      if( RefDeviceIter == mDeviceMap.end() )
      {
         //Not listening for The Grammar that this phrase was recognized in
         return std::string("");
      }
      else
      {
         (*RefDeviceIter).second.mHasRecentString = true;
         return (*RefDeviceIter).second.mStringState;
      }
   }

   int getIntegerState(gadget::Input* RefDevice)
   {
      std::map<gadget::Input*, ReferencedDevice>::iterator RefDeviceIter;
      RefDeviceIter = mDeviceMap.find( RefDevice );
      if( RefDeviceIter == mDeviceMap.end() )
      {
         //Not listening for The Grammar that this phrase was recognized in
         return 0;
      }
      else
      {
         (*RefDeviceIter).second.mHasRecentInt = true;
         return (*RefDeviceIter).second.mIntState;
      }
   }
   
   void resetData()
   {
      mDataMutex.acquire();
      std::map<gadget::Input*, ReferencedDevice>::iterator RefDeviceIter;
      for(RefDeviceIter = mDeviceMap.begin();
          RefDeviceIter != mDeviceMap.end();
          ++RefDeviceIter)
      {
         if( (*RefDeviceIter).second.mHasRecentString )
         {
            (*RefDeviceIter).second.mHasRecentString = false;
            (*RefDeviceIter).second.mStringState = "";
         }
         if( (*RefDeviceIter).second.mHasRecentInt )
         {
            (*RefDeviceIter).second.mHasRecentInt = false;
            (*RefDeviceIter).second.mIntState = 0;
         }
      }
      std::map<std::string, Grammar*>::iterator GrammarNameMapIter;
      for(GrammarNameMapIter = mGrammarNameMap.begin();
          GrammarNameMapIter != mGrammarNameMap.end();
          ++GrammarNameMapIter)
      {
         (*GrammarNameMapIter).second->mIntState = 0;
         (*GrammarNameMapIter).second->mStringState = "";
      }
      mDataMutex.release();
   }

   /** Main thread of control for this active object. */
   void updateLoop(void* nullParam);

   /** Checks if the device is active. */
   bool isActive()
   {
      return mReferenceLevel > 0;
   }
   
   //The NotifyCallbackFunction
   friend SPNOTIFYCALLBACK NotifyCallbackFunction;

   bool startUpdating();
   bool stopUpdating();

   bool addGrammar(const std::string& GrammarFileName, gadget::Input* DevicePtr);

private:
   struct Grammar
   {
      ULONGLONG mID;
      std::string mFileName;
      int mIntState;
      std::string mStringState;
      CComPtr<ISpRecoGrammar> mcpGrammar;
      bool mIsLoaded;
   };

   struct ReferencedDevice
   {
      Grammar *mGrammar;
      std::string mStringState;
      int mIntState;
      bool mHasRecentString;
      bool mHasRecentInt;
   };

   MSFTSpeechServerManager();
   ~MSFTSpeechServerManager();
   
   void UpdateRecoEvent();
   void RecoUpdate(ISpPhrase *pPhrase);
   bool loadGrammar();
   
   bool Initialize();
   void Uninitialize();
   void Update();

   MSG mMSG;
   CComPtr<ISpRecognizer> cpEngine; /**< Pointer to our recognition engine instance */
   CComPtr<ISpRecoContext> cpContext;
   CComPtr<ISpRecoGrammar> cpGrammar;
   //CComPtr<ISpVoice> cpVoice;

   bool mIsActive;
   bool isInitailized;

   std::map<gadget::Input*, ReferencedDevice> mDeviceMap;
   std::map<std::string, Grammar*> mGrammarNameMap;
   std::map<ULONGLONG, Grammar*> mGrammarIDMap;
   ULONGLONG mGrammarIDGen;
   int mReferenceLevel;

   vpr::Thread* mUpdateThread;

   vpr::Semaphore mReferenceSem;
   vpr::Mutex mDataMutex;

   void SpeakString(std::string TheString);
};

#endif
