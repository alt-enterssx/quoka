#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <memory>
#include <errno.h>
#include <cstring>
#include <sstream>

#include "altenter/info/QILogger.h"
#include "altenter/QException.h"

namespace altenter::quokahttp::detail {   
    class QTcpSocket 
    {
        public:
            QTcpSocket();
            ~QTcpSocket();

            void init_socket();
            void bind_address(uint16_t port);

            void addLogger(std::shared_ptr<QILogger> logger);

            int getSocket();
            sockaddr_in getAddress();
            
        private:
            void logAll(const std::string& msg, detail::LogType type);
        
            int qs_socket;
            struct sockaddr_in qs_address;

            std::vector<std::shared_ptr<QILogger>> loggers;
   };
}