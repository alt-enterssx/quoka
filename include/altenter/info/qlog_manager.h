#pragma once 

#include <memory>
#include <vector>
#include "altenter/detail/q_i_logger.h"

namespace altenter::quokahttp::detail {
    class qlog_manager 
    {
        public:
            qlog_manager();
            ~qlog_manager();

            void add_logger(std::shared_ptr<q_i_logger> logger);
            void log(const std::string& msg, log_type type);
            void shutdown();

        private:
            std::vector<std::shared_ptr<q_i_logger>> loggers;
    };
}