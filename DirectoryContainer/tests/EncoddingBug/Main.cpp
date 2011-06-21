#include <iostream>
#include <string>

#include <GradSoft/Logger.h>
#include <GradSoft/DirectoryContainer.h>

#define LOG_INFO_ENABLE true

// $Id: Main.cpp,v 1.1 2002-08-29 15:10:33 rin Exp $

int main(int argc, char** argv)
{
   GradSoft::Logger  log;
   try{
      GradSoft::DirectoryEntry  smth("cyrilic");
      do {
         std::cerr << smth.name() << std::endl;
         std::string s = "cyrilic/";
         s += smth.name();
         if (strcmp(smth.name(),".")&&strcmp(smth.name(),"..")) {
            std::cerr << " Logging to file \"" << s.c_str()
                      << "\" its filename  (encoding ÂÈÍÄÎÑ / „Ž‘ / ëïé8 )." << std::endl;
            log.setOutputFile(s.c_str());
            log.infos() << smth.name() << std::endl;
         }
      } while (smth.next());
   } catch (const GradSoft::DirectoryException& ex) {
      std::cerr << ex.what() << std::endl;
   }
   return 0;
}


