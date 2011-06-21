## Gen project: Main environment settings for Win32-compile process
## (C) GradSoft 2000
## $Id: environment.nt.mak,v 1.9 2004-03-13 09:53:29 rssh Exp $


# installation root: it's recomended to set next variables via it:

INSTALL_DIR=C:\local\ndb

# directory, delivered include files will be installed here:

INSTALL_INC_DIR=$(INSTALL_DIR)\include

# directory, delivered libraries will be installed here:

INSTALL_LIB_DIR=$(INSTALL_DIR)\lib

# enable this if you want to link with debug library

#USE_DEBUG_LIB=1

# enable this if you want to use threading as DLL instead static library

#USE_DLL_THREADING=1

!ifdef USE_DLL_THREADING
CPP_THREADING_MODE="SHARED_THREADING"
!else
CPP_THREADING_MODE="STATIC_THREADING"
!endif



GEN_CPP_FLAGS= -D $(CPP_THREADING_MODE)