// $Id: TestServer.h,v 1.1 2000-08-02 09:07:23 alik Exp $

#ifndef __test1_TestServer_h
#define __test1_TestServer_h


#ifndef __importCORBA_h
#include <importCORBA.h>
#endif

#ifndef __test1_Test1_impl_h
#include <test1/Test1_impl.h>
#endif

#include <GradSoft/ServiceOptions.h>

Test1_ptr createTestManager( ORB_ptr orb, ServiceOptions& options );

#endif
