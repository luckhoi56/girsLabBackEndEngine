# Microsoft Developer Studio Project File - Name="general" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=general - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "general.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "general.mak" CFG="general - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "general - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "general - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "general"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "general - Win32 Release"

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
# ADD CPP /nologo /G6 /MT /W3 /GX /Ox /Ot /Op /I "..\ranlib" /I "..\dcdflib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "general - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\ranlib" /I "..\dcdflib" /I "..\general" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
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

# Name "general - Win32 Release"
# Name "general - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BetaDistMLEFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\BinnedSampleSpace.cpp
# End Source File
# Begin Source File

SOURCE=.\BinnedSpace.cpp
# End Source File
# Begin Source File

SOURCE=.\BrentModeFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\Distribution.cpp
# End Source File
# Begin Source File

SOURCE=.\DistributionPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\FitParametricDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\Function.cpp
# End Source File
# Begin Source File

SOURCE=.\hooke.cpp
# End Source File
# Begin Source File

SOURCE=.\HookeMinFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\Interpretation.cpp
# End Source File
# Begin Source File

SOURCE=.\InterpretationSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\MetropolisBetaGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MetropolisGammaGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MetropolisGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MetropolisLNGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MetropolisNormalGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MetropolisSampler.cpp
# End Source File
# Begin Source File

SOURCE=.\MetropolisTLNGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MinusFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\NumericalRecipes.cpp
# End Source File
# Begin Source File

SOURCE=.\Parameters.cpp
# End Source File
# Begin Source File

SOURCE=.\ParametricDistributionFitter.cpp
# End Source File
# Begin Source File

SOURCE=.\Sample.cpp
# End Source File
# Begin Source File

SOURCE=.\Sampler.cpp
# End Source File
# Begin Source File

SOURCE=.\SampleSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Session.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpsonIntegrator.cpp
# End Source File
# Begin Source File

SOURCE=.\Status.cpp
# End Source File
# Begin Source File

SOURCE=.\SyncMutex.cpp
# End Source File
# Begin Source File

SOURCE=.\VegasIntegrator.cpp
# End Source File
# Begin Source File

SOURCE=.\ZBrentRootFinder.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BetaDistMLEFunction.h
# End Source File
# Begin Source File

SOURCE=.\BinnedSampleSpace.h
# End Source File
# Begin Source File

SOURCE=.\BinnedSpace.h
# End Source File
# Begin Source File

SOURCE=.\BrentModeFinder.h
# End Source File
# Begin Source File

SOURCE=.\Distribution.h
# End Source File
# Begin Source File

SOURCE=.\DistributionPoint.h
# End Source File
# Begin Source File

SOURCE=.\error.h
# End Source File
# Begin Source File

SOURCE=.\FitParametricDistribution.h
# End Source File
# Begin Source File

SOURCE=.\Function.h
# End Source File
# Begin Source File

SOURCE=.\HookeMinFinder.h
# End Source File
# Begin Source File

SOURCE=.\HyperIndex.h
# End Source File
# Begin Source File

SOURCE=.\Interpretation.h
# End Source File
# Begin Source File

SOURCE=.\InterpretationSet.h
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\MetropolisBetaGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MetropolisGammaGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MetropolisGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MetropolisLNGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MetropolisNormalGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MetropolisSampler.h
# End Source File
# Begin Source File

SOURCE=.\MetropolisTLNGenerator.h
# End Source File
# Begin Source File

SOURCE=.\MinusFunction.h
# End Source File
# Begin Source File

SOURCE=.\ModeFinder.h
# End Source File
# Begin Source File

SOURCE=.\NumericalRecipes.h
# End Source File
# Begin Source File

SOURCE=.\Parameters.h
# End Source File
# Begin Source File

SOURCE=.\ParametricDistributionFitter.h
# End Source File
# Begin Source File

SOURCE=.\Sample.h
# End Source File
# Begin Source File

SOURCE=.\Sampler.h
# End Source File
# Begin Source File

SOURCE=.\SampleSet.h
# End Source File
# Begin Source File

SOURCE=.\Session.h
# End Source File
# Begin Source File

SOURCE=.\SimpsonIntegrator.h
# End Source File
# Begin Source File

SOURCE=.\Status.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SyncMutex.h
# End Source File
# Begin Source File

SOURCE=.\VegasIntegrator.h
# End Source File
# Begin Source File

SOURCE=.\ZBrentRootFinder.h
# End Source File
# End Group
# End Target
# End Project
