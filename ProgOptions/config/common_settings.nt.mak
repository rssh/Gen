## main definishing part to include in several Makefiles
## $Id: common_settings.nt.mak,v 1.2 2004-03-13 09:53:21 rssh Exp $


# target entity

LIBPATH=$(PROJECT_ROOT)\src
SLIB=ProgOptions.lib

# required libs 

MUTABLE_LIBS=$(PROJECT_ROOT)\src\$(SLIB)

# tools & flags

!ifdef USE_DEBUG_LIB
CC_LIB_FLG=/MDd
!else
CC_LIB_FLG=/MD
!endif

INSTALL=copy
REMOVE=erase

CPP=cl /c
CPPFLAGS= /nologo $(CC_LIB_FLG) /GR /GX /D "WIN32" /D "CONSOLE" /D HAVE_NAMESPASES $(GEN_CPP_FLAGS)
CPPFLAGS= $(CPPFLAGS) -I$(PROJECT_ROOT)/include -I$(MAKEDIR)

AR=lib
ARFLAGS=/nologo /out:$@

LD=link
LDFLAGS=/nologo /out:$@ $(MUTABLE_LIBS)

