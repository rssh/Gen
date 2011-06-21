## main definishing part to include in several Makefiles
## $Id: common_settings.nt.mak,v 1.5 2004-03-13 09:53:22 rssh Exp $


# target entity

SLIB=Threading.lib
SSLIB=Threading.dll

# required libs


THREADING_LIBS=$(PROJECT_ROOT)\src\$(SLIB)
THREADING_DLLS=$(PROJECT_ROOT)\src\$(SSLIB)


# tools & flags

INSTALL=copy
REMOVE=erase

!ifdef USE_DEBUG_LIB
CC_LIB_FLG=/MDd
!else
CC_LIB_FLG=/MD
!endif

!ifdef USE_DLL_THREADING
CPP_THREADING_MODE="SHARED_THREADING"
!else
CPP_THREADING_MODE="STATIC_THREADING"
!endif

CPP=cl /c
CPPFLAGS= /nologo $(CC_LIB_FLG) /GR /GX /Gy /D "WIN32" /D "CONSOLE" /D "COMPILE_THREADING" /D $(CPP_THREADING_MODE)
CPPFLAGS= $(CPPFLAGS) -I$(PROJECT_ROOT)/../ptrs/include -I$(PROJECT_ROOT)/include -I$(MAKEDIR)

AR=lib
ARFLAGS=/nologo /out:$@

LD=link
LD_SHARED_FLG=/DLL
LDFLAGS=/nologo /out:$@ 








