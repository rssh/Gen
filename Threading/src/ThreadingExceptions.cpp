#include <GradSoft/ThreadingExceptions.h>

#include <string>
#include <sstream>


namespace GradSoft {


ThreadingExceptions::Failure::Failure()
  :std::runtime_error(std::string("Threading:Failure")),
   errorCode_(0),errorMessage_("Threading:Failure"),
   codeIsAddedToMessage_(false)
{
}

ThreadingExceptions::Failure::Failure(int code, const char* message)
 :std::runtime_error(std::string(message)),errorCode_(code),
  errorMessage_(message),codeIsAddedToMessage_(false)
{ 
}

ThreadingExceptions::Failure::Failure(int code, const std::string& message)
 :std::runtime_error(message),errorCode_(code),
  errorMessage_(message),codeIsAddedToMessage_(false)
{ 
}


ThreadingExceptions::Failure::Failure(const Failure& ex)
 :std::runtime_error(ex)
{
  errorCode_ = ex.errorCode_; 
  errorMessage_ = ex.errorMessage_;
  codeIsAddedToMessage_ = ex.codeIsAddedToMessage_;
}

ThreadingExceptions::Failure& ThreadingExceptions::Failure::operator=(const ThreadingExceptions::Failure& ex)
{
  errorCode_ = ex.errorCode_; 
  errorMessage_ = ex.errorMessage_;
  codeIsAddedToMessage_ = ex.codeIsAddedToMessage_;
  return *this;
}


const char* ThreadingExceptions::Failure::what() const throw()
{
 if (!codeIsAddedToMessage_) {
   std::ostringstream ostr;
   ostr << errorMessage_ << ", OS code = " << errorCode_ << '\0';
   errorMessage_ = ostr.str(); 
   codeIsAddedToMessage_ = true;
 }
 return errorMessage_.c_str();
}

void ThreadingExceptions::Failure::setData(unsigned int code, const char* data)
{
 std::ostringstream ostr;
 ostr << data << ", OS code = " << code << '\0';
 errorMessage_ = ostr.str(); 
 codeIsAddedToMessage_ = true;
}

ThreadingExceptions::Failure::~Failure() throw()
{
}

}//namespace GradSoft
