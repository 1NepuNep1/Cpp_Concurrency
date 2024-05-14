#pragma once

#include <utility>
#include <optional>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>

template <class T>
class UnbufferedChannel {
public:
    void Send(const T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (closed_) {
            throw std::runtime_error("Channel is closed. Please, wait.");
        }
        queue_.push(value);
        received_.store(true);
        recv_cv_.notify_one();
        send_cv_.wait(lock, [this] { return closed_.load() || !received_.load(); });
        send_cv_.notify_one();
    }

    std::optional<T> Recv() {
        std::unique_lock<std::mutex> lock(mutex_);
        recv_cv_.wait(lock, [this] { return received_ || closed_ || !queue_.empty(); });
        if (closed_ && !received_ && queue_.empty()) {
            return std::nullopt;
        }
        T value = queue_.front();
        queue_.pop();
        received_.store(false);
        send_cv_.notify_one();
        recv_cv_.notify_one();
        return value;
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mutex_);
        closed_.store(true);
        recv_cv_.notify_all();
        send_cv_.notify_all();
    }

private:
    std::mutex mutex_;
    std::condition_variable send_cv_;
    std::condition_variable recv_cv_;
    std::queue<T> queue_;
    std::atomic<bool> closed_ = {false};
    std::atomic<bool> received_ = {false};
};
