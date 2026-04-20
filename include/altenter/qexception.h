#pragma once 

#include <stdexcept>
#include <string>

namespace altenter::quokahttp {
    
    enum exception_type {
        ERROR,
        CRITICAL
    };

    class qexception: public std::runtime_error
    {
        public: 
            qexception(const std::string&& msg, altenter::quokahttp::exception_type type = altenter::quokahttp::exception_type::ERROR);
            altenter::quokahttp::exception_type get_type();

        private:
            std::string msg;
            altenter::quokahttp::exception_type exc_type;
    };
}