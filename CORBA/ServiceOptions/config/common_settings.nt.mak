## ServiceOptions: main definishing part to include in several Makefiles
## $Id: common_settings.nt.mak,v 1.4 2004-03-13 16:22:00 kav Exp $

#####################
# libraries supplied:

SLIB=ServiceOptions.lib

#####################
# auxiliary settings:

# settings caused by project structure:

PROJECT_INCLUDES= -I$(PROJECT_ROOT)\include -I$(PROJECT_ROOT)\src\include 
PROJECT_LIB_DIRS= -libpath:$(PROJECT_ROOT)\src

# settings caused by external dependencies:

TOOLBOX_LIBS= ProgOptions.lib

TOOLBOX_INCLUDES= -I$(PROJECT_ROOT)\..\..\ProgOptions\include
TOOLBOX_LIB_DIRS= -libpath:$(PROJECT_ROOT)\..\..\ProgOptions\src

# GradC++ToolBox libraries required for test application:

MUTABLE_LIBS=$(PROJECT_ROOT)\src\$(SLIB) $(PROJECT_ROOT)\..\..\ProgOptions\src\ProgOptions.lib

##################
# tools and flags:

#lib
AR=lib
AR_FLAGS=/nologo /out:$(SLIB)

!ifdef USE_DEBUG_LIB
CLIB_FLG=/MDd
!else
CLIB_FLG=/MD
!endif
CPP=cl /c /TP
DEFINES=/D "WIN32" /D "CONSOLE" /D $(TARGET_ORB)
CPPFLAGS=/nologo $(CLIB_FLG) /GR /GX $(DEFINES) $(IDLCXXFLAGS) $(PROJECT_INCLUDES) $(TOOLBOX_INCLUDES)

#exe
LD=link 
LDFLAGS= $(SLIB) $(TOOLBOX_LIBS) $(ORB_LIBS) $(ORB_COSNAMING_LIB) $(PROJECT_LIB_DIRS) $(TOOLBOX_LIB_DIRS) $(IDL_LIBDIR)
