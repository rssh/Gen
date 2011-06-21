## main definishing part to include in several Makefiles
## $Id: common_settings.nt.mak,v 1.3 2004-03-13 09:53:21 rssh Exp $


# target entity

SLIB=Logger.lib

# required libs

MUTABLE_LIBS=$(PROJECT_ROOT)\src\$(SLIB) $(PROJECT_ROOT)\..\Threading\src\Threading.lib


# tools & flags

INSTALL=copy
REMOVE=erase

!ifdef USE_DEBUG_LIB
CC_LIB_FLG=/MDd
!else
CC_LIB_FLG=/MD
!endif


CPP=cl /c
CPPFLAGS= /nologo $(CC_LIB_FLG) /GR /GX /D "WIN32" /D "CONSOLE" $(GEN_CPP_FLAGS)
CPPFLAGS= $(CPPFLAGS) -I$(PROJECT_ROOT)\..\ptrs\include -I$(PROJECT_ROOT)\..\Threading\include -I$(PROJECT_ROOT)\include -I$(MAKEDIR)

AR=lib
ARFLAGS=/nologo /out:$@

LD=link
LDFLAGS=/nologo /out:$@ $(MUTABLE_LIBS)

