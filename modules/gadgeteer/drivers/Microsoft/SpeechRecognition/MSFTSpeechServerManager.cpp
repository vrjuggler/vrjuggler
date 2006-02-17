/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <gadget/Devices/DriverConfig.h>
#include <wchar.h>
#include <boost/bind.hpp>
#include <drivers/Microsoft/SpeechRecognition/MSFTSpeechServerManager.h>

#include <gadget/Util/Debug.h>

#define WM_RECOEVENT WM_APP

MSFTSpeechServerManager::MSFTSpeechServerManager()
   : mIsActive(false)
   , isInitailized(false)
   , mGrammarIDGen(0)
   , mReferenceLevel(0)
{
}

MSFTSpeechServerManager::~MSFTSpeechServerManager()
{
}

bool MSFTSpeechServerManager::Initialize()//(HWND hwnd, char* TheServer)
{
   if(isInitailized)
   {
      return true;
   }

   HRESULT hr = S_OK;
   // Initialize COM
   ::CoInitialize(NULL);

   while ( true )
   {
      // create a recognition engine
      hr = cpEngine.CoCreateInstance(CLSID_SpSharedRecognizer);
      if ( FAILED( hr ) )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "cpEngine.CoCreateInstance(CLSID_SpSharedRecognizer) Failed"
            << std::endl << vprDEBUG_FLUSH;
         break;
      }

      // create the command recognition context
      hr = cpEngine->CreateRecoContext( &cpContext );
      if ( FAILED( hr ) )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "cpEngine->CreateRecoContext( &cpContext ) Failed" << std::endl
            << vprDEBUG_FLUSH;
         break;
      }

      //Try to use a callback function instead of a window event
      hr = cpContext->SetNotifyCallbackFunction(  /*Function Pointer*/NotifyCallbackFunction, 0, 0 );
      if ( FAILED( hr ) )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "cpContext->SetNotifyCallbackFunction(  /*Function Pointer*/NotifyCallbackFunction, 0, 0 ) Failed"
            << std::endl << vprDEBUG_FLUSH;
         break;
      }

      // Tell SR what types of events interest us.  Here we only care about
      // command recognition.
      hr = cpContext->SetInterest( SPFEI(SPEI_RECOGNITION),
                                   SPFEI(SPEI_RECOGNITION) );
      if ( FAILED( hr ) )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "cpContext->SetInterest( SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION) ) Failed"
            << std::endl << vprDEBUG_FLUSH;
         break;
      }

      //Load all of the grammars
      loadGrammar();

      //Set Audio options and setup Voice
      hr = cpContext->SetAudioOptions(SPAO_NONE, NULL, NULL);
      if ( FAILED( hr ) )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "cpContext->SetAudioOptions(SPAO_NONE, NULL, NULL) Failed"
            << std::endl << vprDEBUG_FLUSH;
         break;
      }

      //Get a voice instance
/*
      hr = cpContext->GetVoice(&cpVoice);
      if ( FAILED( hr ) )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "cpContext->GetVoice(&cpVoice) Failed" << std::endl
            << vprDEBUG_FLUSH;
         break;
      }
*/
      break;
   }

   // if we failed and have a partially setup SAPI, close it all down
   if ( FAILED( hr ) )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "Speech Recognition Initialization Failed." << std::endl
         << vprDEBUG_FLUSH;
      Uninitialize();
      return false;
   }

   //cpVoice->Speak(L"Welcome to Speech Recognition Interface", SPF_DEFAULT, NULL);

   isInitailized = true;
   return true;
}

void MSFTSpeechServerManager::Uninitialize()
{
   if(!isInitailized)
   {
      return;
   }

   // Release grammar, if loaded
   if ( cpGrammar )
   {
      cpGrammar.Release();
   }
   // Release recognition context, if created
   if ( cpContext )
   {
      cpContext->SetNotifySink(NULL);
      cpContext.Release();
   }
   // Release recognition engine instance, if created
   if ( cpEngine )
   {
      cpEngine.Release();
   }
   // Release Voice instance, if created
   //if ( cpVoice )
   //{
   //	cpVoice.Release();
   //}
   ::CoUninitialize();	// Uninitialize COM
   isInitailized = false;
}

