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

            void get_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec);
            void post_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec);
            void put_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec);
            void delete_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec);
            void patch_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec);
            void options_point(const std::string& uri, std::function<void(qrequest& request, qresponse& response)> exec);

        private:

            // | Datas
            bool is_running;
            int qs_port;

            // | Classes
            qrouter router;

            // | Details
            detail::qtcp_socket socket;
            detail::qlog_manager log_manager;
            detail::qlistener listener;
    };
}