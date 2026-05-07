#include "altenter/info/qconsole_logger.h"

using namespace altenter::quoka;

qconsole_logger::qconsole_logger() {
    this->thrd = std::thread(&qconsole_logger::process, this);
}

qconsole_logger::~qconsole_logger() {
    this->run = false;
    this->cv.notify_all();
    if (this->thrd.joinable()) {
        this->thrd.join();
    }
}

qconsole_logger& qconsole_logger::logger() {
    static qconsole_logger console_logger;
    return console_logger;
}

void qconsole_logger::shutdown() {
    this->run = false;
    this->cv.notify_all();
    if (this->thrd.joinable()) {
        this->thrd.join();
    }
}

void qconsole_logger::log(const std::string& msg) {
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        this->log_messages.emplace(std::move(msg), detail::log_type::INFO);
    } 
    this->cv.notify_one();
}

void qconsole_logger::log(const std::string& msg, detail::log_type type) {
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        this->log_messages.emplace(msg, type);
    }
    this->cv.notify_one();
}

void qconsole_logger::process() {
    while (run || !log_messages.empty()) {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this] {
            return !log_messages.empty() || !run;
        });

        while (!log_messages.empty()) {
            auto msg = std::move(log_messages.front());
            log_messages.pop();

            lock.unlock(); 

            std::string prefix = get_prefix(msg.type);
            std::string time_str = get_time_info();

            std::cout << prefix << RESET << TIME_COLOR << ' ' << time_str << ' '
                      << RESET << get_color_bg(msg.type) << ' ' << RESET
                      << ' ' << get_color_text(msg.type) << msg.msg << RESET << std::endl;

            lock.lock();
        }
    }
}

std::string qconsole_logger::get_prefix(detail::log_type type) {
    std::string combined;
    switch(type) {
        case detail::log_type::WARNING: {
            combined = WARNING_COLOR;
            combined = combined + " | WARNING  ";

            break;
        }
        case detail::log_type::DEBUG: {
            combined = DEBUG_COLOR;
            combined = combined + " | DEBUG    ";

            break;
        }
        case detail::log_type::INFO: {
            combined = INFO_COLOR;
            combined = combined + " | INFO     ";

            break;
        }
        case detail::log_type::ERROR: {
            combined = ERROR_COLOR;
            combined = combined + " | ERROR    ";

            break;
        }
        case detail::log_type::CRITICAL: {
            combined = CRITICAL_COLOR;
            combined = combined + " | CRITICAL ";

            break;
        }
        default: { return ""; }
    }

    return combined;
}

std::string qconsole_logger::get_time_info() {
    std::time_t t = time(nullptr);
    std::tm* loctime = localtime(&t);

    std::stringstream ss;
    ss << loctime->tm_year + 1900 << '.'
        << std::setw(2) << std::setfill('0') << loctime->tm_mday << '.'
        << std::setw(2) << std::setfill('0') << loctime->tm_mon + 1 << ' '
        << std::setw(2) << std::setfill('0') << loctime->tm_hour << ':'
        << std::setw(2) << std::setfill('0') << loctime->tm_min << ':'
        << std::setw(2) << std::setfill('0') << loctime->tm_sec;

    std::string time_str = ss.str();
    return time_str;
}

char* qconsole_logger::get_color_text(detail::log_type type) {
    switch(type) {
        case detail::log_type::WARNING: {
            return WARNING_COLOR_TEXT;
        }
        case detail::log_type::DEBUG: {
            return DEBUG_COLOR_TEXT;
        }
        case detail::log_type::INFO: {
            return INFO_COLOR_TEXT;
        }
        case detail::log_type::ERROR: {
            return ERROR_COLOR_TEXT;
        }
        case detail::log_type::CRITICAL: {
            return CRITICAL_COLOR_TEXT;
        }
        default: { return ""; }
    }
}

char* qconsole_logger::get_color_bg(detail::log_type type) {
    switch(type) {
        case detail::log_type::WARNING: {
            return WARNING_COLOR;
        }
        case detail::log_type::DEBUG: {
            return DEBUG_COLOR;
        }
        case detail::log_type::INFO: {
            return INFO_COLOR;
        }
        case detail::log_type::ERROR: {
            return ERROR_COLOR;
        }
        case detail::log_type::CRITICAL: {
            return CRITICAL_COLOR;
        }
        default: { return ""; }
    }
}