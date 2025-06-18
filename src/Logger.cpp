#include <Logger.hpp>

void Logger::logMessage(LogLevel level, const std::string& message) {
    static std::mutex logMutex;
    std::lock_guard<std::mutex> guard(logMutex);
    std::ofstream logFile("server.log", std::ios::app);

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    std::string levelStr;
    switch (level) {
        case INFO: levelStr = "INFO"; break;
        case WARNING: levelStr = "WARNING"; break;
        case ERROR: levelStr = "ERROR"; break;
    }

    logFile << std::ctime(&now_c) << " [" << levelStr << "] " << message << std::endl;
    logFile.close();
}