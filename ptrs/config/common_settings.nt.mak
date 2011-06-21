## main definishing part to include in several Makefiles
## $Id: common_settings.nt.mak,v 1.1 2002-03-16 19:03:58 kav Exp $


# tools & flags

INSTALL=copy
REMOVE=erase

CPP=cl /c
CPPFLAGS= /nologo /MD /GR /GX /D "WIN32" /D "CONSOLE" 
CPPFLAGS= $(CPPFLAGS) -I$(PROJECT_ROOT)/include -I$(MAKEDIR)

LD=link
LDFLAGS=/nologo /out:$@ $(MUTABLE_LIBS)

