# Microsoft Developer Studio Project File - Name="Juggler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Juggler - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Juggler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Juggler.mak" CFG="Juggler - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Juggler - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Juggler - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Juggler - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Juggler - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "f:/source/juggler" /I "f:/source/juggler/lib/win32" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Juggler - Win32 Release"
# Name "Juggler - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\Input\InputManager\vjAnalogProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjAPI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjBarrier.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjBaseThread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjChunkDesc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjChunkDescDB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjChunkFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjCondGeneric.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjConfigChunk.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjConfigChunkDB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjCoord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDebug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjDeviceFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjDeviceInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjDigitalProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDisplayManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDrawManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjGesture\vjDummyGesture.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjGlove\vjDummyGlove.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjPosition\vjDummyPosition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjFrustum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjGesture\vjGesture.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjGestureProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlDrawManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjGlove\vjGlove.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjGlove\vjGlove.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjGesture\vjGloveGesture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjGesture\vjGloveGesture.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjGloveProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlPipe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlWinWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjInputManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjKernel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjKeyboardProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjKeyboard\vjKeyboardWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SharedMem\vjMemory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SharedMem\vjMemPoolWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjParseUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Performance\vjPerfDataBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjPlane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjPosInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjPosition\vjPosition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjPosProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjProjection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjPropertyDesc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjProxyFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjQuat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjRWMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjSeg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimAnalog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimDigital.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimGloveGesture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimPosition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjStreamLock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadFunctor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Environment\vjTimedUpdate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Performance\vjTimeStamp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjTSObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjTSTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjUser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjVarValue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjVec3.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjVec4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjWallProjection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjWin32SystemFactory.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Input\vjInput\vjAnalog.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjAnalogInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjAnalogProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjAPI.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjApp.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjBarrier.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjBaseThread.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjCameraProjection.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjChunkDesc.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjChunkDescDB.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjChunkFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjCond.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjCondGeneric.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjCondPosix.h
# End Source File
# Begin Source File

SOURCE=..\..\vjConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjConfigChunk.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjConfigChunkDB.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjConfigChunkHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjCoord.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDebug.h
# End Source File
# Begin Source File

SOURCE=.\vjDefines.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjDeviceFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjDeviceInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjDigital.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjDigitalInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjDigitalProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDisplay.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDisplayManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDrawManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjDummyAnalog.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjDummyDigital.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjKeyboard\vjDummyKeyboard.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjPosition\vjDummyPosition.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjEnumEntry.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjFrustum.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjGestureInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjGestureProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlApp.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlContextData.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlDrawManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjGloveInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjGloveProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlPipe.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlUserData.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\GL\vjGlWinWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjGuard.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjGuardedQueue.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjInput.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjInputManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjKernel.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjKeyboard.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjKeyboardInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjKeyboardProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjKeyboard\vjKeyboardWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\SharedMem\vjMemory.h
# End Source File
# Begin Source File

SOURCE=..\..\SharedMem\vjMemPool.h
# End Source File
# Begin Source File

SOURCE=..\..\SharedMem\vjMemPool_alloc.h
# End Source File
# Begin Source File

SOURCE=..\..\SharedMem\vjMemPool_malloc.h
# End Source File
# Begin Source File

SOURCE=..\..\SharedMem\vjMemPoolHeap.h
# End Source File
# Begin Source File

SOURCE=..\..\SharedMem\vjMemPoolWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjMutexWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjNullMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\Performance\vjNullPerfDataBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjParseUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\Performance\vjPerfDataBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjPosInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjPosition\vjPosition.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjPosProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjProjection.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjProperty.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjPropertyDesc.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjProxyFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjQuat.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjRWMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjSeg.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjSemaphore.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjSemaphoreWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimAnalog.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimDigital.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjSimDisplay.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimGloveGesture.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimInput.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjSim\vjSimPosition.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjStreamLock.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjSurfaceDisplay.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjSystem.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjSystemData.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjSystemFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThread.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadFunctor.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadKeyWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Environment\vjTimedUpdate.h
# End Source File
# Begin Source File

SOURCE=..\..\Performance\vjTimeStamp.h
# End Source File
# Begin Source File

SOURCE=..\..\Performance\vjTimeStampNone.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjTSObject.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjTSObjectProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjTSTable.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjUser.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjVec3.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjVec4.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjWallProjection.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjWin32SystemFactory.h
# End Source File
# End Group
# End Target
# End Project
