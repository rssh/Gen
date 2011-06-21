
#include <iostream>

#include <GradSoft/ProgOptions>

#ifdef HAVE_NAMESPACES
using namespace GradSoft;
#endif

const char* login;
const char* password;
const char* db_link;
const char* db_drv;
const char* servlet_name;
const char* db_path;


ProgOptions options("--","",true);


int main(int argc, char* argv[], char* env[])
{
    options.put("login", "login", true);
    options.put("password", "password", true);
    options.put("db_link", "db_link", true);
    options.put("db_drv", "db_drv", true);
    options.put("servlet-name", "servlet-name", true);
    options.put("db_path", "db_path", true);

    if (!options.parse(argc, argv))
       return 1;

    if (!options.is_set("login") || !options.is_set("password")){
       cerr << "login and password is requered." << endl;
       options.outHelp(cout);
       return 0;
    }

    login = options.argument("login");
    cout << "login : " << login << endl;

    password = options.argument("password");
    cout << "password : " << password << endl;

    if (options.is_set("db_link")) {
       db_link = options.argument("db_link");
       cout << "db_link : " << db_link << endl;
    }
    if (options.is_set("db_drv")) {
       db_drv = options.argument("db_drv");
       cout << "db_drv : " << db_drv << endl;
    }
    if (options.is_set("servlet-name")) {
       servlet_name = options.argument("servlet-name");
       cout << "servlet_name : " << servlet_name << endl;
    }
    if (options.is_set("db_path")) {
       db_path = options.argument("db_path");
       cout << "db_path : " << db_path << endl;
    }

   return 0;

}

