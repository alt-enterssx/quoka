#include "altenter/detail/qrequest_pool.h"

using namespace altenter::quokahttp::detail;

qrequest_item::qrequest_item(std::function<void(int)> func, int client_socket): id(-1), executor(func), status(request_status::QUEUE), client_socket(client_socket) {}
qrequest_item::qrequest_item(): id(-1), status(request_status::QUEUE) {}

qrequest_item::~qrequest_item() {
    this->status = request_status::COMPLETE;
}

void qrequest_item::set_id(int id) { this->id = id; }
int qrequest_item::get_id() { return this->id; }

void qrequest_item::execute() {
    this->executor(this->client_socket);
}

qrequest_pool::qrequest_pool(): running(true) {
    int thread_cnt = -1;
    int conf_threads = qconfig::qoption<int>::option("pool.threads");

    if (conf_threads != 0) {
        thread_cnt = conf_threads;
    }
    else {
        thread_cnt = DEFAULT_THREADS_CNT;
    }

    qlog_manager::manager().logFormat("Request pool initialized with: {} threads", log_type::INFO, thread_cnt);

    for (int i = 0; i < thread_cnt; i++) {
        threads.emplace_back([this] () -> void {
            while(true) {
                qrequest_item task_item;

                {
                    std::unique_lock<std::mutex> lk(this->mtx);
                    this->cv.wait(lk, [this] () -> bool {
                        return !this->requests.empty() && this->running;
                    });

                    if (!this->running && this->requests.empty()) { return; }

                    task_item = std::move(this->requests.back());
                    this->requests.pop();
                }

                task_item.execute();
            }
        });
    }
}

void qrequest_pool::add_task(qrequest_item& item) {
    {
        std::lock_guard<std::mutex> lk(this->mtx);
        item.set_id(this->requests.size() + 1);
        this->requests.push(item);
    }

    cv.notify_all();
}

void qrequest_pool::wait() {
    for (std::thread& t : this->threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void qrequest_pool::shutdown() {
    {
        std::lock_guard<std::mutex> lg(this->mtx);
        this->running = false;
    }

    this->cv.notify_all();

    for (std::thread& t : this->threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

qrequest_pool::~qrequest_pool() {}