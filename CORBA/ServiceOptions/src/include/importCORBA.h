#ifndef __importCORBA_H
#define __importCORBA_H

/*
 * import CORBA definitions to program 'namespace'
 * when namespaces are not supported.
 */

#ifndef __ServiceOptionsPostConfig_h
#include <ServiceOptionsPostConfig.h>
#endif

#ifdef CORBA_H
#include CORBA_H
#else
#error CORBA header file unspecified
#endif

#ifdef RSSH_TAO
#ifdef TAO_HAVE_PORTABLE_SERVER_H
#include <tao/PortableServer/PortableServer.h>
#endif
#ifdef TAO_HAVE_IORTABLE_ADAPTER
#include <tao/IORTable/IORTable.h>
#endif
#endif

#ifdef ORBACUS
#include <OB/BootManager.h>
#endif

#endif
