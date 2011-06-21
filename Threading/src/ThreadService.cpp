
#ifndef __GradSoft_ThreadService_h
#include <GradSoft/ThreadService.h>
#endif

/*
 * part of GradSoft C++ ToolBox
 * (C) GradSoft 2000, 2001
 * http://www.gradsoft.com.ua
 * $Id: ThreadService.cpp,v 1.2 2001-10-31 03:13:50 rssh Exp $
 */


namespace GradSoft {

ThreadService::ThreadService()
    :active_(false) {}

ThreadService::~ThreadService()
    {}

void  ThreadService::activate()
{
 active_ = true;
}

void  ThreadService::deactivate(bool shutdown)
{
 active_ = false;
}

void  ThreadService::mark_deactivate()
{
 active_ = false;
}

}

