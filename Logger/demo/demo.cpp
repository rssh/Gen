
#define LOG_DEBUG_ENABLE true

#define LOG_INFO_ENABLE true

#include <GradSoft/Logger.h>


#include <iostream>
using namespace std;

void debug_callback(const char* msg)
{
 cerr << "debug_callback:" << msg ;
}

int main(int argc, char** argv)
{
 try {
  GradSoft::Logger logger("file.log");
  logger.setCallback(GradSoft::Debug,debug_callback);

  logger.debugs() << "debug output 1 for " << argv[0] << endl; 
  logger.infos() << "QQQQ" << endl;
  logger.infos() << "QQQQ1" << endl;
  logger.debugs() << argv[0] << endl;
  logger.debugs() << "debug output 2 for " << argv[0] << endl; 
  logger.debugs() << "debug output " << 3 << " for " << argv[0] << endl; 
  logger.debugs() << " short debug output " << endl; 
 }catch(GradSoft::Logger::IOException ex){
   cerr << argv[0] << ":can't open log file" << endl;
   return 1;
 }

 return 0;
}

