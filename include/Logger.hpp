#pragma once
#include <mutex>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdarg>

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static void logMessage(LogLevel level, const std::string& message) ;
};

#define LOG_INFO(...) Logger::logMessage(INFO, __VA_ARGS__)
#define LOG_WARNING(...) Logger::logMessage(WARNING, __VA_ARGS__)
#define LOG_ERROR(...) Logger::logMessage(ERROR, __VA_ARGS__)
