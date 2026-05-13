#pragma once

#include <string>

namespace altenter::quoka::detail 
{
    enum log_type 
    {
        WARNING,
        DEBUG,
        INFO,
        ERROR,
        CRITICAL
    };

    class q_i_logger 
    {
        public:
            virtual void log(const std::string& msg, log_type type = log_type::INFO) noexcept = 0;
            virtual void log(const std::string& msg) noexcept = 0;
            virtual void shutdown() noexcept = 0;
    };
}