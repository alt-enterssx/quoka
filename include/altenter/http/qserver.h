#pragma once

#include <vector>
#include <netinet/in.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include <unistd.h>
#include "altenter/detail/q_i_logger.h"
#include "altenter/info/qconsole_logger.h"
#include "altenter/detail/qtcp_socket.h"
#include "altenter/qexception.h"
#include "altenter/info/qlog_manager.h"
#include "altenter/http/qlistener.h"

#define DEFAULT_PORT_START 2060
#define LOCAL_ADDRESS "127.0.0.1"
 
namespace altenter::quokahttp 
{    
    class qserver 
    {
        public:
            qserver();
            ~qserver();

            void run(int port = DEFAULT_PORT_START);

        private:

            bool is_running;

            int qs_port;

            // | Details
            detail::qtcp_socket socket;
            detail::qlog_manager log_manager;
            detail::qlistener listener;
    };
}