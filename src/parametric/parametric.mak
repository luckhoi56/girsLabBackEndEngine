# Microsoft Developer Studio Generated NMAKE File, Based on parametric.dsp
!IF "$(CFG)" == ""
CFG=parametric - Win32 Debug
!MESSAGE No configuration specified. Defaulting to parametric - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "parametric - Win32 Release" && "$(CFG)" != "parametric - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "parametric - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\parametric.lib" "$(OUTDIR)\parametric.bsc"

!ELSE 

ALL : "general - Win32 Release" "$(OUTDIR)\parametric.lib" "$(OUTDIR)\parametric.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"general - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Beta0595InputMode.obj"
	-@erase "$(INTDIR)\Beta0595InputMode.sbr"
	-@erase "$(INTDIR)\BetaDistribution.obj"
	-@erase "$(INTDIR)\BetaDistribution.sbr"
	-@erase "$(INTDIR)\Gamma0595InputMode.obj"
	-@erase "$(INTDIR)\Gamma0595InputMode.sbr"
	-@erase "$(INTDIR)\GammaDistribution.obj"
	-@erase "$(INTDIR)\GammaDistribution.sbr"
	-@erase "$(INTDIR)\LogNormalDistribution.obj"
	-@erase "$(INTDIR)\LogNormalDistribution.sbr"
	-@erase "$(INTDIR)\NormalDistribution.obj"
	-@erase "$(INTDIR)\NormalDistribution.sbr"
	-@erase "$(INTDIR)\ParametricDistribution.obj"
	-@erase "$(INTDIR)\ParametricDistribution.sbr"
	-@erase "$(INTDIR)\ParametricInputMode.obj"
	-@erase "$(INTDIR)\ParametricInputMode.sbr"
	-@erase "$(INTDIR)\TrLogNormalDistribution.obj"
	-@erase "$(INTDIR)\TrLogNormalDistribution.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\parametric.bsc"
	-@erase "$(OUTDIR)\parametric.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /MT /W3 /GX /O2 /I "..\general" /I "..\dcdflib" /I "..\ranlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\parametric.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Beta0595InputMode.sbr" \
	"$(INTDIR)\BetaDistribution.sbr" \
	"$(INTDIR)\Gamma0595InputMode.sbr" \
	"$(INTDIR)\GammaDistribution.sbr" \
	"$(INTDIR)\LogNormalDistribution.sbr" \
	"$(INTDIR)\NormalDistribution.sbr" \
	"$(INTDIR)\ParametricDistribution.sbr" \
	"$(INTDIR)\ParametricInputMode.sbr" \
	"$(INTDIR)\TrLogNormalDistribution.sbr"

"$(OUTDIR)\parametric.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\parametric.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Beta0595InputMode.obj" \
	"$(INTDIR)\BetaDistribution.obj" \
	"$(INTDIR)\Gamma0595InputMode.obj" \
	"$(INTDIR)\GammaDistribution.obj" \
	"$(INTDIR)\LogNormalDistribution.obj" \
	"$(INTDIR)\NormalDistribution.obj" \
	"$(INTDIR)\ParametricDistribution.obj" \
	"$(INTDIR)\ParametricInputMode.obj" \
	"$(INTDIR)\TrLogNormalDistribution.obj" \
	"..\general\Release\general.lib"

"$(OUTDIR)\parametric.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "parametric - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\parametric.lib" "$(OUTDIR)\parametric.bsc"

!ELSE 

