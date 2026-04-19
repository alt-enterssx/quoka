#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <memory>
#include <errno.h>
#include <cstring>
#include <sstream>
#include "altenter/QException.h"
#include "altenter/info/QLogManager.h"

namespace altenter::quokahttp::detail {   
    class QTcpSocket 
    {
        public:
            QTcpSocket(QLogManager& logManager);
            ~QTcpSocket();

            void init_socket();
            void bind_address(uint16_t port);

            int getSocket();
            sockaddr_in getAddress();
            
        private:
            int qs_socket;
            struct sockaddr_in qs_address;

            QLogManager& logManager;
   };
}