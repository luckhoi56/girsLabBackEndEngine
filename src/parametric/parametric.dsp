# Microsoft Developer Studio Project File - Name="parametric" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=parametric - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "parametric.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "parametric.mak" CFG="parametric - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "parametric - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "parametric - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "parametric"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "parametric - Win32 Release"

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
# ADD CPP /nologo /G6 /MT /W3 /GX /Ox /Ot /Op /I "..\general" /I "..\dcdflib" /I "..\ranlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "parametric - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\general" /I "..\dcdflib" /I "..\ranlib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
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

# Name "parametric - Win32 Release"
# Name "parametric - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Beta0595InputMode.cpp
# End Source File
# Begin Source File

SOURCE=.\BetaDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\Gamma0595InputMode.cpp
# End Source File
# Begin Source File

SOURCE=.\GammaDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\LogNormalDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\ParametricDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\ParametricInputMode.cpp
# End Source File
# Begin Source File

SOURCE=.\TrLogNormalDistribution.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Beta0595InputMode.h
# End Source File
# Begin Source File

SOURCE=.\BetaDistribution.h
# End Source File
# Begin Source File

SOURCE=.\Gamma0595InputMode.h
# End Source File
# Begin Source File

SOURCE=.\GammaDistribution.h
# End Source File
# Begin Source File

SOURCE=.\LogNormalDistribution.h
# End Source File
# Begin Source File

SOURCE=.\NormalDistribution.h
# End Source File
# Begin Source File

SOURCE=.\ParametricDistribution.h
# End Source File
# Begin Source File

SOURCE=.\ParametricInputMode.h
# End Source File
# Begin Source File

SOURCE=.\TrLogNormalDistribution.h
# End Source File
# End Group
# End Target
# End Project
