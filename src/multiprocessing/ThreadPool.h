#pragma once

#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <vector>


class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()> > tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::condition_variable done_condition;
    bool stopped;
    size_t active_workers;

public:
    explicit ThreadPool(size_t num_threads);

    ~ThreadPool();

    template<typename Func, typename... Args>
    void enqueue(Func &&f, Args &&... args) { {
            auto task = [func = std::forward<Func>(f), ... args_ = std::forward<Args>(args)]() mutable {
                std::invoke(std::move(func), std::move(args_)...);
            };
            std::lock_guard lock(queue_mutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

    void wait_all();

    [[nodiscard]] size_t size() const;

    void shutdown();
};
