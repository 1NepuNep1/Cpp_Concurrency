#pragma once

#include <utility>
#include <optional>
#include <mutex>
#include <condition_variable>
#include <queue>

template <class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) : size_(size) {
    }

    void Send(const T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        write_cv_.wait(lock, [this] { return closed_.load() || actual_size_.load() < size_; });
        if (closed_) {
            throw std::runtime_error("Channel is closed. Please, wait.");
        }
        queue_.push(value);
        actual_size_.fetch_add(1);
        write_cv_.notify_one();
        recv_cv_.notify_one();
    }

    std::optional<T> Recv() {
        std::unique_lock<std::mutex> lock(mutex_);
        recv_cv_.wait(lock, [this] { return actual_size_.load() > 0 || closed_.load(); });
        if (actual_size_.load() == 0 && closed_) {
            return std::nullopt;
        }
        T value = queue_.front();
        queue_.pop();
        actual_size_.fetch_sub(1);
        write_cv_.notify_one();
        recv_cv_.notify_one();
        return value;
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mutex_);
        closed_.store(true);
        recv_cv_.notify_all();
        write_cv_.notify_all();
    }

private:
    std::mutex mutex_;
    std::condition_variable recv_cv_;
    std::condition_variable write_cv_;
    std::queue<T> queue_;
    int size_;
    std::atomic<size_t> actual_size_ = {0};
    std::atomic<bool> closed_ = {false};
};
