#pragma once 

#include <stdexcept>
#include <string>

namespace altenter::quoka 
{    
    enum exception_type 
    {
        ERROR,
        CRITICAL
    };

    class qexception: public std::runtime_error
    {
        public: 
            qexception(const std::string&& msg, exception_type type = exception_type::ERROR);
            exception_type type();

        private:
            std::string msg;
            exception_type exc_type;
    };
}