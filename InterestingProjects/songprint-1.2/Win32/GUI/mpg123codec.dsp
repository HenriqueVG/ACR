# Microsoft Developer Studio Project File - Name="mpg123codec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mpg123codec - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mpg123codec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mpg123codec.mak" CFG="mpg123codec - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mpg123codec - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "mpg123codec - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mpg123codec - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\OTSLibs\glib-1.3" /I "..\..\src" /I "..\..\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "mpg123codec - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mpg123codec___Win32_Debug"
# PROP BASE Intermediate_Dir "mpg123codec___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\OTSLibs\glib-1.3" /I "..\..\src" /I "..\..\src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
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

# Name "mpg123codec - Win32 Release"
# Name "mpg123codec - Win32 Debug"
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\common.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\dct64_i386.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\decode_i386.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\huffman.h
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\interface.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\l2tables.h
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\layer1.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\layer2.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\layer3.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\mp3.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\mp3.h
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\mpg123.h
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\mpg123_iface.c
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\mpg123_iface.h
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\mpglib.h
# End Source File
# Begin Source File

SOURCE=..\..\src\codecs\mpg123\tabinit.c
# End Source File
# End Target
# End Project
