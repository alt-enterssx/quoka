#pragma once 

#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "altenter/qrequest.h"
#include "altenter/qresponse.h"
#include "altenter/qrouter.h"
#include "altenter/qlog_manager.h"
#include "altenter/qrequest_pool.h"
#include "altenter/q_i_logger.h"
#include "altenter/qexception.h"

#define CLASSNAMELOG "qlistener"

namespace altenter::quoka::detail 
{
    class qlistener 
    {
        public:
            qlistener();
            ~qlistener();

            void listen();
            void set_socket(int qs_socket);

            void process(int socket);
        private:    
            bool is_running;
        
            int qs_socket;
            qrequest_pool pool;
    };
}