#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>

template <class T>
class TimerQueue {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;

private:
    std::multimap<TimePoint, T> pairs_;
    std::mutex mutex_;
    std::condition_variable condvar_;

public:
    void Add(const T& item, TimePoint at) {
        std::unique_lock<std::mutex> locked(mutex_);
        pairs_.emplace(at, item);
        condvar_.notify_one();
    }

    T Pop() {
        std::unique_lock<std::mutex> locked(mutex_);
        while (pairs_.empty()) {
            condvar_.wait(locked);
        }

        for (auto it = pairs_.begin(); it != pairs_.end(); ++it) {
            if (it->first <= Clock::now()) {
                T value = it->second;
                pairs_.erase(it);
                return value;
            }
        }
        auto next = pairs_.begin();
        condvar_.wait_until(locked, next->first);
        T value = next->second;
        pairs_.erase(next);
        return value;
    }
};
