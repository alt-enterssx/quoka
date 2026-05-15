#pragma once 

#include <vector>
#include <sstream>
#include <memory>
#include <cstdint>
#include <tuple>
#include <utility>
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
            qlog_manager& operator=(const qlog_manager&) noexcept = delete;

            static qlog_manager& manager() noexcept;

            void log(const std::string& msg, log_type type) noexcept;
            void log(const std::string& msg, exception_type type) noexcept;
            void set_console(bool status) noexcept;
        
            template<typename T>
            std::string toString(T&& value) {
                std::ostringstream oss;
                oss << value;
                return oss.str();
            }

            template<typename... Args>
            void logFormat(std::string fmt, log_type type, Args&&... args) noexcept {
                std::vector<std::string> arguments = {
                    toString(std::forward<Args>(args))...
                };

                std::string formatted;

                size_t step = 0;
                size_t arg_index = 0;

                while (true) {
                    size_t pos = fmt.find("{}", step);

                    if (pos == std::string::npos) {
                        formatted += fmt.substr(step);
                        break;
                    }

                    formatted += fmt.substr(step, pos - step);

                    if (arg_index < arguments.size()) {
                        formatted += arguments[arg_index++];
                    }
                    else {
                        formatted += "{}";
                    }

                    step = pos + 2;
                }

                this->log(formatted, type);
            }
            
            void shutdown() noexcept;

        private:
            uint8_t flag;
    };
}