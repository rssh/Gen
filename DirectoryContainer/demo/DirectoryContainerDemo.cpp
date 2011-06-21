#include <GradSoft/DirectoryContainer.h>

#include <iostream>
using namespace std;
using namespace GradSoft;

#include <stdio.h>

int main(int argc, char** argv)
{
  if (argc!=2) {
    cerr << "Usage: " << argv[0] << " <path>" << endl;
    return 1;
  }

  cout<<endl<<">>>> 1 >>>>"<<endl;

  // Usinig DirectoryEntry:

  try{

    DirectoryEntry smth(argv[1]);

    do {

     cout<<smth.name()<<endl;

    } while(smth.next());

  } catch(const DirectoryException& ex) {

    cerr << ex.what() << endl;
  }

  cout<<endl<<">>>> 2 >>>>"<<endl;

  // Using DirectoryContainer:

  try{

    DirectoryContainer dir(argv[1]);

    DirectoryContainer::iterator it;

    for( it=dir.begin(); it!=dir.end(); ++it)
    {
      cout << it->name() ;
      if ( it->is_directory() ) cout << " ( DIR ) ";
      else cout << " ( " << it->fsize() << " ) ";
      cout << endl;
    }

  } catch(const DirectoryException& ex) {

    cerr << ex.what() << endl;
  }
  return 0;
}