void MSFTSpeechServerManager::UpdateRecoEvent()
{
   CSpEvent event;  // Event helper class

   // Loop processing events while there are any in the queue
   while (event.GetFrom(cpContext) == S_OK)
   {
      // Look at recognition event only
      switch (event.eEventId)
      {
         case SPEI_RECOGNITION:
            RecoUpdate(event.RecoResult());
            break;
         case SPEI_PHRASE_START:
            break;
         case SPEI_FALSE_RECOGNITION:
            break;
      }
   }
}

void MSFTSpeechServerManager::RecoUpdate(ISpPhrase* pPhrase)
{
   std::string Data;
   std::string speak;

   SPPHRASE *pSPPhrase;
   //Determine the GrammarID of the Recoginition
   std::map<ULONGLONG, Grammar*>::iterator MapIDIter;

   if (SUCCEEDED(pPhrase->GetPhrase(&pSPPhrase)))
   {
      /////////////////////////////////////////////////
      //if this statement isn't true there is an error
      //somewhere in the state transitioning
      /////////////////////////////////////////////////

      mDataMutex.acquire();
      //Determine if the GrammarID of this phrase corresponds to the Grammars
      //that I have
      MapIDIter = mGrammarIDMap.find( pSPPhrase->ullGrammarID );
      if( MapIDIter == mGrammarIDMap.end() )
      {
         //Not listening for The Grammar that this phrase was recognized in
         mDataMutex.release();
         return;
      }

      //Update the Integer state of the Grammar
      (*MapIDIter).second->mIntState = pSPPhrase->Rule.ulId;
      CoTaskMemFree(pSPPhrase);

      //Update the String state of the Grammar
      WCHAR *phraseText;
      if (SUCCEEDED(pPhrase->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &phraseText, NULL)))
      {
         /////////////////////////////////////////////////
         //if this statement isn't true there is an error
         //somewhere in the state transitioning
         /////////////////////////////////////////////////

         char *tempC = new char[ wcslen(phraseText)+1 ];
         wcstombs( tempC, phraseText, wcslen(phraseText)+1 );
         (*MapIDIter).second->mStringState = tempC;

         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "Recognized " << (*MapIDIter).second->mStringState << std::endl
            << vprDEBUG_FLUSH;

         CoTaskMemFree(phraseText);
      }

      //Update the ReferenceDevices Data
      std::map<gadget::Input*, ReferencedDevice>::iterator RefDeviceIter;
      for(RefDeviceIter = mDeviceMap.begin();
          RefDeviceIter != mDeviceMap.end();
          ++RefDeviceIter)
      {
         if( (*RefDeviceIter).second.mGrammar == (*MapIDIter).second )
         {
            (*RefDeviceIter).second.mHasRecentInt = false;
            (*RefDeviceIter).second.mHasRecentString = false;
            (*RefDeviceIter).second.mIntState = (*MapIDIter).second->mIntState;
            (*RefDeviceIter).second.mStringState = (*MapIDIter).second->mStringState;
         }
      }

      mDataMutex.release();
   }
}

bool MSFTSpeechServerManager::loadGrammar()
{
   // Load our grammar
   HRESULT hr = S_OK;

   std::map<std::string, Grammar*>::iterator GramIter;
   for(GramIter = mGrammarNameMap.begin() ; GramIter != mGrammarNameMap.end() ; ++GramIter)
   {
      if( (*GramIter).second->mIsLoaded == false )
      {
         hr = cpContext->CreateGrammar((*GramIter).second->mID,
                                       &(*GramIter).second->mcpGrammar);

         if (FAILED(hr))
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "cpContext->CreateGrammar((*GramIter).second->mFileName->mID, &cpGrammar) Failed"
               << std::endl << vprDEBUG_FLUSH;
            return false;
         }

         WCHAR *tempWC;
         tempWC = new WCHAR[_mbstrlen((*GramIter).second->mFileName.c_str())+1];
         mbstowcs( tempWC, (*GramIter).second->mFileName.c_str(),
                   _mbstrlen((*GramIter).second->mFileName.c_str()) );
         tempWC[_mbstrlen((*GramIter).second->mFileName.c_str())] = L'\0';

         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "Loading Grammar File: " << (*GramIter).second->mFileName
            << std::endl << vprDEBUG_FLUSH;

         hr = (*GramIter).second->mcpGrammar->LoadCmdFromFile(tempWC,
                                                              SPLO_STATIC);

         if ( FAILED( hr ) )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "cpGrammar->LoadCmdFromFile(tempWC, SPLO_STATIC) Failed"
               << std::endl << vprDEBUG_FLUSH;
            return false;
         }

         // Set rules to active, we are now listening for commands
         hr = (*GramIter).second->mcpGrammar->SetRuleState(NULL, NULL,
                                                           SPRS_ACTIVE );
         if ( FAILED( hr ) )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "cpGrammar->SetRuleState(NULL, NULL, SPRS_ACTIVE ) Failed"
               << std::endl << vprDEBUG_FLUSH;
            return false;
         }
         (*GramIter).second->mIsLoaded = true;
      }
   }
}

