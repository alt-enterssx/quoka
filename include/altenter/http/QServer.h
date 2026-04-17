#pragma once

#include <vector>
#include <netinet/in.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include "altenter/info/QILogger.h"
#include "altenter/info/QConsoleLogger.h"
#include "altenter/net/QTcpSocket.h"
#include "altenter/QException.h"

#define DEFAULT_PORT_START 2060

namespace altenter::quokahttp {    
    class QServer 
    {
        public:
            QServer();
            ~QServer();

            void run(int port = DEFAULT_PORT_START);

            void addLogger(std::shared_ptr<detail::QILogger> logger);

        private:
            void shutDownAll();
            void logAll(const std::string& msg, detail::LogType type);

            bool isRunning;

            int qs_port;
            detail::QTcpSocket socket;

            std::vector<std::shared_ptr<detail::QILogger>> loggers;
    };
}