# Microsoft Developer Studio Project File - Name="nonhomog" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=nonhomog - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "nonhomog.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "nonhomog.mak" CFG="nonhomog - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nonhomog - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "nonhomog - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "nonhomog"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nonhomog - Win32 Release"

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
# ADD CPP /nologo /G6 /MT /W3 /GX /Ox /Ot /Op /I "..\general" /I "..\dcdflib" /I "..\parametric" /I "..\homogeneous" /I "..\ranlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "nonhomog - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\ranlib" /I "..\general" /I "..\dcdflib" /I "..\parametric" /I "..\homogeneous" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
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

# Name "nonhomog - Win32 Release"
# Name "nonhomog - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\NHBetaSampleGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\NHFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\NHGammaSampleGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\NHInterpretation.cpp
# End Source File
# Begin Source File

SOURCE=.\NHLikelihood.cpp
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodBetaBinomial.cpp
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodGammaPoisson.cpp
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodLogBinomial.cpp
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodLogLog.cpp
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodLogPoisson.cpp
# End Source File
# Begin Source File

SOURCE=.\NHLogBinomialSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\NHLogPoissonSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\NHModel.cpp
# End Source File
# Begin Source File

SOURCE=.\NHModelBetaBinomial.cpp
# End Source File
# Begin Source File

SOURCE=.\NHModelGammaPoisson.cpp
# End Source File
# Begin Source File

SOURCE=.\NHModelLogBinomial.cpp
# End Source File
# Begin Source File

SOURCE=.\NHModelLogLog.cpp
# End Source File
# Begin Source File

SOURCE=.\NHModelLogPoisson.cpp
# End Source File
# Begin Source File

SOURCE=.\NHObservation.cpp
# End Source File
# Begin Source File

SOURCE=.\NHPrior.cpp
# End Source File
# Begin Source File

SOURCE=.\NHPriorBetaBinomial.cpp
# End Source File
# Begin Source File

SOURCE=.\NHPriorGammaPoisson.cpp
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogBinomial.cpp
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogLog.cpp
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogNormalMedianEF.cpp
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogNormalMedianSigma.cpp
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogPoisson.cpp
# End Source File
# Begin Source File

SOURCE=.\NHSession.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\NHBetaSampleGenerator.h
# End Source File
# Begin Source File

SOURCE=.\NHFunction.h
# End Source File
# Begin Source File

SOURCE=.\NHGammaSampleGenerator.h
# End Source File
# Begin Source File

SOURCE=.\NHInterpretation.h
# End Source File
# Begin Source File

SOURCE=.\NHLikelihood.h
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodBetaBinomial.h
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodGammaPoisson.h
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodLogBinomial.h
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodLogLog.h
# End Source File
# Begin Source File

SOURCE=.\NHLikelihoodLogPoisson.h
# End Source File
# Begin Source File

SOURCE=.\NHLogBinomialSupport.h
# End Source File
# Begin Source File

SOURCE=.\NHLogPoissonSupport.h
# End Source File
# Begin Source File

SOURCE=.\NHModel.h
# End Source File
# Begin Source File

SOURCE=.\NHModelBetaBinomial.h
# End Source File
# Begin Source File

SOURCE=.\NHModelGammaPoisson.h
# End Source File
# Begin Source File

SOURCE=.\NHModelLogBinomial.h
# End Source File
# Begin Source File

SOURCE=.\NHModelLogLog.h
# End Source File
# Begin Source File

SOURCE=.\NHModelLogPoisson.h
# End Source File
# Begin Source File

SOURCE=.\NHObservation.h
# End Source File
# Begin Source File

SOURCE=.\NHPrior.h
# End Source File
# Begin Source File

SOURCE=.\NHPriorBetaBinomial.h
# End Source File
# Begin Source File

SOURCE=.\NHPriorGammaPoisson.h
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogBinomial.h
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogLog.h
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogNormalMedianEF.h
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogNormalMedianSigma.h
# End Source File
# Begin Source File

SOURCE=.\NHPriorLogPoisson.h
# End Source File
# Begin Source File

SOURCE=.\NHSession.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
