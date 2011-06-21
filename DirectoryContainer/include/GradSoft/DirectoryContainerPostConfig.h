#ifndef __GradSoft_DirectoryContainerPostConfig_h
#define __GradSoft_DirectoryContainerPostConfig_h

/*
 * part of GradSoft C++ ToolBox: DirectoryContainer
 * (C) GradSoft, 2000, 2001
 * http://www.gradsoft.com.ua
 * authors: Ruslan Shevchenko <rssh@gradsoft.com.ua>, 
 * authors: Sergey Kavertsev <kav@gradsoft.com.ua>, 
 * 2000, 2001
 * $Id: DirectoryContainerPostConfig.h,v 1.2 2002-03-27 20:02:22 rssh Exp $
 */

#ifdef HAVE_CONFIG_H
#include <GradSoft/DirectoryContainerConfig.h>
#else
# ifdef WIN32
#  include <GradSoft/DirectoryContainerConfigNT.h>
# else
#  error "we have no HAVE_CONFIG_H or WIN32 definition"
#  error "please, run autoconf"
# endif
#endif
#endif
