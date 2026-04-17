#pragma once

#include "altenter/info/QILogger.h"

#include <iostream>
#include <string>
#include <ctime>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <sstream>

#define WARNING_COLOR "\033[1;30;43m"
#define DEBUG_COLOR "\033[1;30;105m"
#define INFO_COLOR "\033[1;30;102m"
#define ERROR_COLOR "\033[1;97;101m"
#define CRITICAL_COLOR "\033[1;97;41m"

#define WARNING_COLOR_TEXT "\033[0;33m"
#define DEBUG_COLOR_TEXT "\033[0;95m"
#define INFO_COLOR_TEXT "\033[0;92m"
#define ERROR_COLOR_TEXT "\033[0;91m"
#define CRITICAL_COLOR_TEXT "\033[0;31m"

#define TIME_COLOR "\033[1;97;40m"

#define RESET "\033[0m"

namespace altenter::quokahttp {

    struct LogMessage {
        std::string msg;
        altenter::quokahttp::detail::LogType type;

        LogMessage(const std::string& msg, altenter::quokahttp::detail::LogType type = altenter::quokahttp::detail::LogType::INFO): msg(msg), type(type) {}
    };

    class QConsoleLogger: public detail::QILogger 
    {
        public:
            QConsoleLogger();
            ~QConsoleLogger();

            void shutdown() override;

            void log(const std::string& msg) override;
            void log(const std::string& msg, altenter::quokahttp::detail::LogType type = altenter::quokahttp::detail::LogType::INFO) override;
            
            template<typename... Args>
            void logFormat(const std::string& msg, altenter::quokahttp::detail::LogType type, Args&&... args) {
                std::vector<std::string> params = {
                    toString(std::forward<Args>(args))...
                };

                std::string result;
                result.reserve(msg.size() + 32);

                size_t argIndex = 0;

                for (size_t i = 0; i < msg.size(); ++i) {
                    if (msg[i] == '{' && i + 1 < msg.size() && msg[i + 1] == '}') {
                        if (argIndex < params.size()) {
                            result += params[argIndex++];
                        } else {
                            result += "{}";
                        }
                        ++i;
                    } else {
                        result += msg[i];
                    }
                }

                this->log(std::move(result), type);
            }

            char* loggerName() override;

            private:
            std::queue<LogMessage> logMessages;
        
            std::mutex mtx;
            std::condition_variable cv;
            std::thread thrd;
            std::atomic<bool> run{true};

            void process();

            std::string getPrefix(altenter::quokahttp::detail::LogType type);
            std::string getTimeInfo();
            char* getColorText(altenter::quokahttp::detail::LogType type);
            char* getColorBg(altenter::quokahttp::detail::LogType type);

            template<typename T>
            std::string toString(T& value) {
                std::ostringstream oss;
                oss << std::forward<T>(value);
                return oss.str();
            }
    };
}