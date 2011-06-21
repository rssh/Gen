// $Id: Test1_impl.h,v 1.2 2000-08-03 09:41:14 alik Exp $

#ifndef __test1_Test1_impl_h
#define __test1_Test1_impl_h

#include <importCORBA.h>

#include <GradSoft/PersistentServantBase>
#include <GradSoft/CORBAFacade.h>

#include <TestPostConfig.h>
#include CORBA_SKELETON_HEADER(Test1)

using namespace Tests;
using namespace GradSoft;

class Test1_impl: virtual public POA_Tests::Test1,
                  public PersistentServantBase
{

public:

  Test1_impl( const char* POA_name, ORB_ptr orb )
                           : PersistentServantBase(POA_name, orb) {}
  ~Test1_impl() {}

  void hello_CORBA();

  void destroy();

};

#endif
