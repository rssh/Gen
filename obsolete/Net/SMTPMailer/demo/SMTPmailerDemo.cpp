#include <GradSoft/SMTPMailer.h>

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
   const int addr_len = 50;
   char addr[addr_len];
   if (argc < 3) { 
      cerr << "Usage : SMTPMailerDemo <snmp-server ip address> <e-mail address>"
           << endl;
      return 0;
   }
   if (strlen(argv[2])<addr_len)  strncpy(addr,argv[2],addr_len); 
   else return 0;
   {
   SMTPMailer mailer(argv[1]);
   try {
      mailer.add_recipient(addr);
      mailer.add_recipient("srkr@mail.grad.kiev.ua", "Sergey Krysanov");
      mailer.sender("srkr@mail.grad.kiev.ua", "Automat");
      mailer.subject("subject");
      mailer.message() << "Subject : SUBJECT\n"
               << "TO : \"Sergey\" <srkr@mail.grad.kiev.ua>\n"
               << "Hello world !!!\nBye !!!"
               << "\n....\n"
               << "..\n.\n.";
      mailer.send();
   } catch(const SMTPMailer::SMTPError& ex) {
      cerr << "Error:" <<  ex.error << endl;
   }
   }
   return 0;
}
