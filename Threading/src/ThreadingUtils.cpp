#include <GradSoft/ThreadingUtils.h>

#ifndef __GradSoft_ThreadingExceptions_h
#include <GradSoft/ThreadingExceptions.h>
#endif

#include <sstream>

#ifndef WIN32
#include <errno.h>
#endif

namespace GradSoft {

#ifdef WIN32

void StrExTranslator::se_translator(
  unsigned int errCode, EXCEPTION_POINTERS* )
{
  const char mess[]="system error occured";
  switch (errCode) {
  case STATUS_NO_MEMORY:
    {
    ThreadingExceptions::NoMemory ex;
    ex.setData(errCode,"NO MEMORY");
    throw ex;
    }
  default :
    {
    ThreadingExceptions::SystemError ex;
    ex.setData(errCode,"system Error, WIN32Code");
    throw ex;
    }
  }
}

void ThreadingError::lastErrorNotification()
{ 
  DWORD errCode=GetLastError();
  if (errCode==ERROR_SUCCESS) return;
  char* message;
  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    errCode,
    MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL),
    (LPTSTR) &message,
    0,
    NULL 
  );
  switch (errCode) { 
  case ERROR_NOT_ENOUGH_MEMORY: 
    {
    ThreadingExceptions::NoResources ex;
    ex.setData(errCode,message);
    LocalFree(message);
    throw ex;
    }
  case ERROR_NOT_ENOUGH_QUOTA:
    {
    ThreadingExceptions::NoResources ex;
    ex.setData(errCode,message);
    LocalFree(message);
    throw ex;
    }
  case ERROR_OUTOFMEMORY: 
    {
    ThreadingExceptions::NoMemory ex;
    ex.setData(errCode,message);
    LocalFree(message);
    throw ex;
    }
  case ERROR_ACCESS_DENIED: 
    {
    ThreadingExceptions::NoPermission ex;
    ex.setData(errCode,message);
    LocalFree(message);
    throw ex;
    }
  case ERROR_BUSY: 
    {
    ThreadingExceptions::ResourceBusy ex;
    ex.setData(errCode,message);
    LocalFree(message);
    throw ex;
    }
  case ERROR_CALL_NOT_IMPLEMENTED:
    {
    ThreadingExceptions::InternalError ex;
    ex.setData(errCode,message);
    LocalFree(message);
    throw ex;
    }
  default: 
    {
    ThreadingExceptions::SystemError ex;
    ex.setData(errCode,message);
    LocalFree(message);
    throw ex;
    }
  }  
}

StrExTranslator::StrExTranslator()
{
 previous_=_set_se_translator(se_translator);
}

StrExTranslator::~StrExTranslator()
{  
 _set_se_translator(previous_);
}


namespace ThreadingWrapper
{
  void InitializeCriticalSection(LPCRITICAL_SECTION parameter)
  {
    DWORD errCode=0;
    bool abnormal=false;
    _se_translator_function previous;
    previous=_set_se_translator(NULL);
    __try {
      ::InitializeCriticalSection(parameter);
    }
    __except (errCode=GetExceptionCode(),1){
      abnormal=true;
    }
    _set_se_translator(previous);	  
    if (abnormal){ 
      StrExTranslator::se_translator(errCode,NULL);
    }
  }
}

#else 

void ThreadingError::checkError(int errorCode, const char* name, bool ownMessage)
{
  if (errorCode==0) return;
  std::ostringstream ostr;
  if (ownMessage) {
    ostr << name << std::endl;
  }else{
    ostr << name << " failed with code " << errorCode << '\0';
  }
  switch(errorCode) {
    case 0: return;
    case EBUSY:
      {
      ThreadingExceptions::ResourceBusy ex;
      ex.setData(errorCode,ostr.str().c_str());
      throw ex;
      }
    case EPERM:
      {
      ThreadingExceptions::NoPermission ex;
      ex.setData(errorCode,ostr.str().c_str());
      throw ex;
      }
    case ENOMEM:
      {
      ThreadingExceptions::NoMemory ex;
      ex.setData(errorCode,ostr.str().c_str());
      throw ex;
      }
    case EDEADLK:
      {
      ThreadingExceptions::PossibleDeadlock ex;
      ex.setData(errorCode,ostr.str().c_str());
      throw ex;
      }
    case EAGAIN:
      {
      ThreadingExceptions::TemporaryNoResources ex;
      ex.setData(errorCode,ostr.str().c_str());
      throw ex;
      }
    case EINVAL:
      {
      ThreadingExceptions::InternalError ex;
      ex.setData(errorCode,ostr.str().c_str());
      throw ex;
      }
    default:
      {
      ThreadingExceptions::InternalError ex;
      ex.setData(errorCode,"");
      throw ex;
      }
  }
}
#endif

}
