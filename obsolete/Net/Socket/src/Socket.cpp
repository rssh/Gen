/*
 * Socket : socket processing.  
 * (C) GradSoft, 2000, 2001
 * http://www.gradsoft.com.ua
 * $Id:
 */

#include <GradSoft/Socket.h>

void Socket::throw_error( const char* funct, int err_no ) throw(SocketError)
{
     sock_err_.why  = funct; 
     sock_err_.why += (const char*) strerror(err_no); 
     throw(sock_err_);
}

int  
Socket::constr_name( struct sockaddr_in& addr, const char* hostnm, int port)
                                                         throw(SocketError)
{
    addr.sin_family = sock_domain_; 
    addr.sin_port   = htons(port);
    if ( !hostnm ) { 
        addr.sin_addr.s_addr = INADDR_ANY;
    } else {
//#ifdef _MSC_VER
      // struct hostent *host = gethostbyname(hostnm);
      // if (hp == NULL) return 0; 
      // memcpy((char*) &addr.sin_addr, (char*) host->h_addr, host->h_length);
//#else
      if ( inet_aton( hostnm, &addr.sin_addr ) == 0) {
          struct hostent *host;
          host = gethostbyname( hostnm );
          if ( host == NULL )  {
             sock_err_.why  = "gethostbyname():";
             sock_err_.why +=  hstrerror(h_errno);
             throw( sock_err_ );
          }
          memcpy((char*) &addr.sin_addr,
                 (char*) *host->h_addr_list, host->h_length);
      }
//#endif
    } 
    return ( sizeof(addr) );
}

Socket:: Socket( const Socket& sock ) throw(SocketError)
{
#ifdef _MSC_VER
       increment();
#endif
      sock_domain_   = sock.sock_domain_;
      sock_type_     = sock.sock_type_;
      sock_protocol_ = sock.sock_protocol_;

#ifndef _MSC_VER
      sock_desc_ = ::dup( sock.sock_desc_ );
#else
#error Not implemented
#endif
      if (sock_desc_ == -1) 
          throw_error("dup()",errno);
};

void Socket::operator=( const Socket& sock ) throw(SocketError)
{
    if (&sock == this) return;
    sock_domain_ = sock.sock_domain_;
    sock_type_ = sock.sock_type_;
    sock_protocol_ = sock.sock_protocol_;

#ifndef _MSC_VER
    sock_desc_ = ::dup( sock.sock_desc_ );
#else 
#error Not implemented
#endif
    if (sock_desc_ == -1) 
          throw_error("dup()",errno);
}

#ifdef _SOCKET_SERVER_
void  Socket::bind( int port ) throw(SocketError)
{
    int len ;

    struct sockaddr_in addr;
    len = constr_name( addr, NULL, port );
    if ( ::bind( sock_desc_, (struct sockaddr*) &addr, len )<0 ) {
       if ( ::getsockname(sock_desc_, 
                         (struct sockaddr*) &addr,(socklen_t*) &len )<0)
          throw_error("bind()",errno);
    }
};
#endif

#ifdef _SOCKET_SERVER_
void  Socket::listen( int backlog = 5 )  throw(SocketError)
{
    if ( (::listen( sock_desc_, backlog )) !=0  )
 			throw_error("listen()",errno);
}
#endif

#ifdef _SOCKET_SERVER_
Socket  Socket::accept() throw(SocketError)
{
    Socket sock;  
    sock.sock_domain_   = sock_domain_;
    sock.sock_type_     = sock_type_;
    sock.sock_protocol_ = sock_protocol_;

    if ( ( sock.sock_desc_ = ::accept(sock_desc_,NULL,NULL) ) < 0 ) 
          	throw_error("accept()",errno);
    return Socket(sock);
}
#endif


#ifdef _SOCKET_CLIENT_
void  Socket::connect( const char* hostname, int port ) throw(SocketError)
{
    struct sockaddr_in addr;
    int len = constr_name( addr, hostname, port ); 
    if ( ::connect( sock_desc_, (struct sockaddr*)&addr, len )>0 ) 
     	  				throw_error("accept()",errno);
}
#endif

int Socket::write(const char* buf,int len, int flag = 0) throw(SocketError)
{
    int send_bytes;
    send_bytes= ::send(sock_desc_, buf, len, flag);
    if ( send_bytes == -1 ) throw_error("write()",errno);
    return send_bytes;
}

int Socket::read(char* buf, int len, int flag = 0) throw(SocketError)
{
    int recieved_bytes;
    recieved_bytes = ::recv(sock_desc_, buf, len, flag);
    if ( recieved_bytes < 0 ) throw_error("read()",errno);
    else buf[recieved_bytes]='\0';
    return recieved_bytes;
}


const char* Socket::name()
{
    const int max_size = 1024;
    static char  my_name[ max_size ];
    if ( gethostname( my_name, max_size ) == -1) 
                     throw_error("gethostname()", errno );
    return my_name;
}


#ifdef _SOCKET_SERVER_
void Socket::client_info( char* hostnm, size_t size, int& port ) 
                                              throw(SocketError)
{
    int len;
    struct sockaddr_in addr;
    struct hostent *host;

    len = sizeof(addr);
    if (getpeername(sock_desc_, (struct sockaddr *) &addr, 
                                (socklen_t *) &len) < 0)
                                throw_error("getpeername()", errno );
    else { 
       if ((host = gethostbyaddr((char *) &addr.sin_addr,
                                  sizeof(addr.sin_addr),
                                  AF_INET)) != NULL) 
       { strncpy(hostnm, host->h_name, size-1 ); hostnm[size-1]='\0'; }
       else throw_error("gethostbyaddr()", errno );
       port = ntohs(addr.sin_port);
    }
}
#endif


void  Socket::shutdown(int mode=2)  throw(SocketError)
{  
    if ( ::shutdown(sock_desc_,mode) != 0 ) 
          throw_error("shutdown()", errno );
}
