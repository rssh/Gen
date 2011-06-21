#ifndef __ServiceOptionsPostConfig_h
#define __ServiceOptionsPostConfig_h

#ifdef WIN32

#include <ServiceOptionsWin32Config.h>

#elif defined(HAVE_CONFIG_H)

#include <ServiceOptionsConfig.h>

#else

#error unknown platform: please, run autoconf

#endif

#endif
