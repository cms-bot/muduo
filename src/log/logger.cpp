#include "logger.h"

void Logger::Init(const std::string &logFile,
                  size_t maxFileSize,
                  size_t maxFiles,
                  const std::string &pattern,
                  spdlog::level::level_enum logLevel,
                  bool logToConsole,
                  bool logToFile,
                  bool multiThreaded)
{
    std::lock_guard<std::mutex> lock(mutex_);
    use_async_logger_ = multiThreaded;
    try
    {
        std::shared_ptr<spdlog::sinks::sink> console_sink;
        if (logToConsole)
        {
            console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink.get()->set_pattern(pattern);

        }

        std::shared_ptr<spdlog::sinks::sink> file_sink;
        if (logToFile)
        {
            file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFile, maxFileSize, maxFiles);
            file_sink.get()->set_pattern(pattern);
        }
        std::vector<spdlog::sink_ptr> sinks;
        if (console_sink)
            sinks.push_back(console_sink);
        if (file_sink)
            sinks.push_back(file_sink);

        if (use_async_logger_)
        {
            spdlog::init_thread_pool(8192, 1);
            async_logger_ = std::make_shared<spdlog::async_logger>("async", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
            spdlog::register_logger(async_logger_);
            async_logger_->set_level(logLevel);
            spdlog::set_default_logger(async_logger_);
            spdlog::flush_on(spdlog::level::info);
        }
        else
        {
            logger_ = std::make_shared<spdlog::logger>("sync", sinks.begin(), sinks.end());
            logger_->set_level(logLevel);
            spdlog::set_default_logger(logger_);
            spdlog::flush_on(spdlog::level::info);
        }
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}
void Logger::SetLogLevel(spdlog::level::level_enum level)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (use_async_logger_)
    {
        if (async_logger_)
        {
            async_logger_->set_level(level);
        }
    }
    else
    {
        if (logger_)
        {
            logger_->set_level(level);
        }
    }
}

std::shared_ptr<spdlog::logger> Logger::GetLogger()
{
    return use_async_logger_ ? async_logger_ : logger_;
}

Logger &Logger::GetInstance()
{
    static Logger instance;
    return instance;
}
