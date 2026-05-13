#pragma once 

#include <vector>
#include <sstream>
#include <memory>
#include <cstdint>
#include "altenter/qconsole_logger.h"
#include "altenter/qexception.h"

namespace altenter::quoka::detail 
{
    class qlog_manager 
    {
        protected:
           qlog_manager();
           ~qlog_manager();

        public:
            qlog_manager(const qlog_manager&) noexcept = delete;
            void operator=(const qlog_manager&) noexcept = delete;

            static qlog_manager& manager() noexcept;

            void log(const std::string& msg, log_type type) noexcept;
            void log(const std::string& msg, exception_type type) noexcept;
            void set_console(bool status) noexcept;
            
            template<typename... Args>
            void logFormat(std::string_view fmt, log_type type, Args&&... args) noexcept {
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
            
            void shutdown() noexcept;

        private:
            uint8_t flag;
    };
}