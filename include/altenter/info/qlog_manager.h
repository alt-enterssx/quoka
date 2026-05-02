#pragma once 

#include <vector>
#include <sstream>
#include <memory>
#include "altenter/detail/q_i_logger.h"

namespace altenter::quokahttp::detail 
{
    class qlog_manager 
    {
        protected:
           qlog_manager();
           ~qlog_manager();

        public:
            qlog_manager(const qlog_manager&) = delete;
            void operator=(const qlog_manager&) = delete;

            static qlog_manager& get_instance();

            void add_logger(std::shared_ptr<q_i_logger> logger);
            void log(const std::string& msg, log_type type);
            
            template<typename... Args>
            void logFormat(std::string_view fmt, log_type type, Args&&... args) {
                std::ostringstream ss;

                std::string_view rest = fmt;
                size_t arg_index = 0;

                    std::string arg_strings[] = {
                    ([&]{
                        std::ostringstream tmp;
                        tmp << std::forward<Args>(args);
                        return tmp.str();
                    }())...
                };

                while (true) {
                    size_t pos = rest.find("{}");

                    if (pos == std::string_view::npos) {
                        ss << rest;
                        break;
                    }

                    ss << rest.substr(0, pos);

                    if (arg_index < sizeof...(Args)) {
                        ss << arg_strings[arg_index++];
                    } else {
                        ss << "{}";
                    }

                    rest = rest.substr(pos + 2);
                }

                this->log(ss.str(), type);
            }
            
            void shutdown();

        private:
            std::vector<std::shared_ptr<q_i_logger>> loggers;
    };
}