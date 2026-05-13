#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "altenter/qconfig.h"
#include "altenter/qlog_manager.h"

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
            qrequest_item(std::function<void(int)> func, int client_socket) noexcept;
            qrequest_item() noexcept;

            ~qrequest_item() noexcept;

            int get_id() const noexcept;
            void set_id(int id) noexcept;

            void execute() const noexcept;
        private:
            request_status status;
            int id;

            std::function<void(int)> executor;

            int client_socket;
    };

    class qrequest_pool
    {
        public:
            qrequest_pool() noexcept;
            ~qrequest_pool() noexcept;

            void add_task(qrequest_item& task) noexcept;

            void wait() noexcept;
            void shutdown() noexcept;

        private:
            bool running;

            std::vector<std::thread> threads;
            std::queue<qrequest_item> requests;

            std::mutex mtx;
            std::condition_variable cv;
    };
}

