// $Id: PersistentServantBase.cpp,v 1.3 2000-08-10 17:20:57 rssh Exp $

#include <GradSoft/PersistentServantBase>


#ifdef HAVE_NAMESPACES
namespace GradSoft {
#endif


#ifdef CORBA_HAVE_POA

PersistentServantBase::PersistentServantBase( const char* POA_name,
                                                  ORB_ptr orb )
{
  orb_ = ORB::_duplicate(orb);
  create_user_persistent_POA(POA_name);
}

PersistentServantBase::PersistentServantBase( 
                        PortableServer::POA_ptr defaultPOA, ORB_ptr orb )
{
  orb_ = ORB::_duplicate(orb);
  defaultPOA_ = PortableServer::POA::_duplicate( defaultPOA );
  POA_name_ = defaultPOA_->the_name();
}

#else

PersistentServantBase::PersistentServantBase( ORB_ptr orb, BOA_ptr boa )
{

  orb_ = ORB::_duplicate(orb);
  boa_ = BOA::_duplicate(boa);
}

#endif


#ifdef CORBA_HAVE_POA
void PersistentServantBase::create_user_persistent_POA(const char* POAname)
{
  Object_var poaObj = orb_->resolve_initial_references("RootPOA"); 
  PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObj.in());

  PortableServer::POAManager_var poaManager = rootPOA->the_POAManager();
  poaManager->activate();
  PolicyList policyList;
  policyList.length(2);
  policyList[0] = rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);
  policyList[1] = rootPOA->create_id_assignment_policy(PortableServer::USER_ID);
  defaultPOA_ = rootPOA->create_POA( POAname,
                                     poaManager,
                                     policyList );
}

#endif

#ifdef HAVE_NAMESPACES
}
#endif

