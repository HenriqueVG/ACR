# Microsoft Developer Studio Project File - Name="vorbiscodec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=vorbiscodec - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vorbiscodec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vorbiscodec.mak" CFG="vorbiscodec - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vorbiscodec - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "vorbiscodec - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vorbiscodec - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "..\..\src\codecs\vorbis\vorbis\include" /I "..\..\..\OTSLibs\glib-1.3" /I "..\..\src" /I "..\..\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "vorbiscodec - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\src\codecs\vorbis\vorbis\include" /I "..\..\..\OTSLibs\glib-1.3" /I "..\..\src" /I "..\..\src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "HAVE_STRING_H" /YX /FD /GZ /c
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

# Name "vorbiscodec - Win32 Release"
# Name "vorbiscodec - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\analysis.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\bitwise.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\block.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\codebook.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\envelope.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\floor0.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\framing.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\iir.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\info.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\lpc.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\lsp.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\mapping0.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\mdct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\psy.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\registry.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\res0.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\sharedbook.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\smallft.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\synthesis.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\time0.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis_iface.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\vorbisfile.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis\lib\window.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\codecs\vorbis\vorbis_iface.h
# End Source File
# End Group
# End Target
# End Project
