#pragma once

#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <vector>


class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads) : stop_flag(false), active_workers(0) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock lock(this->queue_mutex);
                        this->condition.wait(lock, [this]() {
                            return this->stop_flag || !this->tasks.empty();
                        });
                        if (this->stop_flag && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                        ++active_workers;
                    }

                    task();

                    {
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

    ~ThreadPool() {
        shutdown();
    }

    void enqueue(std::function<void()> task) {
        {
            std::lock_guard lock(queue_mutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

    void wait_all() {
        std::unique_lock lock(queue_mutex);
        done_condition.wait(lock, [this]() {
            return tasks.empty() && active_workers == 0;
        });
    }

    void shutdown() {
        {
            std::lock_guard lock(queue_mutex);
            stop_flag = true;
        }
        condition.notify_all();
        for (auto& t : workers) {
            if (t.joinable())
                t.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    std::condition_variable done_condition;
    bool stop_flag;
    size_t active_workers;
};
