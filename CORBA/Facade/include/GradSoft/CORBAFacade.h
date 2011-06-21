#ifndef __GradSoft_CORBAFacade_h
#define __GradSoft_CORBAFacade_h

/**
 * part of GradSoft C++ toolbox.
 * (C) GradSoft, 2000
 * author: Alik Rivkind, <Alik@Rivkind.Kiev.UA>
 * $Id: CORBAFacade.h,v 1.3 2000-08-03 19:36:17 rssh Exp $
 **/


#ifdef HAVE_NAMESPACES
namespace GradSoft {
#endif

class CORBAFacade
{

public:

#ifdef CORBA_HAVE_POA

  static void deactivate_object( PortableServer::Servant p_servant );

#else

  static void deactivate_object( ORB_ptr orb_p,
                                 Object* p_servant );
#endif

};

#ifdef HAVE_NAMESPACES
}
#endif

#endif
