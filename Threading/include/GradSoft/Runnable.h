#ifndef __GradSoft_Runnable_h
#define __GradSoft_Runnable_h 

/*
 * part of GradSoft Threading ToolBox
 * (C) GradSoft, 2000, 2001
 * $Id: Runnable.h,v 1.10 2004-03-13 09:53:24 rssh Exp $
 */

#ifndef __GradSoft_ThreadingPostConfig_h
#include <GradSoft/ThreadingPostConfig.h>
#endif

/**
 *
 **/
namespace GradSoft
{

/**
 * Abstract class for runnable
 * Runnable is item of execution
 **/
class __dll_threading Runnable
{
public:

  ///
  Runnable();

  ///
  virtual ~Runnable();

  ///
  virtual void run() = 0;

private:

  Runnable(const Runnable& );
  Runnable& operator=(const Runnable&);

};

}

#endif
