# Microsoft Developer Studio Project File - Name="simplePf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=simplePf - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "simplePf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "simplePf.mak" CFG="simplePf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "simplePf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "simplePf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "simplePf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "$(VJ_BASE_DIR)/include" /I "." /I "..\pfTools" /I "..\..\nav" /I "C:\Program Files\Silicon Graphics\OpenGL Performer\include" /I "C:\Program Files\Silicon Graphics\OpenGL Performer\include\Performer" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_CONSOLE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Juggler.lib Juggler_ogl.lib Juggler_pf.lib Juggler_utils.lib libpf.lib libpfdu-util.lib libpfui.lib comctl32.lib ws2_32.lib opengl32.lib glu32.lib user32.lib gdi32.lib /nologo /subsystem:console /machine:I386 /out:"simplePf.exe" /libpath:"C:\Program Files\Silicon Graphics\OpenGL Performer\lib"

!ELSEIF  "$(CFG)" == "simplePf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /MTd /W3 /Gm /GR /GX /ZI /Od /I "$(VJ_BASE_DIR)/include" /I "." /I "..\pfTools" /I "..\..\nav" /I "C:\Program Files\Silicon Graphics\OpenGL Performer\include" /I "C:\Program Files\Silicon Graphics\OpenGL Performer\include\Performer" /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /YX /FD /GZ /c
# SUBTRACT CPP /nologo
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Juggler.lib Juggler_ogl.lib Juggler_pf.lib Juggler_utils.lib libpf.lib libpfdu-util.lib libpfui.lib comctl32.lib ws2_32.lib opengl32.lib glu32.lib user32.lib gdi32.lib /nologo /subsystem:console /debug /machine:I386 /out:"simplePf.exe" /pdbtype:sept /libpath:"$(VJ_BASE_DIR)/lib/debug" /libpath:"C:\Program Files\Silicon Graphics\OpenGL Performer\Lib\Debug"

!ENDIF 

# Begin Target

# Name "simplePf - Win32 Release"
# Name "simplePf - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=simplePfApp.cpp

!IF  "$(CFG)" == "simplePf - Win32 Release"

!ELSEIF  "$(CFG)" == "simplePf - Win32 Debug"

# ADD CPP /MTd

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\simplePfApp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
