#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <functional>
#include <vector>
#include <atomic>
#include <memory>
#include <mutex>

#include "noncopyable.h"
#include "timestamp.h"
#include "currentthread.h"

class Channel;
class Poller;

class EventLoop: public Noncopyable
{
public:
    using Functor = std::function<void()>;
    
    EventLoop();
    ~EventLoop();
    void Loop();
    void Quit();
    Timestamp PollerReturnTime() const {return pollerReturnTime_;}
    void RunInLoop(Functor cb);
    void QueueInLoop(Functor cb);
    void Wakeup();

    void UpdateChannel(Channel *channel);
    void RemoveChannel(Channel *channel);
    bool HasChannel(Channel *channel);
    bool IsInLoopThread() const { return threadId_ ==  CurrentThread::Tid(); }


private:
    void HandleRead();
    void DoPendingFunctors();

    using ChannelList = std::vector<Channel>;

    std::atomic_bool looping_;
    std::atomic_bool quit_;

    const pid_t threadId_;

    Timestamp pollerReturnTime_;
    std::unique_ptr<Poller> poller_;

    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;

    ChannelList activeChannels_;
    std::atomic_bool callingPendingFunctors_; 
    std::vector<Functor> pendingFunctors_; 
    std::mutex mutex_; 
    

};
#endif