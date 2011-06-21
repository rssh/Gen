
/*
 * part of GradSoft C++ ToolBox.
 * (C) GradSoft 2000, 2001
 * http://www.gradsoft.com.ua
 * $Id: SingleThreadBlocking.cpp,v 1.9 2002-08-13 12:38:09 yulp Exp $
 */

#ifndef __GradSoft_ThreadServices_SingleThreadBlocking_h
#include <GradSoft/ThreadServices/SingleThreadBlocking.h>
#endif


/**
 * SingleThreadBlocking ThreadService.
 * (i. e. this is 'non-thread' ThreadService: each message is
 * processed in the same thread as caller, without
 * any dispatching.
 **/


namespace GradSoft {


void SingleThreadBlocking::process(Runnable* runnable)
{
 if (!is_active()) {
      throw ThreadService::NotActive();
 }
 if (current_ == NULL) {
    current_ = runnable;
    current_->run();
    current_ = NULL;
 }else{
    throw ThreadingExceptions::TemporaryNoResources();
 }
}


}
