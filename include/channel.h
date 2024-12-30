#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "noncopyable.h"
#include "timestamp.h"

#include <functional>
#include <memory>

class EventLoop;

class Channel :public Noncopyable
{
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop* loop,int fd);
    ~Channel();
    void HandleEvent(Timestamp recvTime);

    void SetReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void SetWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void SetCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void SetErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    void Tie(const std::shared_ptr<void>&);
    int Fd() const {return fd_; }
    int Events() const {return events_;}
    int Revents() const {return revents_;}
    int SetRevents(int revt){revents_ = revt;}

    void EnableReading() { events_ |= kReadEvent; Update(); }
    void DisableReading() { events_ &= ~kReadEvent; Update(); }
    void EnableWriting() { events_ |= kWriteEvent; Update(); }
    void DisableWriting() { events_ &= ~kWriteEvent; Update(); }
    void DisableAll() { events_ = kNoneEvent; Update(); }

    // 返回fd当前的事件状态
    bool IsNoneEvent() const { return events_ == kNoneEvent; }
    bool IsWriting() const { return events_ & kWriteEvent; }
    bool IsReading() const { return events_ & kReadEvent; }

    int Index() { return index_; }
    void SetIndex(int idx) { index_ = idx; }

    // one loop per thread
    EventLoop* OwnerLoop() { return loop_; }
    void Remove();
private:
    void Update();
    void HandleEventWithGuard(Timestamp recvTime);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int fd_;
    int events_;
    int revents_;
    int index_;

    std::weak_ptr<void> tie_;
    bool tied_;

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

};

#endif