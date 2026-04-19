#pragma once 

#include <memory>
#include <vector>
#include "altenter/info/QILogger.h"

namespace altenter::quokahttp::detail {
    class QLogManager 
    {
        public:
            QLogManager();
            ~QLogManager();

            void addLogger(std::shared_ptr<detail::QILogger> logger);
            void log(const std::string& msg, detail::LogType type);
            void shutdown();

        private:
            std::vector<std::shared_ptr<QILogger>> loggers;
    };
}