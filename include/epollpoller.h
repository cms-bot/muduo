#ifndef __EPOLLPOLLER_H__
#define __EPOLLPOLLER_H__

#include "poller.h"
#include "timestamp.h"

#include <vector>
#include <sys/epoll.h>

class Channel;

class EpollPoller : public Poller
{
public:
    EpollPoller(EventLoop *loop);
    ~EpollPoller() override;
    Timestamp Poll(int timeoutMs, ChannelList *activeChannels) override;
    void UpdateChannel(Channel *channel) override;
    void RemoveChannel(Channel *channel) override;
private:
    static const int kInitEventListSize = 16;
    void FillActiveChannels(int numEvents, ChannelList *activeChannels) const;
    void Update(int operation, Channel *channel);
    
    using EventList = std::vector<epoll_event>;
    int epollfd_;
    EventList events_;
};
#endif