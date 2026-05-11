#pragma once

#include <vector>
#include <netinet/in.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include <unistd.h>
#include <functional>
#include "altenter/detail/q_i_logger.h"
#include "altenter/info/qconsole_logger.h"
#include "altenter/detail/qtcp_socket.h"
#include "altenter/qexception.h"
#include "altenter/info/qlog_manager.h"
#include "altenter/http/qlistener.h"
#include "altenter/http/qrouter.h"
#include "altenter/utils/qconfig.h"

#define DEFAULT_PORT_START 5050
#define LOCAL_ADDRESS "127.0.0.1"
 
namespace altenter::quoka 
{    
    class qserver 
    {
        protected:
            qserver(int port);
        
        public:
            class builder 
            {
                public:
                    builder();

                    builder& set_port(int port);
                    std::unique_ptr<qserver> build();
                private:
                    int port_;
            }; 

            ~qserver();

            void run();
        private:

            // | Datas
            bool is_running;
            int qs_port;

            // | Details
            detail::qtcp_socket socket;
            detail::qlistener listener;
    };
}