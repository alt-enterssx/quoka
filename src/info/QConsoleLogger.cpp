#include "altenter/info/QConsoleLogger.h"

using namespace altenter::quokahttp;

QConsoleLogger::QConsoleLogger() {
    this->thrd = std::thread(&QConsoleLogger::process, this);
}

QConsoleLogger::~QConsoleLogger() {
    this->run = false;
    this->cv.notify_all();
    if (this->thrd.joinable()) {
        this->thrd.join();
    }
}

void QConsoleLogger::shutdown() {
    this->run = false;
    this->cv.notify_all();
    if (this->thrd.joinable()) {
        this->thrd.join();
    }
}

void QConsoleLogger::log(const std::string& msg) {

    {

        std::lock_guard<std::mutex> lock(this->mtx);

        this->logMessages.emplace(std::move(msg), detail::LogType::INFO);

    } 

    this->cv.notify_one();

}

void QConsoleLogger::log(const std::string& msg, detail::LogType type) {
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        this->logMessages.emplace(msg, type);
    }
    this->cv.notify_one();
}

char* QConsoleLogger::loggerName() { return "Console Logger"; }

void QConsoleLogger::process() {
    while (run || !logMessages.empty()) {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this] {
            return !logMessages.empty() || !run;
        });

        while (!logMessages.empty()) {
            auto msg = std::move(logMessages.front());
            logMessages.pop();

            lock.unlock(); 

            std::string prefix = getPrefix(msg.type);
            std::string time_str = getTimeInfo();

            std::cout << prefix << RESET << TIME_COLOR << ' ' << time_str << ' '
                      << RESET << getColorBg(msg.type) << ' ' << RESET
                      << ' ' << getColorText(msg.type) << msg.msg << RESET << std::endl;

            lock.lock();
        }
    }
}

std::string QConsoleLogger::getPrefix(detail::LogType type) {
    std::string combined;
    switch(type) {
        case detail::LogType::WARNING: {
            combined = WARNING_COLOR;
            combined = combined + " | WARNING  ";

            break;
        }
        case detail::LogType::DEBUG: {
            combined = DEBUG_COLOR;
            combined = combined + " | DEBUG    ";

            break;
        }
        case detail::LogType::INFO: {
            combined = INFO_COLOR;
            combined = combined + " | INFO     ";

            break;
        }
        case detail::LogType::ERROR: {
            combined = ERROR_COLOR;
            combined = combined + " | ERROR    ";

            break;
        }
        case detail::LogType::CRITICAL: {
            combined = CRITICAL_COLOR;
            combined = combined + " | CRITICAL ";

            break;
        }
        default: { return ""; }
    }

    return combined;
}

std::string QConsoleLogger::getTimeInfo() {
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

char* QConsoleLogger::getColorText(detail::LogType type) {
    switch(type) {
        case detail::LogType::WARNING: {
            return WARNING_COLOR_TEXT;
        }
        case detail::LogType::DEBUG: {
            return DEBUG_COLOR_TEXT;
        }
        case detail::LogType::INFO: {
            return INFO_COLOR_TEXT;
        }
        case detail::LogType::ERROR: {
            return ERROR_COLOR_TEXT;
        }
        case detail::LogType::CRITICAL: {
            return CRITICAL_COLOR_TEXT;
        }
        default: { return ""; }
    }
}

char* QConsoleLogger::getColorBg(detail::LogType type) {
    switch(type) {
        case detail::LogType::WARNING: {
            return WARNING_COLOR;
        }
        case detail::LogType::DEBUG: {
            return DEBUG_COLOR;
        }
        case detail::LogType::INFO: {
            return INFO_COLOR;
        }
        case detail::LogType::ERROR: {
            return ERROR_COLOR;
        }
        case detail::LogType::CRITICAL: {
            return CRITICAL_COLOR;
        }
        default: { return ""; }
    }
}