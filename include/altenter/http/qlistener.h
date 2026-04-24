#pragma once 

#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "altenter/info/qlog_manager.h"
#include "altenter/detail/qrequest_pool.h"
#include "altenter/detail/q_i_logger.h"
#include "altenter/qexception.h"

#define CLASSNAMELOG "qlistener"

namespace altenter::quokahttp::detail {
    class qlistener 
    {
        public:
            qlistener(qlog_manager& log_manager);
            ~qlistener();

            void listen();
            void set_socket(int qs_socket);

            void process(int socket);
        private:
            bool is_running;

            int qs_socket;
            qlog_manager& log_manager;
            qrequest_pool pool;
    };
}