#pragma once

#include <string>

namespace altenter::quokahttp::detail {
    class QILogger 
    {
        public:
        virtual void log(std::string&& msg) = 0;
    };
}