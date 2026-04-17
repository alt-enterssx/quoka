#pragma once 

#include <stdexcept>
#include <string>

namespace altenter::quokahttp {
    
    enum ExceptionType {
        ERROR,
        CRITICAL
    };

    class QException: public std::runtime_error
    {
        public: 
            QException(const std::string&& msg, altenter::quokahttp::ExceptionType type = altenter::quokahttp::ExceptionType::ERROR);
            altenter::quokahttp::ExceptionType getType();

        private:
            std::string msg;
            altenter::quokahttp::ExceptionType exc_type;
    };
}