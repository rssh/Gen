#ifndef GradSoft_SMTPmailer_h
#define GradSoft_SMTPMailer_h 

#define _SOCKET_CLIENT_

#include <GradSoft/Socket.h>
#include <strstream>
#include <vector>
#include <string>


using namespace std;

class SMTPMailer {
public:
   enum { ERROR_LEN = 100 };

   class SMTPError {
     public:
       char   error[ERROR_LEN + 1];
       SMTPError() {
           memset(error, '\0', ERROR_LEN + 1);
       }
   };
private:
   SMTPError  mail_err_;

   Socket sock_;
   char   smtp_server_[60];
   int    port_;

   bool has_endl_; // for period transparency

   mutable ostrstream msg_;

   vector<string> recipients_;
   vector<string> recipients_names_;
   vector<string> recipients_bcc_;
   vector<string> recipients_bcc_names_;

   char* sender_mail_;
   char* sender_name_;
   char* subject_;

private:
   void initMail() throw(SMTPError);

   void write(const char* message) throw(SMTPError);

public:

   SMTPMailer(const char* smtpServerHostname, int port=25);

   ~SMTPMailer();

   ostrstream& message() { return msg_; };

   void subject(const char* text);

   void sender(const char* sender_mail,
               const char* sender_name=NULL);

   void add_recipient(const char* recipient_mail,
                      const char* recipient_name=NULL);

   void add_recipient_bcc(const char* recipient_mail,
                          const char* recipient_name=NULL);

   void readFromFile(const char* filename) throw(SMTPError);

   void send() throw(SMTPError);

private:

   SMTPMailer(const SMTPMailer& mailer);

   SMTPMailer operator=(const SMTPMailer& mailer);

};

#endif
