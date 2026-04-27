#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <memory>
#include <errno.h>
#include <cstring>
#include <sstream>
#include "altenter/qexception.h"
#include "altenter/info/qlog_manager.h"

namespace altenter::quokahttp::detail 
{   
    class qtcp_socket 
    {
        public:
            qtcp_socket(qlog_manager& log_manager);
            ~qtcp_socket();

            void init_socket();
            void bind_address(uint16_t port);

            int get_socket();
            sockaddr_in get_address();
            
        private:
            int qs_socket;
            struct sockaddr_in qs_address;

            qlog_manager& log_manager;
   };
}