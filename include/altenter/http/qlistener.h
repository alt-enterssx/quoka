#pragma once 

#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "altenter/http/dto/qrequest.h"
#include "altenter/http/dto/qresponse.h"
#include "altenter/http/qrouter.h"
#include "altenter/info/qlog_manager.h"
#include "altenter/detail/qrequest_pool.h"
#include "altenter/detail/q_i_logger.h"
#include "altenter/qexception.h"

#define CLASSNAMELOG "qlistener"

namespace altenter::quoka::detail 
{
    class qlistener 
    {
        public:
            qlistener(qrouter& router);
            ~qlistener();

            void listen();
            void set_socket(int qs_socket);

            void process(int socket);
        private:    
            bool is_running;
        
            int qs_socket;
            qrouter& router;
            qrequest_pool pool;
    };
}