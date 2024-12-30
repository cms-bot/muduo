#include "epollpoller.h"
#include "logger.h"
#include "channel.h"

#include <errno.h>
#include <unistd.h>
#include <strings.h>

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

EpollPoller::EpollPoller(EventLoop *loop)
        : Poller(loop)
        , epollfd_(::epoll_create1(EPOLL_CLOEXEC))
        , events_(kInitEventListSize)
{
    if(epollfd_ < 0)
    {
        UVLOG_ERROR("epoll_create error:%d \n", errno);
    }
}
