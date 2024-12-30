#include "poller.h"
#include "channel.h"

Poller::Poller(EventLoop* loop)
            : owerLoop_(loop)
{
}
bool Poller::HasChannel(Channel* channel) const
{
    auto it = channels_.find(channel->Fd());
    return it != channels_.end() && it->second == channel;
}