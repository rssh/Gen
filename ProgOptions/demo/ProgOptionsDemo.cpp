#include <GradSoft/ProgOptions.h>

using namespace GradSoft;
using namespace std;

ProgOptions options;

bool zz1Callback(const ProgOptions* options, const char* argument,
                 void* )
{
 cout << "zz1:callback called with argument " << 
            ((!argument) ? "NULL" : argument) << endl;
 return true;
}

void init()
{
 options.put("qqq", "qqq option", false );
 options.put("zzz", "option with argument", true );
 options.put("zz1", "option with argument and callback", true, zz1Callback );
 options.put("zz3", "zz3 option", false, zz1Callback );
};

int main(int argc, char** argv)
{
 init();
 options.setAdditionalHelp(true,
   "This program illustate usage of GradSoft library"
 );
 options.setAdditionalHelp(false,
   "and this is shown at the end of usage screen"
 );
 if (!options.parse(argc,argv)) return 1;
 if (options.is_set("qqq")) {
   cout << "qqq is set" << endl;
 }
 if (options.is_set("zzz")) {
   cout << "zzz is set with argument:" << options.argument("zzz") << endl;
 }
 if (options.is_set("zz1")) {
   cout << "zz1 is set with argument:" << options.argument("zz1") << endl;
 }
 if (options.is_set("zz3")) {
   cout << "zz3 is set " << endl;
 }
 //options.saveToFile("zzz");
 return 0;
}
