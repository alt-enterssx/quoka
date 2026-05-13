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
#include "altenter/qlog_manager.h"

namespace altenter::quoka::detail 
{   
    class qtcp_socket 
    {
        public:
            qtcp_socket() noexcept;
            ~qtcp_socket() noexcept;

            void init_socket();
            void bind_address(uint16_t port);

            int get_socket() const noexcept;
            sockaddr_in get_address() const noexcept;
            
        private:
            int qs_socket;
            struct sockaddr_in qs_address;
   };
}