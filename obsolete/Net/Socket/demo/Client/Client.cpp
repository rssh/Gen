#include <iostream.h>
#include <stdlib.h>

#include <GradSoft/Client.h>
#include <GradSoft/Socket.h>


static const char hello_msg[20]="Hello server !!!";


int main(int argc, char* argv[])
{
   int port;
   char buf[80],*errpos;

   if (argc < 3) { 
      cerr << "Usage : Client <host> <port>" << endl;
      return 0;
   }

   port = strtoul(argv[2],&errpos,10);
   if ( (*errpos)  ) {
       cerr << "Error : Parameter <port> must be interger." << endl;
       return 0;
   }

   try { 
     Socket sock(AF_INET,SOCK_STREAM);

     sock.connect(argv[1],port);
     sock.read(buf, sizeof(buf));

     cerr << "[Server reply]: " << buf << endl;
     cerr  << "Send message to server" << endl;

     sock.write(hello_msg, sizeof(hello_msg));
   } catch( const SocketError& ex ) {
     cerr << ex.why << endl;
   }
   return 0;
}
