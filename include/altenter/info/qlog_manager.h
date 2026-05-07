#pragma once 

#include <vector>
#include <sstream>
#include <memory>
#include <cstdint>
#include "altenter/info/qconsole_logger.h"
#include "altenter/qexception.h"

namespace altenter::quoka::detail 
{
    class qlog_manager 
    {
        protected:
           qlog_manager();
           ~qlog_manager();

        public:
            qlog_manager(const qlog_manager&) = delete;
            void operator=(const qlog_manager&) = delete;

            static qlog_manager& manager();

            void log(const std::string& msg, log_type type);
            void log(const std::string& msg, exception_type type);
            void set_console(bool status);
            
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
                    } 
                    else {
                        ss << "{}";
                    }

                    rest = rest.substr(pos + 2);
                }

                this->log(ss.str(), type);
            }
            
            void shutdown();

        private:
            uint8_t flag;
    };
}