ALL : "general - Win32 Debug" "$(OUTDIR)\parametric.lib" "$(OUTDIR)\parametric.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"general - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Beta0595InputMode.obj"
	-@erase "$(INTDIR)\Beta0595InputMode.sbr"
	-@erase "$(INTDIR)\BetaDistribution.obj"
	-@erase "$(INTDIR)\BetaDistribution.sbr"
	-@erase "$(INTDIR)\Gamma0595InputMode.obj"
	-@erase "$(INTDIR)\Gamma0595InputMode.sbr"
	-@erase "$(INTDIR)\GammaDistribution.obj"
	-@erase "$(INTDIR)\GammaDistribution.sbr"
	-@erase "$(INTDIR)\LogNormalDistribution.obj"
	-@erase "$(INTDIR)\LogNormalDistribution.sbr"
	-@erase "$(INTDIR)\NormalDistribution.obj"
	-@erase "$(INTDIR)\NormalDistribution.sbr"
	-@erase "$(INTDIR)\ParametricDistribution.obj"
	-@erase "$(INTDIR)\ParametricDistribution.sbr"
	-@erase "$(INTDIR)\ParametricInputMode.obj"
	-@erase "$(INTDIR)\ParametricInputMode.sbr"
	-@erase "$(INTDIR)\TrLogNormalDistribution.obj"
	-@erase "$(INTDIR)\TrLogNormalDistribution.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\parametric.bsc"
	-@erase "$(OUTDIR)\parametric.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\general" /I "..\dcdflib" /I "..\ranlib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\parametric.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\parametric.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Beta0595InputMode.sbr" \
	"$(INTDIR)\BetaDistribution.sbr" \
	"$(INTDIR)\Gamma0595InputMode.sbr" \
	"$(INTDIR)\GammaDistribution.sbr" \
	"$(INTDIR)\LogNormalDistribution.sbr" \
	"$(INTDIR)\NormalDistribution.sbr" \
	"$(INTDIR)\ParametricDistribution.sbr" \
	"$(INTDIR)\ParametricInputMode.sbr" \
	"$(INTDIR)\TrLogNormalDistribution.sbr"

"$(OUTDIR)\parametric.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\parametric.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Beta0595InputMode.obj" \
	"$(INTDIR)\BetaDistribution.obj" \
	"$(INTDIR)\Gamma0595InputMode.obj" \
	"$(INTDIR)\GammaDistribution.obj" \
	"$(INTDIR)\LogNormalDistribution.obj" \
	"$(INTDIR)\NormalDistribution.obj" \
	"$(INTDIR)\ParametricDistribution.obj" \
	"$(INTDIR)\ParametricInputMode.obj" \
	"$(INTDIR)\TrLogNormalDistribution.obj" \
	"..\general\Debug\general.lib"

"$(OUTDIR)\parametric.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("parametric.dep")
!INCLUDE "parametric.dep"
!ELSE 
!MESSAGE Warning: cannot find "parametric.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "parametric - Win32 Release" || "$(CFG)" == "parametric - Win32 Debug"
SOURCE=.\Beta0595InputMode.cpp

"$(INTDIR)\Beta0595InputMode.obj"	"$(INTDIR)\Beta0595InputMode.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BetaDistribution.cpp

"$(INTDIR)\BetaDistribution.obj"	"$(INTDIR)\BetaDistribution.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Gamma0595InputMode.cpp

"$(INTDIR)\Gamma0595InputMode.obj"	"$(INTDIR)\Gamma0595InputMode.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GammaDistribution.cpp

"$(INTDIR)\GammaDistribution.obj"	"$(INTDIR)\GammaDistribution.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LogNormalDistribution.cpp

"$(INTDIR)\LogNormalDistribution.obj"	"$(INTDIR)\LogNormalDistribution.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NormalDistribution.cpp

"$(INTDIR)\NormalDistribution.obj"	"$(INTDIR)\NormalDistribution.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ParametricDistribution.cpp

"$(INTDIR)\ParametricDistribution.obj"	"$(INTDIR)\ParametricDistribution.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ParametricInputMode.cpp

"$(INTDIR)\ParametricInputMode.obj"	"$(INTDIR)\ParametricInputMode.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TrLogNormalDistribution.cpp

"$(INTDIR)\TrLogNormalDistribution.obj"	"$(INTDIR)\TrLogNormalDistribution.sbr" : $(SOURCE) "$(INTDIR)"


!IF  "$(CFG)" == "parametric - Win32 Release"

"general - Win32 Release" : 
   cd "\sandbox\ptdev\native\redcas\general"
   $(MAKE) /$(MAKEFLAGS) /F .\general.mak CFG="general - Win32 Release" 
   cd "..\parametric"

"general - Win32 ReleaseCLEAN" : 
   cd "\sandbox\ptdev\native\redcas\general"
   $(MAKE) /$(MAKEFLAGS) /F .\general.mak CFG="general - Win32 Release" RECURSE=1 CLEAN 
   cd "..\parametric"

!ELSEIF  "$(CFG)" == "parametric - Win32 Debug"

"general - Win32 Debug" : 
   cd "\sandbox\ptdev\native\redcas\general"
   $(MAKE) /$(MAKEFLAGS) /F .\general.mak CFG="general - Win32 Debug" 
   cd "..\parametric"

"general - Win32 DebugCLEAN" : 
   cd "\sandbox\ptdev\native\redcas\general"
   $(MAKE) /$(MAKEFLAGS) /F .\general.mak CFG="general - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\parametric"

!ENDIF 


!ENDIF 

