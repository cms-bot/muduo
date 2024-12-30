#include "buffer.h"

#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>

ssize_t Buffer::ReadFd(int fd,int* saveError)
{
    char extrabuf[65536] = {0};
    
    struct iovec vec[2];

    const size_t writeable = WritableBytes();
    vec[0].iov_base = Begin() + writerIndex_;
    vec[0].iov_len = writeable;

    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    const int iovcnt = (writeable < sizeof(extrabuf)) ? 2:1;
    const ssize_t ret = ::readv(fd,vec,iovcnt);
    if(ret < 0)
    {
        *saveError = errno;
    }
    else if(ret <= writeable)
    {
        writerIndex_ += ret;
    }else
    {
        writerIndex_ = buffer_.size();
        Append(extrabuf, ret - writeable);
    }
    return ret;
}

ssize_t Buffer::WriteFd(int fd,int* saveError)
{
    ssize_t ret = ::write(fd,Peek(),ReadableBytes());
    if(ret < 0)
    {
        *saveError = errno;
    }
    return ret;
}