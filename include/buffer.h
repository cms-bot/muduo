#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <string>
#include <vector>

class Buffer
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize)
        , readerIndex_(kCheapPrepend)
        , writerIndex_(kCheapPrepend)
    {}

    size_t ReadableBytes() const 
    {
        return writerIndex_ - readerIndex_;
    }

    size_t WritableBytes() const
    {
        return buffer_.size() - writerIndex_;
    }

    size_t PrependableBytes() const
    {
        return readerIndex_;
    }

    const char* Peek() const
    {
        return Begin() + readerIndex_;
    }

    // onMessage string <- Buffer
    void Retrieve(size_t len)
    {
        if (len < ReadableBytes())
        {
            readerIndex_ += len;
        }
        else 
        {
            RetrieveAll();
        }
    }

    void RetrieveAll()
    {
        readerIndex_ = writerIndex_ = kCheapPrepend;
    }

    std::string RetrieveAllAsString()
    {
        return RetrieveAsString(ReadableBytes()); 
    }

    std::string RetrieveAsString(size_t len)
    {
        std::string result(Peek(), len);
        Retrieve(len); 
        return result;
    }

    void EnsureWriteableBytes(size_t len)
    {
        if (WritableBytes() < len)
        {
            MakeSpace(len); // 扩容函数
        }
    }

    void Append(const char *data, size_t len)
    {
        EnsureWriteableBytes(len);
        std::copy(data, data+len, BeginWrite());
        writerIndex_ += len;
    }

    char* BeginWrite()
    {
        return Begin() + writerIndex_;
    }

    const char* BeginWrite() const
    {
        return Begin() + writerIndex_;
    }

    ssize_t ReadFd(int fd, int* saveErrno);
    ssize_t WriteFd(int fd, int* saveErrno);
private:
    char* Begin()
    {
        return &*buffer_.begin(); 
    }
    const char* Begin() const
    {
        return &*buffer_.begin();
    }
    void MakeSpace(size_t len)
    {
        if (WritableBytes() + PrependableBytes() < len + kCheapPrepend)
        {
            buffer_.resize(writerIndex_ + len);
        }
        else
        {
            size_t readalbe = ReadableBytes();
            std::copy(Begin() + readerIndex_, 
                    Begin() + writerIndex_,
                    Begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readalbe;
        }
    }

    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
};

#endif