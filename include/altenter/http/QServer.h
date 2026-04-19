#pragma once

#include <vector>
#include <netinet/in.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include <unistd.h>
#include "altenter/info/QILogger.h"
#include "altenter/info/QConsoleLogger.h"
#include "altenter/net/QTcpSocket.h"
#include "altenter/QException.h"
#include "altenter/info/QLogManager.h"

#define DEFAULT_PORT_START 2060
#define LOCAL_ADDRESS "127.0.0.1"

namespace altenter::quokahttp {    
    class QServer 
    {
        public:
            QServer();
            ~QServer();

            void run(int port = DEFAULT_PORT_START);

        private:

            bool isRunning;

            int qs_port;
            detail::QTcpSocket socket;

            detail::QLogManager logManager;
    };
}