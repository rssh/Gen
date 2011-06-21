// $Id: HelloWorldClient.cpp,v 1.12 2002-04-11 12:37:34 kav Exp $

#ifndef __ServiceOptionsDemoPostConfig_h
#include <ServiceOptionsDemoPostConfig.h>
#endif

#include CORBA_H
#include COSNAMING_H
#include CORBA_STUB_HEADER(HelloWorlder)

#include <GradSoft/ProgOptions.h>

using namespace GradSoft;

using namespace std;

int run(CORBA::ORB_ptr,ProgOptions&);

int main(int argc, char** argv)
{
  ProgOptions options("--","",true);
  CORBA::ORB_var orb;
  int status;

  options.put("with-naming","get HelloService via NameService",false);
  if (!options.parse(argc,argv))
  {
    cerr<<argv[0]<<": erroneous options"<<endl;
    return -1;
  }

  try
  {
    ProgOptions::ArgsHolder argsHolder;
    argsHolder.takeArgv(options);
#ifdef CORBA_ORB_INIT_HAVE_3_ARGS
    orb = CORBA::ORB_init(argsHolder.argc,argsHolder.argv,CORBA_ORB_INIT_THIRD_ARG);
#else
    orb = CORBA::ORB_init(argsHolder.argc,argsHolder.argv);
#endif
    status=run(orb.in(),options);
  }
  catch(CORBA::SystemException& ex)
  {
#ifdef CORBA_SYSTEM_EXCEPTION_IS_STREAMBLE
    cerr << ex << endl;
#else
    cerr << argv[0]<<": CORBA System Exception" << endl;
#endif
  }

  if(!CORBA::is_nil(orb))
  {
    try
    {
      orb->destroy();
    }
    catch(const CORBA::Exception& ex)
    {
      cerr << "error while ORB destroing" << endl;
      return -1;
    }
  }

  return status;
}


int run(CORBA::ORB_ptr orb, ProgOptions& options)
{
  CORBA::Object_var obj;

  if (options.is_set("with-naming")) {

    try{

      obj = orb -> resolve_initial_references("NameService");

    } catch(const CORBA::ORB::InvalidName&) {

      cerr << options.argv()[0] << ": can't resolve 'NameService'" << endl;
      return -1;
    }

    if(CORBA::is_nil(obj.in()))
    {
      cerr << options.argv()[0] << ": 'NameService' is a nil object reference" << endl;
      return -1;
    }

    cerr<<"'NameService' resolved"<<endl;

    CosNaming::NamingContext_var nc = CosNaming::NamingContext::_narrow(obj.in());

    if(CORBA::is_nil(nc.in()))
    {
      cerr << options.argv()[0]
           << ": 'NameService' is not a NamingContext object reference"
	   << endl;
      return -1;
    }

    CosNaming::Name name;
    name.length(1);
    name[0].id = CORBA::string_dup("HelloService");
    name[0].kind = CORBA::string_dup("");

    try {

      obj = nc -> resolve(name);

    } catch(CosNaming::NamingContext::NotFound& ex) {

      cerr << options.argv()[0] << ": can't find 'HelloService' in context" << endl;
      return -1;

    } catch(CORBA::COMM_FAILURE& ex) {

      cerr << options.argv()[0]
           << "Caught system exception COMM_FAILURE -- "
           << "unable to contact thenaming service. "
           << endl;
      return -1;
    }

    cerr<<"'HelloService' resolved"<<endl;

  }else{

    try {

      obj = orb->resolve_initial_references("HelloService");

    } catch(const CORBA::ORB::InvalidName&) {

      cerr << options.argv()[0] << ": can't resolve 'HelloService'" << endl;
      return -1;
    }

    cerr<<"'HelloService' resolved"<<endl;

  }

  if( CORBA::is_nil(obj) )
  {
    cerr << options.argv()[0] << ": HelloService is a nil object reference" << endl;
    return -1;
  }

  Demo::HelloWorlder_var demoManager = Demo::HelloWorlder::_narrow(obj);

  if (CORBA::is_nil(demoManager))
  {
    cerr << options.argv()[0] << ": can't narrow demoManager to correct type" << endl;
    return -1;
  } 

  cout << "manager narrowed" << endl;

  demoManager->hello_world();

  cout << "...->hello_world() called. Watch the server!" << endl;

  return 0;
}