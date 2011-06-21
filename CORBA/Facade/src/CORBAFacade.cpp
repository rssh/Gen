// $Id: CORBAFacade.cpp,v 1.3 2000-08-04 07:33:51 alik Exp $

#ifndef __GradSoft_CORBAFacade_h
#include <GradSoft/CORBAFacade.h>
#endif


#ifdef HAVE_NAMESPACES
namespace GradSoft {
#endif


#ifdef CORBA_HAVE_POA
void CORBAFacade::deactivate_object( PortableServer::Servant p_servant )
#else
void CORBAFacade::deactivate_object( ORB_ptr orb_p,
                                     Object* p_servant )
#endif
{

#ifdef CORBA_HAVE_POA
  PortableServer::POA_ptr poa = p_servant->_default_POA();
  PortableServer::ObjectId_var oid = poa->servant_to_id( p_servant );
  poa->deactivate_object(oid);
#else
  orb_p->disconnect( p_servant );
#endif
}


#ifdef HAVE_NAMESPACES
}
#endif
