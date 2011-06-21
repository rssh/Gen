#ifndef __importCORBA_H
#define __importCORBA_H

/**
 * import CORBA header file and some related files
 */

#ifndef __ServiceOptionsDemoPostConfig_h
#include <ServiceOptionsDemoPostConfig.h>
#endif

#ifdef CORBA_H
#include CORBA_H
#endif

#ifdef RSSH_TAO
#include "tao/PortableServer/PortableServer.h"
#endif

#endif
