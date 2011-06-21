#include <SocketConfig.h>
#include <iostream.h>
#include <errno.h>

#include <GradSoft/Server.h>
#include <GradSoft/Socket.h>

#include <GradSoft/Threading.h>

#ifdef HAVE_NAMESPACES
using namespace GradSoft;
using namespace std;
#endif

const char hello_msg[20] = "Hello client !!!";

class ClientThread : public Thread
{
   public:
   Socket sock;

   void run()
   { 
       char buf[80],host[50];
       int port; 

       cerr << "Client accepted !!!" << endl;
     try {
       sock.write( ::hello_msg, sizeof(::hello_msg) );
       sock.client_info(host,sizeof(host),port);
       sock.read(buf, sizeof(buf));
       cerr << "[Client reply:" << host << ":" << port << "]"  << buf << endl;  
       sock.shutdown();
     } catch( const SocketError& ex ) {
       cerr << ex.why << endl;
     }
   }
};


int main(int argc,char* argv[])
{
    unsigned int port = 0;
    char* errpos;
    if ( argc < 2  ) {
	cerr << " Usage: Server <port>" << endl;  
	return 0;
    }
    try {
       Socket  sock( AF_INET, SOCK_STREAM );
 
       port = strtoul(argv[1],&errpos,10);
       if ( (*errpos)  ) {
	  cerr << "Error : Parameter <port> must be interger." << endl;
	  return 0;
       }
    
       sock.bind( port );
       sock.listen(3);

       for(;;)
       {
         ClientThread* client =  new ClientThread();
         client->sock = sock.accept();
         client->start();
       } 
       sock.shutdown();
    } catch ( const SocketError& ex ) {
      cerr << ex.why << endl;
    }
}
