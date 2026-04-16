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

    enum LogType {
        WARNING,
        DEBUG,
        INFO,
        ERROR,
        CRITICAL
    };

    struct LogMessage {
        std::string msg;
        altenter::quokahttp::LogType type;

        LogMessage(std::string&& msg, altenter::quokahttp::LogType type = altenter::quokahttp::LogType::INFO): msg(msg), type(type) {}
    };

    class QConsoleLogger: public altenter::quokahttp::detail::QILogger {
        public:
            QConsoleLogger();
            ~QConsoleLogger();

            void log(std::string&& msg) override;
            void log(std::string&& msg, altenter::quokahttp::LogType type = altenter::quokahttp::LogType::INFO);
            
            private:
            std::queue<LogMessage> logMessages;
        
            std::mutex mtx;
            std::condition_variable cv;
            std::thread thrd;
            std::atomic<bool> run{true};

            void process();

            std::string getPrefix(altenter::quokahttp::LogType type);
            std::string getTimeInfo();
            char* getColorText(altenter::quokahttp::LogType type);
            char* getColorBg(altenter::quokahttp::LogType type);
    };
}