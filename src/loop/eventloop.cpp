#include "eventloop.h"
#include "logger.h"
#include "poller.h"
#include "channel.h"


#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <memory>

__thread EventLoop* t_loopInThisThread_ = nullptr;

const int KpollTimeMs = 10*1000;

int CreateEventFd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        std::string msg;
        msg = std::string("eventfd error")+std::to_string(evtfd);
        UVLOG_ERROR(msg);
    }
    return evtfd;
}
EventLoop::EventLoop()
    : looping_(false)
    , quit_(false)
    , callingPendingFunctors_(false)
    , threadId_(CurrentThread::Tid())
    //, poller_(Poller::newDefaultPoller(this))
    , wakeupFd_(CreateEventFd())
    , wakeupChannel_(new Channel(this, wakeupFd_))
{
    UVLOG_INFO("EventLoop created %p in thread %d \n", this, threadId_);
    if (t_loopInThisThread_)
    {
        UVLOG_ERROR("Another EventLoop %p exists in this thread %d \n", t_loopInThisThread_, threadId_);
    }
    else
    {
        t_loopInThisThread_ = this;
    }
    wakeupChannel_->SetReadCallback(std::bind(&EventLoop::HandleRead, this));
    wakeupChannel_->EnableReading();
}
EventLoop::~EventLoop()
{
    wakeupChannel_->DisableAll();
    wakeupChannel_->Remove();
    ::close(wakeupFd_);
    t_loopInThisThread_ = nullptr;
}
void EventLoop::Loop()
{
    looping_ = true;
    quit_ = false;
    UVLOG_INFO("EventLoop %p start looping \n", this);
    while(!quit_)
    {
        activeChannels_.clear();
        
    }
}


