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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GR /GX /Zi /Od /I "." /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /TP /Zm800 /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
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
# Begin Group "Config"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Config\vjChunkDesc.C
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjChunkDesc.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjChunkDescDB.C
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjChunkDescDB.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjConfigChunk.C
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjConfigChunk.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjConfigChunkDB.C
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjConfigChunkDB.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjParseUtil.C
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjParseUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjProperty.C
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjPropertyDesc.C
# End Source File
# Begin Source File

SOURCE=..\..\Config\vjVarValue.h
# End Source File
# End Group
# Begin Group "Memory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\SharedMem\vjMemory.C
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
# End Group
# Begin Group "Sync"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Sync\vjBarrier.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjCond.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjCondGeneric.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjGuard.h
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

SOURCE=..\..\Sync\vjRWMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjSemaphore.h
# End Source File
# Begin Source File

SOURCE=..\..\Sync\vjSemaphoreWin32.h
# End Source File
# End Group
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Math\vjCoord.C
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjCoord.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjMatrix.C
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjQuat.C
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjQuat.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjVec3.C
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjVec3.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjVec4.C
# End Source File
# Begin Source File

SOURCE=..\..\Math\vjVec4.h
# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Input\vjInput\vjAnalog.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjAnalogProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjDigital.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjDigitalProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjDummyAnalog.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjDummyDigital.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjPosition\vjDummyPosition.C
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjPosition\vjDummyPosition.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjInput.C
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjInput.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjInputManager.C
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjInputManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjKeyboardWin32.C
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjInput\vjKeyboardWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjPosition\vjPosition.C
# End Source File
# Begin Source File

SOURCE=..\..\Input\vjPosition\vjPosition.h
# End Source File
# Begin Source File

SOURCE=..\..\Input\InputManager\vjPosProxy.h
# End Source File
# End Group
# Begin Group "Threads"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Threads\vjThread.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadFunctor.C
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadFunctor.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadKeyWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadPool.C
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadPool.h
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadWin32.C
# End Source File
# Begin Source File

SOURCE=..\..\Threads\vjThreadWin32.h
# End Source File
# End Group
# Begin Group "Compatibility"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\config.cpp

!IF  "$(CFG)" == "Juggler - Win32 Release"

!ELSEIF  "$(CFG)" == "Juggler - Win32 Debug"

# ADD CPP /Yc"config.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\strings.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# End Group
# Begin Group "Kernel"

# PROP Default_Filter ""
# Begin Group "Gl"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=..\..\Kernel\vjAPI.C
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjAPI.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjAPIFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjApp.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjCameraProjection.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDebug.C
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjDebug.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjFrustum.C
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjFrustum.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjKernel.C
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjKernel.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjProjection.C
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjProjection.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjSimulator.h
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjStreamLock.C
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjStreamLock.h
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

SOURCE=..\..\Kernel\vjWallProjection.C
# End Source File
# Begin Source File

SOURCE=..\..\Kernel\vjWallProjection.h
# End Source File
# End Group
# End Target
# End Project
