// $Id: TestServer.cpp,v 1.1 2000-08-02 09:07:23 alik Exp $

#ifndef __test1_TestServer_h
#include <test1/TestServer.h>
#endif

const char* SERVICE_POA_NAME = "Test1POA";
const char* SERVICE_NAME = "Test1";

Test1_ptr createTestManager( ORB_ptr orb, ServiceOptions& options )
{
  Test1_impl* m_impl = new Test1_impl( SERVICE_POA_NAME, orb );
  options.putServiceName(SERVICE_NAME);
  options.bindPersistentPOAServiceObject( 
                 m_impl->_default_POA(), orb, m_impl, SERVICE_NAME );
  Test1_ptr manager = m_impl->_this();
  options.create_object_ior( orb, manager, SERVICE_NAME );
  m_impl->_remove_ref();

  return manager;
}

