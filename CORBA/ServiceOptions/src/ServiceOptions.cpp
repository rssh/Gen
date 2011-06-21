// part of GradSoft C++ ToolBox
// (C) GradSoft 2000, 2001, 2002, 2003
// $Id: ServiceOptions.cpp,v 1.31 2004-03-13 09:53:18 rssh Exp $

#include <importCORBA.h>
#include <importCosNaming.h>

#include <GradSoft/ServiceOptions.h>

#include <sstream>
#include <fstream>

#ifndef STREAMBUF_HAVE_PUBSEEKPOS
#define pubseekpos seekpos
#endif

namespace GradSoft {

bool ServiceOptions::ior_file_callback(const ProgOptions* po, const char* argument, void* data)
{
 ProgOptions* po1 = const_cast<ProgOptions*>(po);
 ServiceOptions* sop = dynamic_cast<ServiceOptions*>(po1);
 const char* name = static_cast<char*>(data);
 sop->set_ior_fname(name, argument);
 return true;
}

ServiceOptions::ServiceOptions(const char* optPrefix, const char* pkgPrefix)
  :ProgOptions(optPrefix,pkgPrefix,true)
{
 put("with-naming","bind service options into Naming Service");
 put("ior-stdout","print ior of service objects into stdout");
}

ServiceOptions::~ServiceOptions()
{
}

void ServiceOptions::putServiceName(const char* name)
{
 ServiceRecord& sr=serviceRecords_[name];
 std::ostringstream ostr;
 ostr << "ior-file-" << name << '\0';
 sr.set_ior_file_option(ostr.str().c_str());
      
 ostr.rdbuf()->pubseekpos(0,std::ios::out);
 ostr << "output ior of " << name << " to file <argument>" << '\0';
 sr.set_ior_file_description(ostr.str().c_str());
 put(sr.get_ior_file_option(),sr.get_ior_file_description(),true,
     ServiceOptions::ior_file_callback,(void*)name);
 
#ifdef MICO
 char* argv[2];
 argv[0]="-POAImplName";
 argv[1]=const_cast<char*>(name);
 putArgs(NULL,2,argv);
#endif
}

bool ServiceOptions::bindServiceObject(CORBA::ORB_ptr orb, 
                                       CORBA::Object_ptr object,
                                       PortableServer::Servant p_servant,
                                       const char* objName,
                                       bool rebind)
{
 if(is_set("with-naming")) {
     CosNaming::Name name;
     name.length(1);
     name[0].id=CORBA::string_dup(objName);
     name[0].kind=CORBA::string_dup("");
     CORBA::Object_var obj ;
     try {
        obj = orb->resolve_initial_references("NameService");
     }catch(const CORBA::ORB::InvalidName& ex){
        std::cerr << argv()[0] << ": can't resolve `NameService'" << std::endl;
        return false;
     }
     CosNaming::NamingContext_var nc;
     try {
        nc = CosNaming::NamingContext::_narrow(obj.in());
     }catch(const CORBA::SystemException& ex){
        std::cerr << argv()[0] << ": can't narrow naming service" << std::endl;
        return false;
     }
     try {
        if(rebind){
          nc->rebind(name,object);
        }else{
          try{
            nc->bind(name,object);
          }catch(const CosNaming::NamingContext::AlreadyBound&){
            std::cerr<<argv()[0]<<":can't bind "<<objName<<" (AlreadyBound)"<< std::endl;
            return false;
          }
        }
     }catch(const CosNaming::NamingContext::CannotProceed&){
        std::cerr<<argv()[0]<<": can't bind "<<objName<<" (CannotProceed)"<< std::endl;
        return false;
     }catch(const CosNaming::NamingContext::InvalidName&){
        std::cerr<<argv()[0]<<":can't bind "<<objName<<" (InvalidName)"<< std::endl;
        return false;
     }
     std::cout<<argv()[0]<<": binding completed successfully"<<std::endl;
 }
 CORBA::String_var ior ;
 try {
   ior = orb->object_to_string(object);
 }catch(const CORBA::SystemException& ex){
#ifdef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
   std::cerr << ex << std::endl;
#else
   std::cerr << "CORBA::SystemException" << std::endl; 
#endif
   return false;
 }
 const char* fname = get_ior_fname(objName);
 if (fname!=NULL && strcmp(fname,"")!=0) {
    std::ofstream ofs (fname);
    if (ofs.bad()) {
       std::cerr << argv()[0] << ": can't open file " << fname <<  std::endl;
       perror(argv()[0]);
       return false;
    }
    ofs << ior.in();
    ofs.close();
 }
 if (is_set("ior-stdout")) {
    std::cout << ior << std::flush;
 }
#ifdef ORBACUS
 CORBA::Object_var bmgrObj = orb->resolve_initial_references("BootManager");
 OB::BootManager_var bootManager = OB::BootManager::_narrow(bmgrObj);
 PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(objName);
 bootManager -> add_binding(oid.in(),object);
#elif defined(OMNIORB)
 PortableServer::POA_var objPOA = p_servant->_default_POA();
 CORBA::String_var objPOAName = objPOA->the_name();
 if (strcmp(objPOAName.in(),"omniINSPOA")!=0) {
   CORBA::Object_var insPOAObj = orb->resolve_initial_references("omniINSPOA");
   PortableServer::POA_var insPOA = PortableServer::POA::_narrow(insPOAObj);
   PortableServer::POAManager_var insPOAManager=insPOA->the_POAManager();
   insPOAManager->activate();
   PortableServer::ObjectId_var oid = 
                          PortableServer::string_to_ObjectId(objName);
   insPOA->activate_object_with_id(oid.in(),p_servant);
 }
#elif defined(RSSH_TAO)
#ifdef TAO_HAVE_IORTABLE_ADAPTER
 CORBA::Object_var table = orb->resolve_initial_references ("IORTable");
 IORTable::Table_var adapter = IORTable::Table::_narrow (table.in());
 if (CORBA::is_nil(adapter.in())) {
   cerr<<argv()[0]<<": nil IORTable reference"<<endl;
 } else {
   adapter->bind (objName, ior.in ());
 }
#else
 ACE_CString ins(objName);
 if (orb->_tao_add_to_IOR_table(ins,object)!=0) {
   return false;
 }
#endif
#elif defined(MICO)
  // create persistent POA with name as service name if we have no one.
 PortableServer::POA_var objPOA = p_servant->_default_POA();
 CORBA::String_var objPOAName = objPOA->the_name();
 std::cerr << "existent OBJPOAName=" << objPOAName.in() << std::endl;
 std::cerr << "objName=" << objName << std::endl;
 if (strcmp(objPOAName.in(),objName)!=0) {
   CORBA::Object_var rootPOAObj = orb->resolve_initial_references ("RootPOA");
   PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(rootPOAObj);
   CORBA::PolicyList pl;
   pl.length (2);                              
   pl[0] = rootPOA->create_lifespan_policy (PortableServer::PERSISTENT);
   pl[1] = rootPOA->create_id_assignment_policy (PortableServer::USER_ID);
   PortableServer::POA_var objPOA = rootPOA->create_POA(objName,
			                  PortableServer::POAManager::_nil (),
			                  pl);
   PortableServer::POAManager_var objPOAManager = objPOA->the_POAManager ();   
   PortableServer::ObjectId_var objId =
                           PortableServer::string_to_ObjectId (objName);
   objPOA->activate_object_with_id (objId.in(), p_servant);
   objPOAManager->activate();
 }else{
   //PortbaobjPOA
 }  
#endif
 return true;
}


ServiceOptions::ServiceRecord::ServiceRecord()
 :ior_file_option_(""),
  ior_file_description_(""),
  ior_fname_("")
{
}

}


