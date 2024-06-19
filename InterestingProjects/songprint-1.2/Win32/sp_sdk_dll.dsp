# Microsoft Developer Studio Project File - Name="sp_sdk_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=sp_sdk_dll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sp_sdk_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sp_sdk_dll.mak" CFG="sp_sdk_dll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sp_sdk_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "sp_sdk_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sp_sdk_dll - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SP_SDK_DLL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../win32" /I "../Common" /I "../../OTSLibs/glib-1.3" /I "../../OTSLibs/fftw-2.1.2/fftw" /I "../../OTSLibs/fftw-2.1.2/rfftw" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 glib-1.3.lib Ws2_32.lib fftw.lib /nologo /dll /machine:I386 /out:"Release\songprint.dll" /implib:"Release/songprint_exports"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "sp_sdk_dll - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SP_SDK_DLL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../win32" /I "../Common" /I "../../OTSLibs/glib-1.3" /I "../../OTSLibs/fftw-2.1.2/fftw" /I "../../OTSLibs/fftw-2.1.2/rfftw" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 glib-1.3.lib Ws2_32.lib fftw.lib /nologo /dll /debug /machine:I386 /out:"Debug/songprint.dll" /implib:"Debug/songprint_exports.lib" /pdbtype:sept /libpath:".\Debug\Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "sp_sdk_dll - Win32 Release"
# Name "sp_sdk_dll - Win32 Debug"
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
