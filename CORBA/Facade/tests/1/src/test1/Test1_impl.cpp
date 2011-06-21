// $Id: Test1_impl.cpp,v 1.2 2000-08-03 09:41:14 alik Exp $

#ifndef __test1_Test1_impl_h
#include <test1/Test1_impl.h>
#endif

#ifdef _MSC_VER
using namespace std;
#endif

void Test1_impl::hello_CORBA()
{ cout << "Hello, CORBA! Persistent POA." << endl; }

void Test1_impl::destroy()
{ 
  cout << "destroy()" << endl;
  CORBAFacade::deactivate_object(this);
}
