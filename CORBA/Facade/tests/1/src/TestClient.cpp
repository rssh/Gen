// $Id: TestClient.cpp,v 1.1 2000-08-02 09:07:21 alik Exp $

#include <importCORBA.h>

#include <TestPostConfig.h>

#include CORBA_SKELETON_HEADER(Test1)

#ifdef _MSC_VER
using namespace std;
#endif

ORB_ptr myORB;

int main(int argc, char** argv)
{
cout << "TestClient begin" << endl;
  try
  {
    ORB_var orb = ORB_init(argc, argv);
cout << "ORB inited" << endl;
    myORB=orb.in();

    Object_var obj;
    try
    { obj = orb->resolve_initial_references("Test1"); }
    catch(const ORB::InvalidName&)
    {
       cerr << argv[0] << ": can't resolve HelloService" << endl;
       return 1;
    }
cout << "references resolved" << endl;
        
    if( is_nil(obj) )
    {
      cerr << argv[0] << ": HelloService is a nil object reference" << endl;
      return 1;
    }

    Tests::Test1_var tst = Tests::Test1::_narrow(obj);
    if( is_nil(tst) )
    {
      cerr << argv[0] << ": can't narrow dMManager to correct type" << endl;
      return 1;
    } 
cout << "manager narrowed" << endl;

    tst->hello_CORBA();

cout << "...->hello_CORBA() called. Watch the server!" << endl;

    myORB->destroy();
  }
  catch(SystemException& ex)
  {
#if defined(ORBACUS) && !defined(OB_INTEGER_VERSION)
    OBPrintException(ex);
#else
#if defined(ORBACUS) && (OB_INTEGER_VERSION < 3999951L)
    OBPrintException(ex);
#else
    cerr << ex << endl;
#endif
#endif
    return 1;
  }
cout << "TestClient finished" << endl;
  return 0;
}
