## main definishing part to include in several Makefiles
## $Id: common_settings.nt.mak,v 1.2 2004-03-13 09:53:20 rssh Exp $


# target entity

LIBPATH=$(PROJECT_ROOT)\src
SLIB=DynamicModules.lib

# required libs 

MUTABLE_LIBS=$(PROJECT_ROOT)\src\$(SLIB)

# tools & flags

INSTALL=copy
REMOVE=erase

!ifdef USE_DEBUG_LIB
CC_LIB_FLG=/MDd
!else
CC_LIB_FLG=/MD
!endif


CPP=cl /c
CPPFLAGS= /nologo $(CC_LIB_FLG) /GR /GX /D "WIN32" /D "CONSOLE" 
CPPFLAGS= $(CPPFLAGS) -I$(PROJECT_ROOT)/include -I$(MAKEDIR)

AR=lib
ARFLAGS=/nologo /out:$@

EXE=link
EXEFLAGS=/nologo /out:$@ $(MUTABLE_LIBS)

DLL=link -DLL 
DLLFLAGS=/nologo /out:$@ 
