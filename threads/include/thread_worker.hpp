#pragma once

#include <queue>
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>

using Task = std::function<void()>;

class Executor {
public:
    Executor()
    : thread_{&Executor::Loop, this} {
    }

    // Thread - safe
    void Add(Task task) {
        {
            std::lock_guard _{mutex_};
            tasks_.push(std::move(task));
        }
        cv_.notify_one();
    }

    // Thread - safe
    void Stop() {
        {
            std::lock_guard _{mutex_};
            stop_ = true;
        }
        cv_.notify_one();
        thread_.join();
    }

private:

    void Loop() {
        for (;;) {
            Task new_task;
            {
                std::unique_lock lock{mutex_};
                cv_.wait(lock, [this](){ return stop_ || !tasks_.empty(); });
                if (stop_) {
                    return;
                }
                new_task = tasks_.front();
                tasks_.pop();
            }
            new_task();
        }
    }

    bool stop_ = false;
    std::condition_variable cv_;
    std::mutex mutex_;
    std::queue<Task> tasks_;
    std::thread thread_;
};

