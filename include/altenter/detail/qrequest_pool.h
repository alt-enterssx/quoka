#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "altenter/utils/qconfig.h"
#include "altenter/info/qlog_manager.h"

#define DEFAULT_THREADS_CNT 4

namespace altenter::quoka::detail
{    
    enum request_status
    {
        COMPLETE,
        QUEUE
    };
    
    class qrequest_item
    {
        public:
            qrequest_item(std::function<void(int)> func, int client_socket);
            qrequest_item();

            ~qrequest_item();

            int get_id();
            void set_id(int id);

            void execute();
        private:
            request_status status;
            int id;

            std::function<void(int)> executor;

            int client_socket;
    };

    class qrequest_pool
    {
        public:
            qrequest_pool();
            ~qrequest_pool();

            void add_task(qrequest_item& task);

            void wait();
            void shutdown();

        private:
            bool running;

            std::vector<std::thread> threads;
            std::queue<qrequest_item> requests;

            std::mutex mtx;
            std::condition_variable cv;
    };
}

