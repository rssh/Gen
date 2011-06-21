## main definishing part to include in several Makefiles
## $Id: env_inc.nt.mak,v 1.1 2000-08-02 09:05:57 alik Exp $

##
# User editable part
##

## Main
PROJECT_ROOT=E:\Grad\Gen\CORBA\Facade
## install
INSTALL_DIR=e:\release
INSTALL_IDL_DIR=$(INSTALL_DIR)/idl
INSTALL_INC_DIR=$(INSTALL_DIR)/include
INSTALL_LIB_DIR=$(INSTALL_DIR)/lib
## ORB
TARGET_ORB=ORBACUS
ORB_DIR=d:\ooc
#
ORB_LIBDIR=$(ORB_DIR)/lib
ORB_INCLUDES=-I$(ORB_DIR)/include -I$(ORB_DIR)/include/ob
ORB_LIBS=$(ORB_LIBDIR)/CosNaming.lib $(ORB_LIBDIR)/ob.lib $(ORB_LIBDIR)/jtc.lib
## MSVC
MSVC_DIR="d:\Microsoft Visual Studio\VC98"

##
# Not for end user
##

# idl
IDL2CXX=idl
IDL_DIR=$(PROJECT_ROOT)\idl
INCLUDES=-I. -I$(INSTALL_DIR)\idl -I$(ORB_DIR)\idl -I$(ORB_DIR)\include
IDLFLAGS=-D_MSVC_VER -DNO_CORBA_OCTETSEQ -D$(TARGET_ORB) -DHAVE_NAMESPACES
IDL_SRV_ALL_OBJS=
# libs
AR=lib
AR_FLAGS=/nologo /out:$@
LIB=PersistentServantBase.lib
CLLIB=
RANLIB=-
# MSVC
MSVC_INCLUDE=-I$(MSVC_DIR)/include
MSVC_LIB_DIR=$(MSVC_DIR)/lib
MSVC_LIBS=$(MSVC_LIB_DIR)/uuid.lib $(MSVC_LIB_DIR)/user32.lib $(MSVC_LIB_DIR)/advapi32.lib $(MSVC_LIB_DIR)/wsock32.lib $(MSVC_LIB_DIR)/msvcrt.lib $(MSVC_LIB_DIR)/msvcprt.lib $(MSVC_LIB_DIR)/msvcirt.lib $(MSVC_LIB_DIR)/OLDNAMES.LIB $(MSVC_LIB_DIR)/kernel32.lib
GRADSOFTLIB=$(INSTALL_LIB_DIR)/ProgOptions.lib $(INSTALL_LIB_DIR)/ServiceOptions.lib
# project make & include
PREFIX=$(INSTALL_DIR)
MAKE=nmake
INSTALL_DATA=xcopy /s
RM=del
PROJECT_INCLUDE=-I. -I$(PROJECT_ROOT)/include -I$(PROJECT_ROOT)/src -I$(PROJECT_ROOT)/src/include
# C
CC=cl /c
CFLAGS=$(CPPFLAGS) -I$(MSVC_INCLUDE) /Gy /MD 
# cpp
CXX=cl /c
CPPFLAGS=/D "WIN32" /D "CONSOLE" /D "NO_CORBA_GET_BUFFER" /D "OB4" /D "CORBA_MODULE_CLASS_MAPPING" /D "CORBA_MODULE_NAMESPACE_MAPPING" /D $(TARGET_ORB) /D "CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE" /D "HAVE_NAMESPACES"
CXXFLAGS=/Zm500 /MD /W3 /GR /GX $(CPPFLAGS) \
	-I$(IDL_DIR) $(PROJECT_INCLUDE) \
	$(MSVC_INCLUDE) -I$(INSTALL_DIR)/include \
	$(ORB_INCLUDES)
CXXCOMPILE=$(CXX) $(CXXFLAGS)
#exe
LD=cl
LDFLAGS=$(PROJECT_ROOT)\src\$(LIB) $(ORB_LIBS) $(MSVC_LIBS) $(GRADSOFTLIB)
#dirs
TEST_DIR=$(PROJECT_ROOT)\tests
TEST1_DIR=$(TEST_DIR)\1
TEST1_IDL_DIR=$(TEST1_DIR)\idl
TEST1_SRC_DIR=$(TEST1_DIR)\src
#rules
.SUFFIXES: .obj .c .cpp .exe
.cpp.obj::
	$(CXX) $(CXXFLAGS) $<
.c.obj::
	$(CXX) $(CXXFLAGS) $<