bool MSFTSpeechServerManager::addGrammar(const std::string& GrammarFileName,
                                         gadget::Input* DevicePtr)
{
   if( mGrammarNameMap.find(GrammarFileName) != mGrammarNameMap.end() )
   {
      return true;
   }
   else
   {
      mGrammarIDGen = mGrammarIDGen + 1;

      Grammar* tGram = new Grammar;
      tGram->mFileName = GrammarFileName;
      tGram->mID = mGrammarIDGen;
      tGram->mIntState = 0;
      tGram->mStringState = "";
      tGram->mIsLoaded = false;

      mGrammarNameMap[tGram->mFileName] = tGram;
      mGrammarIDMap[tGram->mID] = tGram;

      if(isInitailized)
      {
         loadGrammar();
      }

      if( mDeviceMap.find(DevicePtr) != mDeviceMap.end() )
      {
         return true;
      }
      else
      {
         ReferencedDevice tRefDevice;
         tRefDevice.mGrammar = tGram;
         tRefDevice.mStringState = "";
         tRefDevice.mIntState = 0;
         mDeviceMap[DevicePtr] = tRefDevice;
      }

      return true;
   }
}

void MSFTSpeechServerManager::SpeakString(std::string TheString)
{
   wchar_t* toSpeak;

   unsigned int i;
   toSpeak = new wchar_t[TheString.size()+1];
   for(i = 0; i < TheString.size(); ++i)
   {
      toSpeak[i] = TheString[i];
   }
   toSpeak[i] = '\0';

   //cpVoice->Speak(toSpeak, SPF_DEFAULT, NULL);
   delete toSpeak;
}

void MSFTSpeechServerManager::updateLoop()
{
   //Initialize the Speech Manager
   if(!Initialize())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MSFTSpeechRecog Speech Manager did not initialize."
         << std::endl << vprDEBUG_FLUSH;
      return;
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::MSFTSpeechRecog Speech Manager thread initialized."
         << std::endl << vprDEBUG_FLUSH;
   }

   //Keep updating
   while ( isActive() )
   {
      Update();
   }

   Uninitialize();

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "gadget::MSFTSpeechServerManager update thread Exiting." << std::endl
      << vprDEBUG_FLUSH;
}

bool MSFTSpeechServerManager::startUpdating()
{
   mReferenceSem.acquire();
   mReferenceLevel++;
   mReferenceSem.release();

   if(mReferenceLevel > 1)
   {
      return true;
   }

   // Create a new thread to handle the sampling control
   mUpdateThread =
      new vpr::Thread(boost::bind(&MSFTSpeechServerManager::updateLoop, this));

   if ( ! mUpdateThread->valid() )
   {
      return false;  // Fail
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::MSFTSpeechServerManager update thread created."
         << std::endl << vprDEBUG_FLUSH;
         return true;
   }
}

bool MSFTSpeechServerManager::stopUpdating()
{
   mReferenceSem.acquire();
   mReferenceLevel--;
   mReferenceSem.release();
   if(mReferenceLevel == 0)
   {
      mUpdateThread->join();
      delete mUpdateThread;
      mUpdateThread = NULL;
   }
   return (mReferenceLevel == 0);
}

void MSFTSpeechServerManager::Update()
{
   if(GetMessage(&mMSG, NULL, 0, 0))
   {
      TranslateMessage(&mMSG);
      DispatchMessage(&mMSG);
   }
}

void __stdcall NotifyCallbackFunction(WPARAM wParam, LPARAM lParam)
{
   MSFTSpeechServerManager::instance()->UpdateRecoEvent();
}
