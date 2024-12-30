#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "noncopyable.h"
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <memory>
#include <mutex>
#include <string>
#include <iostream>
#include <chrono>

class Logger: public Noncopyable
{
public:
    static Logger& GetInstance();

    void Init(const std::string& logFile = "../logs/muduo.log",
              size_t maxFileSize = 1048576, 
              size_t maxFiles = 3, 
              const std::string& pattern = "%Y-%m-%d %H:%M:%S[%t] [%^%l%$] [%s %!:%#] v%", 
              spdlog::level::level_enum logLevel = spdlog::level::info,
              bool logToConsole = true,
              bool logToFile = true,
              bool multiThreaded = true);

    void SetLogLevel(spdlog::level::level_enum level);
    std::shared_ptr<spdlog::logger> GetLogger();
private:
    std::shared_ptr<spdlog::logger> logger_;
    std::shared_ptr<spdlog::async_logger> async_logger_;
    bool use_async_logger_;
    std::mutex mutex_;
};

#define LOG_TRACE(...) Logger::GetInstance().GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::GetInstance().GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) Logger::GetInstance().GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Logger::GetInstance().GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::GetInstance().GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::GetInstance().GetLogger()->critical(__VA_ARGS__)
#define LOG_FATAL(...) Logger::GetInstance().GetLogger()->fatal(__VA_ARGS__)

#define UVLOG_TRACE(...)  SPDLOG_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::trace, __VA_ARGS__)
#define UVLOG_DEBUG(...)  SPDLOG_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::debug, __VA_ARGS__)
#define UVLOG_INFO(...)   SPDLOG_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::info, __VA_ARGS__)
#define UVLOG_WARN(...)   SPDLOG_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::warn, __VA_ARGS__)
#define UVLOG_ERROR(...)  SPDLOG_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::err, __VA_ARGS__)

#endif

