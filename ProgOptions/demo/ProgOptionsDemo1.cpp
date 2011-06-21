#include <GradSoft/ProgOptions.h>

using namespace GradSoft;
using namespace std;

ProgOptions options;



int main(int argc, char** argv)
{
 options.put("qqq", "qqq option", false );
 if (!options.parse(argc,argv)) return 1;
 if (options.is_set("qqq")) {
   cout << "qqq is set" << endl;
 }
 return 0;
}


