# Microsoft Developer Studio Project File - Name="SongSigDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SongSigDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SongSigDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SongSigDemo.mak" CFG="SongSigDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SongSigDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SongSigDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SongSigDemo - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\example" /I "..\..\src" /I "..\..\..\OTSLibs\glib-1.3" /I "..\..\..\OTSLibs\fftw-2.1.2\fftw" /I "..\..\..\OTSLibs\fftw-2.1.2\rfftw" /I "..\..\src\codecs\mpg123" /I "..\..\src\codecs\wav" /I "..\..\src\codecs\vorbis" /I "..\..\src\codecs\vorbis\vorbis\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 glib-1.3.lib fftw.lib ws2_32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCD" /libpath:"..\..\..\lib\debug\\"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "SongSigDemo - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\example" /I "..\..\src" /I "..\..\..\OTSLibs\glib-1.3" /I "..\..\..\OTSLibs\fftw-2.1.2\fftw" /I "..\..\..\OTSLibs\fftw-2.1.2\rfftw" /I "..\..\src\codecs\mpg123" /I "..\..\src\codecs\wav" /I "..\..\src\codecs\vorbis" /I "..\..\src\codecs\vorbis\vorbis\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 glib-1.3.lib fftw.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBCD" /pdbtype:sept /libpath:"..\..\..\lib\debug\\"

!ENDIF 

# Begin Target

# Name "SongSigDemo - Win32 Release"
# Name "SongSigDemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DirectoryTree.cpp
# End Source File
# Begin Source File

SOURCE=.\SongSigDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\SongSigDemo.rc
# End Source File
# Begin Source File

SOURCE=.\SSDAddSongsThread.cpp
# End Source File
# Begin Source File

SOURCE=.\SSDDirChooserDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SSDDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\SSDMainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\SSDSongDataView.cpp
# End Source File
# Begin Source File

SOURCE=.\SSDSongListView.cpp
# End Source File
# Begin Source File

SOURCE=.\SSDSongSigThread.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DirectoryTree.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SongSigDemo.h
# End Source File
# Begin Source File

SOURCE=.\SSDAddSongsThread.h
# End Source File
# Begin Source File

SOURCE=.\SSDDirChooserDialog.h
# End Source File
# Begin Source File

SOURCE=.\SSDDocument.h
# End Source File
# Begin Source File

SOURCE=.\SSDMainFrame.h
# End Source File
# Begin Source File

SOURCE=.\SSDSongDataView.h
# End Source File
# Begin Source File

SOURCE=.\SSDSongListView.h
# End Source File
# Begin Source File

SOURCE=.\SSDSongSigThread.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\35floppy.ico
# End Source File
# Begin Source File

SOURCE=.\res\Cddrive.ico
# End Source File
# Begin Source File

SOURCE=.\res\Clsdfold.ico
# End Source File
# Begin Source File

SOURCE=.\res\Drive.ico
# End Source File
# Begin Source File

SOURCE=.\res\driveimages.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Drivenet.ico
# End Source File
# Begin Source File

SOURCE=.\res\Openfold.ico
# End Source File
# Begin Source File

SOURCE=.\res\SongSigDemo.rc2
# End Source File
# Begin Source File

SOURCE=.\RES\SSD.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ssdt.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# End Target
# End Project
