#include "ThreadPool.h"


ThreadPool::ThreadPool(const size_t num_threads) : stopped(false), active_workers(0) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this] {
            for (;;) {
                std::function<void()> task; {
                    std::unique_lock lock(this->queue_mutex);
                    this->condition.wait(lock, [this] {
                        return this->stopped || !this->tasks.empty();
                    });
                    if (this->stopped && this->tasks.empty()) {
                        return;
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                    ++active_workers;
                }

                task(); {
                    std::lock_guard lock(this->queue_mutex);
                    --active_workers;
                    if (tasks.empty() && active_workers == 0) {
                        done_condition.notify_all();
                    }
                }
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::wait_all() {
    std::unique_lock lock(queue_mutex);
    done_condition.wait(lock, [this] {
        return tasks.empty() && active_workers == 0;
    });
}

size_t ThreadPool::size() const {
    return workers.size();
}

void ThreadPool::shutdown() { {
        std::lock_guard lock(queue_mutex);
        stopped = true;
    }
    condition.notify_all();
    for (auto &t: workers) {
        if (t.joinable()) {
            t.join();
        }
    }
}
