#pragma once
#include "pch.h"

namespace Log {
    class Logger {
        public: 
            static void log(const std::string& message);
            static void log_error(const std::string& error_message);

    private:
        static std::mutex mutex_;
    };
}