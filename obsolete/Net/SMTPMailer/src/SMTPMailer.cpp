#include <GradSoft/SMTPMailer.h>

#include <iostream>
#include <fstream>
#include <cstring>
#include <assert.h>

using namespace std;

SMTPMailer::SMTPMailer(const char* smtp, int port)
   : sock_(AF_INET,SOCK_STREAM), port_(port)
{
   strcpy( smtp_server_, smtp );
   sock_.connect( smtp_server_, port_ );
   sender_mail_ = NULL;
   sender_name_ = NULL;
   subject_ = NULL;
}


SMTPMailer::SMTPMailer(const SMTPMailer& mailer)
   : sock_(AF_INET,SOCK_STREAM), port_(mailer.port_)
{
   strcpy( smtp_server_, mailer.smtp_server_  );
   sock_.connect( smtp_server_, port_ );
   sender_mail_ = NULL;
   if (mailer.sender_mail_ != NULL){
     sender_mail_ = new char[strlen(mailer.sender_mail_)+1];
     strcpy(sender_mail_, mailer.sender_mail_);
   }
   sender_name_ = NULL;
   if (mailer.sender_name_ != NULL){
     sender_name_ = new char[strlen(mailer.sender_name_)+1];
     strcpy(sender_name_, mailer.sender_name_);
   }
   subject_ = NULL;
   if (mailer.subject_ != NULL){
     subject_ = new char[strlen(mailer.subject_)+1];
     strcpy(subject_, mailer.subject_);
   }
   if (mailer.recipients_.size()>0){
     int i=0;
     for (i=0; i<mailer.recipients_.size(); i++){
       recipients_.push_back(mailer.recipients_[i]);
       recipients_names_.push_back(mailer.recipients_names_[i]);
     }
   }
   if (mailer.recipients_bcc_.size()>0){
     int i=0;
     for (i=0; i<mailer.recipients_bcc_.size(); i++){
       recipients_bcc_.push_back(mailer.recipients_bcc_[i]);
       recipients_bcc_names_.push_back(mailer.recipients_bcc_names_[i]);
     }
   }
//   string message = mailer.message_content();
   msg_ << mailer.msg_.str();
}

SMTPMailer::~SMTPMailer()
{
  msg_.rdbuf()->freeze(0);
  recipients_.clear();
  if (sender_mail_ != NULL)
    delete[] sender_mail_;
  if (subject_ != NULL)
    delete[] subject_;
  if (sender_name_ != NULL)
    delete[] sender_name_;
}

void SMTPMailer::sender(const char* sender_mail, const char* sender_name)
{
  assert(sender_mail);
  if (sender_mail_ != NULL)
    delete[] sender_mail_;
  sender_mail_ = new char[strlen(sender_mail)+1];
  strcpy(sender_mail_, sender_mail);
  if (sender_name_ != NULL){
    delete[] sender_name_;
    sender_name_ = NULL;
  }
  if (sender_name != NULL){
    sender_name_ = new char[strlen(sender_name)+1];
    strcpy(sender_name_, sender_name);
  }
}

void SMTPMailer::subject(const char* text)
{
  assert(text);
  if (subject_ != NULL)
    delete[] subject_;
  subject_ = new char[strlen(text)+1];
  strcpy(subject_, text);
}

void SMTPMailer::add_recipient(const char* recipient_mail, const char* recipient_name)
{
  assert(recipient_mail);
  recipients_.push_back(recipient_mail);
  string tmp;
  if (recipient_name != NULL){
    tmp = recipient_name;
  }else{
    tmp = "";
  }
  recipients_names_.push_back(tmp);
}


void SMTPMailer::add_recipient_bcc(const char* recipient_mail, const char* recipient_name)
{
  assert(recipient_mail);
  recipients_bcc_.push_back(recipient_mail);
  string tmp;
  if (recipient_name != NULL){
    tmp = recipient_name;
  }else{
    tmp = "";
  }
  recipients_bcc_names_.push_back(tmp);
}


