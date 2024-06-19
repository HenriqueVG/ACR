# Microsoft Developer Studio Project File - Name="sp_sdk_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=sp_sdk_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sp_sdk_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sp_sdk_lib.mak" CFG="sp_sdk_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sp_sdk_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "sp_sdk_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sp_sdk_lib - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../win32" /I "../Common" /I "../../OTSLibs/glib-1.3" /I "../../OTSLibs/fftw-2.1.2/fftw" /I "../../OTSLibs/fftw-2.1.2/rfftw" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\songprint.lib"

!ELSEIF  "$(CFG)" == "sp_sdk_lib - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../win32" /I "../Common" /I "../../OTSLibs/glib-1.3" /I "../../OTSLibs/fftw-2.1.2/fftw" /I "../../OTSLibs/fftw-2.1.2/rfftw" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\songprint.lib"

!ENDIF 

# Begin Target

# Name "sp_sdk_lib - Win32 Release"
# Name "sp_sdk_lib - Win32 Debug"
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=..\src\sig_tables.c
# End Source File
# Begin Source File

SOURCE=..\src\sig_tables.h
# End Source File
# Begin Source File

SOURCE=..\src\songprint.h
# End Source File
# Begin Source File

SOURCE=..\src\sp_id_t.c
# End Source File
# Begin Source File

SOURCE=..\src\sp_identify.c
# End Source File
# Begin Source File

SOURCE=..\src\sp_identify.h
# End Source File
# Begin Source File

SOURCE=..\src\sp_signature.c
# End Source File
# Begin Source File

SOURCE=..\src\sp_signature.h
# End Source File
# Begin Source File

SOURCE=..\src\sp_types.h
# End Source File
# End Target
# End Project
