#pragma once

#include <string>

namespace altenter::quokahttp::detail {

    enum LogType {
        WARNING,
        DEBUG,
        INFO,
        ERROR,
        CRITICAL
    };

    class QILogger 
    {
        public:
            virtual char* loggerName() = 0;
            virtual void log(const std::string& msg, altenter::quokahttp::detail::LogType type = altenter::quokahttp::detail::LogType::INFO) = 0;
            virtual void log(const std::string& msg) = 0;
            virtual void shutdown() = 0;
    };
}