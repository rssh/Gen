#include <demo_handler.h>

#ifdef WIN32
#include <windows.h>
#define pause()  WaitForSingleObject(__ctrl_evnt_for_pause, INFINITE)
#endif

static demo_handler myhandler;
static demo_handlerNew myhandler_new;

using namespace GradSoft;

int main(int argc, char* argv[])
{
    try {
       Signaling demo1( SignalSet::sSIGINT , myhandler );
       std::cout << "Press Ctrl+C" << std::endl;
       pause();
       std::cout << "Press Ctrl+C again" << std::endl;
       pause();
       std::cout << "And again" << std::endl;
       pause();
       {
          Signaling demo2( SignalSet::sENABLE_ALL, myhandler_new );
          std::cout << "Kill process with some signal:" << std::endl;
          pause();
       }
       std::cout << "Press Ctrl+C" << std::endl;
       pause();
    } catch ( const Signaling::Error& err ) {
      std::cerr << "Error: " << err.why << std::endl;
    }
    return 0;
}
