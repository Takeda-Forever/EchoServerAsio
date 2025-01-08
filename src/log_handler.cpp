#include "../include/log_handler.hpp"

namespace Log {
    std::mutex Logger::mutex_;
    void Logger::log(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream log_file("log.txt", std::ios_base::app); 
        if (log_file.is_open()) {
            log_file << message << std::endl; 
        }
        log_file.close();
    }
    void Logger::log_error(const std::string& error_message)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream log_file("error_log.txt", std::ios_base::app);
        if (log_file.is_open()) {
            log_file << "ERROR: " << error_message << std::endl;
        }
        log_file.close();
    }
}