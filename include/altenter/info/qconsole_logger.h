#pragma once

#include "altenter/detail/q_i_logger.h"

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

namespace altenter::quoka 
{
    struct log_message 
    {
        std::string msg;
        altenter::quoka::detail::log_type type;

        log_message(const std::string& msg, detail::log_type type = detail::log_type::INFO): msg(msg), type(type) {}
    };

    class qconsole_logger: public detail::q_i_logger 
    {
        protected:
            qconsole_logger();
        
        public:
            ~qconsole_logger();

            static qconsole_logger& logger();

            void shutdown() override;

            void log(const std::string& msg) override;
            void log(const std::string& msg, detail::log_type type = detail::log_type::INFO) override;
            
        private:
            std::queue<log_message> log_messages;
        
            std::mutex mtx;
            std::condition_variable cv;
            std::thread thrd;
            std::atomic<bool> run{true};

            void process();

            std::string get_prefix(detail::log_type type);
            std::string get_time_info();
            char* get_color_text(detail::log_type type);
            char* get_color_bg(detail::log_type type);

    };
}