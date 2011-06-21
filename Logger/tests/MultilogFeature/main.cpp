#include <iostream>

#include <GradSoft/Logger.h>

/*
 *  Test example to show multilog logger feature
 *  $Id: main.cpp,v 1.2 2002-08-29 10:06:29 rin Exp $
 *
 */

int main(int argc, char** argv)
{
   std::cerr << "Opening log file \"main.log\" and logging 1000 first messages... " << std::endl;

   GradSoft::Logger logger;
   logger.setOutputFile("main.log");

   for (long i=1; i<1001; i++) {
      logger.infos() << "First message number " << i << "." << std::endl;
   }

   std::cerr << "Now alt-tab and execute \"main1.exe\", then enter here a string... ";
   std::string str="";
   std::cin >> str;

   std::cerr << "Logging last message... " << std::endl;
   logger.infos() << "Last message." << std::endl;

   std::cerr << "Now see the log file \"main.log\". "
             << "Note that all first messages are lost now. "
             << "Also note that last message was added to the position, as if nothing happened. " << std::endl;
   return 0;
}


