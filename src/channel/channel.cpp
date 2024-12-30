#include "channel.h"
#include "eventloop.h"
#include "logger.h"
#include <sys/epoll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop*loop,int fd)
    : loop_(loop),fd_(fd),events_(0),revents_(0),index_(-1),tied_(false)
{}
Channel::~Channel()
{}

void Channel::Tie(const std::shared_ptr<void>& obj)
{
    tie_ = obj;
    tied_ = true;
}
void Channel::Update()
{
    //TO DO:
}
void Channel::Remove()
{
    //TO DO
}
void Channel::HandleEvent(Timestamp recvTime)
{
    if(tied_)
    {
        std::shared_ptr<void> guard = tie_.lock();
        if(guard)
        {
            HandleEventWithGuard(recvTime);
        }
    }else
    {
        HandleEventWithGuard(recvTime);
    }
}
void Channel::HandleEventWithGuard(Timestamp recvTime)
{
    char msg[1024] = {0};
    sprintf(msg, "channel handleEvent revents:%d",revents_);
    UVLOG_INFO(msg);
    if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if(closeCallback_)
        {
            closeCallback_();
        }
    }
    if(revents_ & EPOLLERR)
    {
        if(errorCallback_)
        {
            errorCallback_();
        }
    }
    if(revents_ & (EPOLLIN | EPOLLPRI))
    {
        if(readCallback_)
        {
            readCallback_(recvTime);
        }
    }
    if(revents_ & EPOLLOUT)
    {
        if(writeCallback_)
        {
            writeCallback_();
        }
    }

}