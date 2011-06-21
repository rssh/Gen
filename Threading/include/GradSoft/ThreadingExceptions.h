#ifndef __GradSoft_ThreadingExceptions_h
#define __GradSoft_ThreadingExceptions_h


/*
 * GradSoft C++ ToolBox : Threading
 * (C) GradSoft, 2001
 * Ukraine, Kiev
 * http://www.gradsoft.com.ua
 * $Id: ThreadingExceptions.h,v 1.24 2004-03-13 09:53:25 rssh Exp $
 */

#ifndef __GradSoft_ThreadingPostConfig_h
#include <GradSoft/ThreadingPostConfig.h>
#endif

#ifndef __STDEXCEPT_
#include <stdexcept>
#ifndef __STDEXCEPT_
#define __STDEXCEPT_
#endif
#endif

#ifdef WIN32
#pragma warning( disable : 4290 ) 
#endif

/**
 *
 **/
namespace GradSoft {

/**
 * 'namespace' for Threading Exceptions definitions.
 **/
class __dll_threading ThreadingExceptions
{
public:

  /**
   * Base class for Threading Exceptions
   **/
  class Failure : public std::runtime_error
  {
  protected:
    friend class ThreadingError;
#ifdef WIN32
    friend class StrExTranslator;
#endif
    void setData(unsigned int errCode,const char* errMessage);
  //public:
    int errorCode_;
    mutable std::string errorMessage_;
    mutable bool codeIsAddedToMessage_;
  public:

    ///
    Failure();

    ///
    Failure(int code, const char* message);

    ///
    Failure(int code, const std::string& message);

    ///
    Failure(const Failure&);

    ///
    ~Failure() throw();


    ///
    Failure& operator=(const Failure& x);

    /**
     * return OS-depended error code
     **/
    int getErrorCode()  const  { return errorCode_; }

    /**
     * return error message
     **/
    const char* getErrorMessage() const { return what(); }

    /**
     * return error message
     **/
    const char* what() const  throw();

  };

  /**
   * base class for exceptions, which caused by resource limits.
   **/
  class NoResources: public Failure {};

  /**
   * NoMemory.
   * (in general - same, as std::bad_alloc)
   **/
  class NoMemory : public NoResources {};

  /**
   * resource temporary not aviable
   **/
  class TemporaryNoResources: public NoResources {};

  /**
   * current program have no permission to perform some actions
   **/
  class NoPermission : public NoResources {};

  /**
   * operation can cause a deadlock
   * (note, that actually deadlock exception on some platforms
   *  or with some compilation options can be not throwed)
   **/
  class PossibleDeadlock : public Failure {};

  /**
   * operation want get access to resource, which is busy now.
   **/
  class ResourceBusy : public Failure {};

  /**
   * InvalidResource
   **/
  class InvalidResource: public Failure {};

  /**
   * system error occured
   **/
  class SystemError : public Failure {};

  /**
   * indicate  internal error or incorrect configuration of package
   **/
  class InternalError : public Failure 
  {
  public:
     ///
     InternalError()
      :Failure(-1,"Threading:InternalError")
     {}

     ///
     InternalError(int code, const char* message)
      :Failure(code,message)
     {}
  };

  /**
   * functionality is not implemented on this platform
   **/
  class NotImplemented : public Failure 
  {
   public:

     NotImplemented()
      :Failure(-1,"Threading:not implemented") {}
     
  };

};

}

#endif