void SMTPMailer::send() throw(SMTPError)
{
  if (sender_mail_ == NULL){
      strcpy(mail_err_.error, "Required sender");
      throw(mail_err_);
   }

  if (recipients_.size()== 0){
      strcpy(mail_err_.error, "At least one recipient required");
      throw(mail_err_);
   }

   const unsigned short int array_size = 256;
   char str[ array_size ];

   initMail();

cerr << "message body." << endl;

   ostrstream tmp;

   tmp << "MAIL FROM:" << sender_mail_ << "\n" << '\0';
   sock_.write( (const char*) tmp.str(), strlen(tmp.str()) );
   tmp.rdbuf()->freeze(0);
   tmp.rdbuf()->pubseekpos(0,ios::out);


   sock_.read(str,array_size-1);
   if ( strstr((const char*) str,"250")==NULL ) {
      strcpy((char*) mail_err_.error,(const char*) str);
      cout << str << endl;
      throw(mail_err_);
   }

   int i=0;
   for (i=0; i<recipients_.size(); i++){
       tmp << "RCPT TO:" << recipients_[i].data() << "\n" << '\0';
       sock_.write( (const char*) tmp.str(), strlen(tmp.str()) );
       tmp.rdbuf()->freeze(0);
       tmp.rdbuf()->pubseekpos(0,ios::out);

       sock_.read(str,array_size-1);
       if ( strstr((const char*) str,"250")==NULL ) {
          strcpy((char*) mail_err_.error,(const char*) str);
          throw(mail_err_);
       }
   }

   strcpy(str,"DATA\n");
   sock_.write( (const char*) str, strlen(str) );

   sock_.read(str,array_size-1);
   if ( strstr((const char*) str,"354")==NULL ) {
      strcpy(mail_err_.error,(const char*) str);
      throw(mail_err_);
   }
   has_endl_ = true;


   const char* message;
   tmp << "TO:" << recipients_names_[0].data() << " <" << recipients_[0].data() << ">";
   for (i=1; i<recipients_.size(); i++)
     tmp << "," << recipients_names_[i].data() << " <" << recipients_[i].data() << ">";
   tmp << "\n" << '\0';
   message = tmp.str();
   tmp.rdbuf()->freeze(0);
   tmp.rdbuf()->pubseekpos(0,ios::out);

   write(message);

   if (recipients_bcc_.size() > 0)
   {
     tmp << "bcc:" << recipients_bcc_names_[0].data() << " <" << recipients_bcc_[0].data() << ">";
     for (i=1; i<recipients_bcc_.size(); i++)
        tmp << "," << recipients_bcc_names_[i].data() << " <" << recipients_bcc_[i].data() << ">";
     tmp << "\n" << '\0';
     message = tmp.str();
     tmp.rdbuf()->freeze(0);
     tmp.rdbuf()->pubseekpos(0,ios::out);
     write(message);
   }


   
   tmp << "FROM:" << sender_name_ << " <" << sender_mail_ << ">\n" << '\0';

   message = tmp.str();
   tmp.rdbuf()->freeze(0);
   tmp.rdbuf()->pubseekpos(0,ios::out);

   write(message);

   
   if (subject_ != NULL){
     tmp << "Subject:" << subject_ << "\n" << '\0';
     message = tmp.str();
     write(message);
   }
   tmp.rdbuf()->freeze(0);

// Write end of header see RFC822
   message = "\n\n";
   write(message);

   msg_ << '\0';
   message = msg_.str();
   if (message != NULL){
     write(message);
   }

   const char* exit_message="\n.\n";
   sock_.write( exit_message, strlen(exit_message) );

   sock_.read(str,array_size-1);
   if ( strstr((const char*) str,"250")==NULL ) {
      strcpy((char*) mail_err_.error,(const char*) str);
      throw(mail_err_);
   }
   strcpy(str,"QUIT\n");
   sock_.write( (const char*) str, strlen(str) );

   sock_.read(str,array_size-1);
   if ( strstr((const char*) str,"221")==NULL ) {
      strcpy(mail_err_.error,(const char*) str);
      throw(mail_err_);
   }
}



void SMTPMailer::readFromFile(const char* filename) throw(SMTPError)
{
   ifstream  source(filename);
   const unsigned short int array_size = 256;
   char str[ array_size ];
   if (!source) {
     strcpy(mail_err_.error,"File "); strncat(mail_err_.error,filename,
             ERROR_LEN - 16);
     strcat(mail_err_.error," not exist.");
     throw(mail_err_);
   }
   while ( ! source.eof() ) {
      source.getline(str,array_size-1);
      this->message() << str << "\n";
   }
}

void SMTPMailer::write(const char* message) throw(SMTPError)
{
    if (*message != '\0') {
        if (has_endl_ && *message == '.') {
            sock_.write("..", 2);
            message++;
        }
        const char* endPos;
        for (endPos = message;*endPos != '\0'; ++endPos) {
            if (endPos[0] == '\n' && endPos[1] == '.') {
                sock_.write(message, endPos - message);
                sock_.write("\n..", 3);
                message = ++endPos + 1;
            }
        }
        if (endPos > message) sock_.write(message, endPos - message);
        has_endl_ = (endPos[-1] == '\n');
    }
}

void SMTPMailer::initMail() throw(SMTPError)
{
   const unsigned short int array_size = 256;
   char str[ array_size ];
   memset(str,'\0',array_size);
   sock_.read(str,array_size-1);
   if ( strncmp(str,"220",3) ) {
      strncpy(mail_err_.error,(const char*) str, ERROR_LEN);
      throw(mail_err_);
   }
   strcpy(str,"HELO "); strcat(str,smtp_server_); strcat(str,"\n");
   sock_.write( (const char*) str, strlen(str) );

   sock_.read(str,array_size-1);
   if ( strncmp(str,"250",3) ) {
      strncpy((char*) mail_err_.error,(const char*) str, ERROR_LEN);
      throw(mail_err_);
   }
}
