#include "Logger.hpp"
#include <iostream>

std::mutex Logger::mutex_;
std::condition_variable Logger::cv_;
std::queue<std::pair<LogLevel, std::string>> Logger::log_queue_;
std::thread Logger::worker_;
std::atomic<bool> Logger::running_{false};
std::string Logger::filename_;

void Logger::Init(const std::string& filename) {
    filename_ = filename;
    running_ = true;
    worker_ = std::thread(ProcessLogs);
}

void Logger::Shutdown() {
    running_ = false;
    cv_.notify_all();
    if (worker_.joinable()) {
        worker_.join();
    }
}

void Logger::Log(LogLevel level, const std::string& message) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        log_queue_.emplace(level, message);
    }
    cv_.notify_one();
}

std::string Logger::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARN";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

std::string Logger::FormatLog(LogLevel level, const std::string& message) {
    return GetTimestamp() + "\n [" + LevelToString(level) + "] " + message;
}

void Logger::ProcessLogs() {
    std::ofstream logFile(filename_, std::ios::app);

    while (running_ || !log_queue_.empty()) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [&] { return !log_queue_.empty() || !running_; });

        std::queue<std::pair<LogLevel, std::string>> temp_queue;
        temp_queue.swap(log_queue_);
        lock.unlock();

        while (!temp_queue.empty()) {
            auto& [level, message] = temp_queue.front();
            std::string formatted = FormatLog(level, message);
            logFile << formatted << '\n';
            temp_queue.pop();
        }
        logFile.flush();
    }

    logFile.close();
}