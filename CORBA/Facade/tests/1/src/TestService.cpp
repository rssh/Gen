// $Id: TestService.cpp,v 1.1 2000-08-02 09:07:21 alik Exp $

#include <importCORBA.h>

#include <test1/TestServer.h>

ORB_ptr myORB;

int main(int argc, char** argv)
{

  int retval = 0;

  try
  {  

    ServiceOptions options;
    if( !options.parse(argc,argv) )
    { return 1; }

#ifdef CORBA_ORB_INIT_HAVE_3_ARGS
    ORB_var orb = ORB_init(argc,argv,CORBA_ORB_INIT_THIRD_ARG);
#else
    ORB_var orb = ORB_init(argc,argv);
#endif
    myORB=orb.in();

cout << ":: main 1" << endl;

    Test1_var tM;
    try
    { tM = createTestManager( orb, options ); }
    catch(SystemException& ex)
    {
      cerr << "System Exception";
      myORB->destroy();
      return 1;
    }

cout << ":: main 2" << endl;
    orb->run();
cout << ":: main 3" << endl;
    myORB->destroy();
  }
  catch(const SystemException& ex)
  {
#if defined(ORBACUS) && !defined(OB_INTEGER_VERSION)
    OBPrintException(ex);
#else
    cerr << "System Exception" << endl; 
#endif
    retval = 1;
  }

  return retval;
}
