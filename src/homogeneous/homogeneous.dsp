# Microsoft Developer Studio Project File - Name="homogeneous" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=homogeneous - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "homogeneous.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "homogeneous.mak" CFG="homogeneous - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "homogeneous - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "homogeneous - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "homogeneous"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "homogeneous - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /Ox /Ot /Op /I "..\parametric" /I "..\general" /I "..\ranlib" /I "..\dcdflib" /I "..\slatec" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "homogeneous - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\parametric" /I "..\general" /I "..\ranlib" /I "..\dcdflib" /I "..\slatec" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
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

# Name "homogeneous - Win32 Release"
# Name "homogeneous - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\LogNormalBinomialDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\LogNormalPoissonDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleHomogeneous.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleUEUpdater.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabularBinomialDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\TabularBinomialSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\TabularDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\TabularIntegrator.cpp
# End Source File
# Begin Source File

SOURCE=.\TabularLogNormalBinomialIntegrator.cpp
# End Source File
# Begin Source File

SOURCE=.\TabularLogNormalIntegrator.cpp
# End Source File
# Begin Source File

SOURCE=.\TabularLogNormalPoissonIntegrator.cpp
# End Source File
# Begin Source File

SOURCE=.\TabularNormalIntegrator.cpp
# End Source File
# Begin Source File

SOURCE=.\TabularPoissonDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\TabularPoissonSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\UniformBinomialDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\UniformPoissonDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdatedTabularDistribution.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\LogNormalBinomialDistribution.h
# End Source File
# Begin Source File

SOURCE=.\LogNormalPoissonDistribution.h
# End Source File
# Begin Source File

SOURCE=.\SimpleHomogeneous.h
# End Source File
# Begin Source File

SOURCE=.\SimpleUEUpdater.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabularBinomialDistribution.h
# End Source File
# Begin Source File

SOURCE=.\TabularBinomialSupport.h
# End Source File
# Begin Source File

SOURCE=.\TabularDistribution.h
# End Source File
# Begin Source File

SOURCE=.\TabularIntegrator.h
# End Source File
# Begin Source File

SOURCE=.\TabularLogNormalBinomialIntegrator.h
# End Source File
# Begin Source File

SOURCE=.\TabularLogNormalIntegrator.h
# End Source File
# Begin Source File

SOURCE=.\TabularLogNormalPoissonIntegrator.h
# End Source File
# Begin Source File

SOURCE=.\TabularNormalIntegrator.h
# End Source File
# Begin Source File

SOURCE=.\TabularPoissonDistribution.h
# End Source File
# Begin Source File

SOURCE=.\TabularPoissonSupport.h
# End Source File
# Begin Source File

SOURCE=.\UniformBinomialDistribution.h
# End Source File
# Begin Source File

SOURCE=.\UniformPoissonDistribution.h
# End Source File
# Begin Source File

SOURCE=.\UpdatedTabularDistribution.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
