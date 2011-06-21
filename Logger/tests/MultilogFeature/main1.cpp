#include <iostream>

#include <GradSoft/Logger.h>

/*
 *  Test example to show multilog logger feature
 *  $Id: main1.cpp,v 1.1 2002-08-27 17:03:34 rin Exp $
 *
 */

int main(int argc, char** argv)
{
   std::cerr << "Opening log file \"main.log\" and logging middle message... " << std::endl;

   GradSoft::Logger logger;
   logger.setOutputFile("main.log");

   logger.infos() << "Middle message." << std::endl;

   std::cerr << "Logging middle message complete, now resume first program.";
   return 0;
}


