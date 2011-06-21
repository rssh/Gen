
#ifndef __GradSoft_ThreadServices_ThreadPerClient_h
#include <GradSoft/ThreadServices/ThreadPerClient.h>
#endif

/*
 * Part of GradSoft C++ ToolBox
 * (C) GradSoft, 2001
 * (C) Ruslan Shevchenko, 2001
 * $Id: ThreadPerClient.cpp,v 1.3 2001-10-31 03:13:52 rssh Exp $
 */

namespace GradSoft {

void ThreadPerClient::ClientThread::run()
{
 client_->run();
 delete this;
}

void ThreadPerClient::process(Runnable* runnable)
{
 if (!is_active()) 
    throw ThreadService::NotActive();
 ClientThread* clientThread_p = new ClientThread(runnable);
 clientThread_p->start();
 Thread::yield();
}

}

