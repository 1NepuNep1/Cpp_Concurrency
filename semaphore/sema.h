#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

class DefaultCallback {
public:
    void operator()(int& value) {
        --value;
    }
};

class Semaphore {
public:
    Semaphore(int count) : count_(count) {
    }

    void Leave() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        if (!queue_.empty()) {
            queue_.front()->notify_one();
            queue_.pop();
        }
    }

    template <class Func>
    void Enter(Func callback) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (count_ <= 0) {
            auto cv = std::make_shared<std::condition_variable>();
            queue_.push(cv);
            cv->wait(lock, [this, cv] {
                return cv == queue_.front();
            });
        }
        callback(count_);
    }

    void Enter() {
        DefaultCallback callback;
        Enter(callback);
    }

private:
    std::mutex mutex_;
    int count_ = 0;
    std::queue<std::shared_ptr<std::condition_variable>> queue_;
};
