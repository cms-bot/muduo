#ifndef __POLLER_H__
#define __POLLER_H__

#include "noncopyable.h"
#include "timestamp.h"

#include <vector>
#include <unordered_map>

class Channel;
class EventLoop;

class Poller : public Noncopyable
{
public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop* loop);
    virtual ~Poller()= default;

    virtual Timestamp Poll(int timeoutMs,ChannelList* activeChannels) = 0;
    virtual void UpdateChannel(Channel* channel) = 0;
    virtual void RemoveChannel(Channel* channel) = 0;

    bool HasChannel(Channel* channel) const;
    static Poller* NewDefaultPoller(EventLoop* loop);
protected:
    using ChannelMap = std::unordered_map<int,Channel*>;
    ChannelMap channels_;
private:
    EventLoop* owerLoop_;
};
#endif