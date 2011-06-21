## main definishing part to include in several Makefiles
## $Id: env_inc.nt.mak,v 1.1 2001-09-26 09:25:52 yad Exp $


##
# Not for end user
##

PROJECT_ROOT=$(PROJECT_ROOT)\DynamicModules


# MSVC
MSVC_INCLUDE=/I$(MSVC_DIR)\include
MSVC_LIB_DIR=$(MSVC_DIR)\lib
MSVC_LINK_PATH=/libpath:$(MSVC_LIB_DIR)


PROJECT_INCLUDE= /I$(PROJECT_ROOT)\include /I$(PROJECT_ROOT)\src

HELLO_INCLUDE= /I$(PROJECT_ROOT)\demo\Hello

UNINSTALL_LIBS=DynamicModules.lib
LIB=$(PROJECT_ROOT)\src\DynamicModules.lib

######
# project make & include
#

MAKE=nmake
INSTALL_DATA=xcopy /s
RM=@del
RMDIR=@rmdir /S /Q

# C
CC=cl /c
CFLAGS=$(CPPFLAGS) $(MSVC_INCLUDE) /MD 

# cpp
CXX=cl
CPPFLAGS=/DHAVE_NAMESPACES
CXXFLAGS=/MD /GR /GX /W3 $(CPPFLAGS) $(PROJECT_INCLUDE)
CXXCOMPILE=$(CXX) /c $(CXXFLAGS)

# libs
AR=lib
AR_FLAGS=/nologo /out:$@

#exe
LINK=link
LINKEXE=$(LINK) $(MSVC_LINK_PATH) /out:$@

#dll
CXXDLL=$(CXX) $(CXXFLAGS) /LD
LINKDLL=$(LINK) /DLL $(MSVC_LINK_PATH)

#rules
#.SUFFIXES: .obj .c .cpp .exe .dll
#.cpp.obj::
#	$(CXX) $(CXXFLAGS) $<
#.c.obj::
#	$(CXX) $(CXXFLAGS) $<
#.dll.cpp::
#	$(BUILD_DLL) $<
