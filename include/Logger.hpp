
#pragma once
#include <mutex>
#include <fstream>
#include <string>
#include <chrono>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <sstream>
#include <iomanip>

enum class LogLevel { INFO, WARNING, ERROR };

class Logger {
public:
    static void Init(const std::string& filename = "server.log");
    static void Shutdown();
    static void Log(LogLevel level, const std::string& message);

private:
    static void ProcessLogs();
    static std::string FormatLog(LogLevel level, const std::string& message);
    static std::string GetTimestamp();
    static std::string LevelToString(LogLevel level);

    static std::mutex mutex_;
    static std::condition_variable cv_;
    static std::queue<std::pair<LogLevel, std::string>> log_queue_;
    static std::thread worker_;
    static std::atomic<bool> running_;
    static std::string filename_;
};

#define LOG_INFO(msg) Logger::Log(LogLevel::INFO, msg)
#define LOG_WARNING(msg) Logger::Log(LogLevel::WARNING, msg)
#define LOG_ERROR(msg) Logger::Log(LogLevel::ERROR, msg)