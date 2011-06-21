#ifndef GradSoft_SOCKET_H
#define GradSoft_SOCKET_H


/*
 * Socket : socket processing.  
 * (C) GradSoft, 2000, 2001
 * http://www.gradsoft.com.ua
 * $Id:
 */

#ifndef _MSC_VER
#include <SocketConfig.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_STRING
#include <string>
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#else

#include <winsock.h>
#include <iostream>
#include <io.h>
#define HAVE_BOOL
typedef int FAR socklen_t;

#endif

///
class SocketError {
public:
      /** String describing error code.
       \begin{verbatim}
       class SocketError {
       public:
           string why;
       };
       \end{verbatim}
      */
      string why; 
};

///
class Socket {
private:
   int  sock_desc_;              // socket file descriptor
   int  sock_type_;
   int  sock_domain_;
   int  sock_protocol_;
   SocketError sock_err_;


   void throw_error( const char* funct, int err_no ) throw(SocketError);

   int  constr_name( struct sockaddr_in& addr, const char* hostnm, int port)
                                                          throw(SocketError);
  
 
public:

   /// Default constructor.
   Socket(): sock_domain_(0), sock_type_(0), sock_protocol_(0), sock_desc_(-1)
   {}
  
   /**
    * Constructor.
    * @param domain defines the protocol family
    * @param type specifies  the semantics of communication
    * @param protocol specifies  a particular protocol to be used with the socket
    **/
   Socket(int domain, int type, int protocol = 0) throw(SocketError)
   : sock_domain_(domain), sock_type_(type),  sock_protocol_(protocol)  
   {
#ifdef _MSC_VER
       increment();
#endif
      sock_desc_ = ::socket( sock_domain_, sock_type_, sock_protocol_ );
      if (sock_desc_ == -1) 
          throw_error("socket()",errno);
   };

   /// Copy constructor.
   Socket( const Socket& sock ) throw(SocketError);

   ///
   void operator=( const Socket& sock ) throw(SocketError);

   /**
    * Bind a name to a socket.
    * @param port defines a port to a socket.
    **/
#ifdef _SOCKET_SERVER_
   void  bind( int port ) throw(SocketError);
#endif

   /**
    * Listen for connections on a socket.
    * @param backlog defines the maximum length the queue 
    *                of  pending  connections  may  grow  to.
    **/

#ifdef _SOCKET_SERVER_
   void listen( int backlog = 5 )  throw(SocketError);
#endif

   /**
    * Accept a connection on a socket.
    * @return the accepted socket.
    **/

#ifdef _SOCKET_SERVER_
   Socket  accept() throw(SocketError);
#endif

   /**
    * Initiate a connection on a socket.
    * @param hostnm specifies the host name of the server.
    * @param port specifies the port of the server.
    **/

#ifdef _SOCKET_CLIENT_
   void  connect( const char* hostname, int port ) throw(SocketError);
#endif

   /**
    *  Send a message from a socket.
    *  @param buf reference to input buffer.
    *  @param len buffer's length.
    *  @param flag may be one of the following
       \begin{verbatim}
       0              Nothing.
       MSG_OOB        Sends out-of-band data on sockets.
       MSG_DONTROUTE  Bypasses  the  usual routing table lookup and sends 
                      the packet directly to the interface  described  by  
                      the  destination  address.
       MSG_DONTWAIT   Enables non-blocking operation 
       MSG_NOSIGNAL   Requests  not  to  send SIGPIPE on errors on stream
                      oriented sockets when the other end breaks the 
                      connection.
        \end{verbatim}
    **/

   int write(const char* buf,int len, int flag = 0) throw(SocketError);

   /**
    *  Receive a message from a socket.
    *  @param buf reference to input buffer.
    *  @param len buffer's length.
    *  @param flag may be one of the following (see write()).
    *  @see write().
    **/
   
   int read(char* buf, int len, int flag = 0) throw(SocketError);

    /**
     * Returns the name of the computer that program is running on.
     * @return computer's name.
     **/

   const char* name();

    /**
     * Return client's hostname and port 
     * @param hostnm client's host name.
     * @param size of array  hostnm
     * @param port client's port.
     */ 

#ifdef _SOCKET_SERVER_
   void client_info( char* hostnm, size_t size, int& port ) throw(SocketError);
#endif

    /**
     *  Shut down part of a full-duplex connection.
     *  @param mode may be one of the following.
        \begin{verbatim}
          0 - further receives will be disallowed.
          1 - further sends will be disallowed.
          2 - further sends and receives will be disallowed.
        \end{verbatim}
     **/

   void  shutdown(int mode=2)  throw(SocketError);

   ~Socket() {
       close(sock_desc_);
#ifdef _MSC_VER
       decrement();
#endif
   };

#ifdef _MSC_VER
private:

    static long& counter() {
       static long counter_ = 0;
       return counter_;
    };

    static void increment() {
        if (counter() == 0) {
            //
            // Winsock startup
            //
            WORD wVersionRequested = MAKEWORD(1, 1);
            WSADATA wsaData;
            if(WSAStartup(wVersionRequested, &wsaData) != 0)
            {
                std::cerr << "Can not find usable winsock dll" << std::endl;
            }
        }
        counter()++;
    };

    static void decrement() {
        counter()--;
        if (counter() == 0) {    //
            // Winsock shutdown
            //
            if(WSACleanup() != 0)
            {
                std::cerr << "Can not cleanup WinSockA" << std::endl;
            }

        }
    };
#endif
};
#endif
