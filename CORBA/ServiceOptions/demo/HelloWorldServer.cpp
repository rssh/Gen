
#include <ServiceOptionsDemoPostConfig.h>

#include CORBA_H
#include COSNAMING_H
#include CORBA_SKELETON_HEADER(HelloWorlder)

#include <GradSoft/ServiceOptions.h>

using namespace GradSoft;

#include <string.h>
#include <iostream>
using namespace std;

class HelloWorlder_impl:public POA_Demo::HelloWorlder
{
public:
  void hello_world() throw (CORBA::SystemException)
    { cout << "Hello, world" << endl; }
};

const char* SERVICE_NAME = "HelloService";

int main(int argc, char** argv)
{
 ServiceOptions options;
 options.putServiceName(SERVICE_NAME);
 if (!options.parse(argc,argv)) return 1;
 ProgOptions::ArgsHolder argsHolder;
 argsHolder.takeArgv(options);

#if 1 
 cout<<options.argv()[0]<<" has been started with following options:"<<endl;
 for(int i=1;i<options.argc();++i) 
 cerr<<"  "<<options.argv()[i]<<endl;
#endif

 CORBA::ORB_var orb;
 try {
#ifdef CORBA_ORB_INIT_HAVE_3_ARGS
    orb = CORBA::ORB_init(argsHolder.argc,argsHolder.argv,CORBA_ORB_INIT_THIRD_ARG);
#else
    orb = CORBA::ORB_init(argsHolder.argc,argsHolder.argv);
#endif
 }catch(const CORBA::SystemException& ex){
    std::cerr << "CORBA::SystemException thrown during CORBA::ORB_init"<<endl;
#ifdef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
    std::cerr << ex << endl; 
#endif 
    return 1;
 }

 CORBA::Object_var poaObj = orb->resolve_initial_references("RootPOA");
 PortableServer::POA_var poa = PortableServer::POA::_narrow(poaObj.in());
 PortableServer::POAManager_var poaManager = poa->the_POAManager();
 poaManager->activate();

 HelloWorlder_impl helloWorlder_impl;
 Demo::HelloWorlder_var helloWorlder = helloWorlder_impl._this();
 options.bindServiceObject(orb.in(),helloWorlder.in(),&helloWorlder_impl,SERVICE_NAME,true);
 try {
   orb->run();
 }catch(const CORBA::SystemException& ex){
   std::cerr << "CORBA::SystemException thrown during CORBA::ORB_run"<<endl;
#ifdef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
   std::cerr << "minor code is "<<ex.minor()<< endl;
#endif
 }
 orb->destroy();
 return 0;
}
