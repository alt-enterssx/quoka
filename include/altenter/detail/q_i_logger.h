#pragma once

#include <string>

namespace altenter::quokahttp::detail {

    enum log_type {
        WARNING,
        DEBUG,
        INFO,
        ERROR,
        CRITICAL
    };

    class q_i_logger 
    {
        public:
            virtual void log(const std::string& msg, altenter::quokahttp::detail::log_type type = altenter::quokahttp::detail::log_type::INFO) = 0;
            virtual void log(const std::string& msg) = 0;
            virtual void shutdown() = 0;
    };
}