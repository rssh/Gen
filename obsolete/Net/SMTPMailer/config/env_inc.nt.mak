## main definishing part to include in several Makefiles
## $Id: env_inc.nt.mak,v 1.1 2001-02-09 15:02:34 yad Exp $


##
# Not for end user
##
PROJECT_ROOT=$(PROJECT_ROOT)\Net\SMTPmailer

# libs
AR=lib
AR_FLAGS=/nologo /out:$@
LIB=SMTPmailer.lib

# MSVC
MSVC_INCLUDE=-I$(MSVC_DIR)/include
MSVC_LIB_DIR=$(MSVC_DIR)/lib
MSVC_LIBS=$(MSVC_LIB_DIR)/wsock32.lib $(MSVC_LIB_DIR)/uuid.lib $(MSVC_LIB_DIR)/msvcprt.lib $(MSVC_LIB_DIR)/msvcrt.lib $(MSVC_LIB_DIR)/OLDNAMES.lib $(MSVC_LIB_DIR)/kernel32.lib

######
# project make & include
#
MAKE=nmake

INSTALL_DATA=xcopy /s

RM=del

# C
CC=cl /c
CFLAGS=$(CPPFLAGS) -I$(MSVC_INCLUDE) /Gy /MD

# cpp
CXX=cl /c
CPPFLAGS=/D "WIN32" /D "CONSOLE" /D "HAVE_NAMESPACES"
PROJECT_INCLUDE= -I$(PROJECT_ROOT)/include \
                 -I$(PROJECT_ROOT)/src \
                 -I$(PROJECT_ROOT)/../Socket/include \
                 -I$(PROJECT_ROOT)/src/include
CXXFLAGS=/Zm500 /MD /W3 /GR /GX $(CPPFLAGS) $(PROJECT_INCLUDE) $(MSVC_INCLUDE)
CXXCOMPILE=$(CXX) $(CXXFLAGS)

#exe
LD=cl /MD
LDFLAGS=$(PROJECT_ROOT)\src\$(LIB) $(MSVC_LIBS)

#rules
.SUFFIXES: .obj .c .cpp .exe
.cpp.obj::
    $(CXX) $(CXXFLAGS) $<
.c.obj::
    $(CXX) $(CXXFLAGS) $<
