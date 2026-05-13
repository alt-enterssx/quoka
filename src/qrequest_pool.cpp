#include "altenter/qrequest_pool.h"

using namespace altenter::quoka::detail;

qrequest_item::qrequest_item(std::function<void(int)> func, int client_socket) noexcept : id(-1), executor(func), status(request_status::QUEUE), client_socket(client_socket) {}
qrequest_item::qrequest_item() noexcept : id(-1), status(request_status::QUEUE) {}

qrequest_item::~qrequest_item() {
    this->status = request_status::COMPLETE;
}

void qrequest_item::set_id(int id) noexcept { this->id = id; }
int qrequest_item::get_id() const noexcept { return this->id; }

void qrequest_item::execute() const noexcept {
    this->executor(this->client_socket);
}

qrequest_pool::qrequest_pool() noexcept : running(true) {
    int thread_ctn = qconfig::qoption<int>().option("pool.threads").default_value(DEFAULT_THREADS_CNT).value();

    qlog_manager::manager().logFormat(
        "Request pool initialized with: {} threads",
        log_type::INFO, 
        thread_ctn
    );

    for (int i = 0; i < thread_ctn; i++) {
        threads.emplace_back([this] () -> void {
            while(true) {
                qrequest_item task_item;

                {
                    std::unique_lock<std::mutex> lk(this->mtx);
                    this->cv.wait(lk, [this] () -> bool {
                        return !this->requests.empty() && this->running;
                    });

                    if (
                        !this->running && this->requests.empty()
                    ) { return; }

                    task_item = std::move(this->requests.back());
                    this->requests.pop();
                }

                task_item.execute();
            }
        });
    }
}

void qrequest_pool::add_task(qrequest_item& item) noexcept {
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

void qrequest_pool::shutdown() noexcept {
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

qrequest_pool::~qrequest_pool() noexcept {}