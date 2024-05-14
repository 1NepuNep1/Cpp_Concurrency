#pragma once
#include <mutex>
#include <condition_variable>

class RWLock {
public:
    template <class Func>
    void Read(Func func) {
        LockRead();
        try {
            func();
        } catch (...) {
            EndRead();
            throw;
        }
        EndRead();
    }

    void LockRead() {
        std::unique_lock<std::mutex> lock(mutex_);
        read_cv_.wait(lock, [this] { return !is_writing_; });
        ++blocked_readers_;
    }

    template <class Func>
    void Write(Func func) {
        LockWrite();
        try {
            func();
        } catch (...) {
            EndWrite();
            throw;
        }
        EndWrite();
    }

    void LockWrite() {
        std::unique_lock<std::mutex> lock(mutex_);
        write_cv_.wait(lock, [this] { return !is_writing_ && blocked_readers_ == 0; });
        is_writing_ = true;
    }

private:
    std::mutex mutex_;
    std::condition_variable read_cv_;
    std::condition_variable write_cv_;
    int blocked_readers_ = 0;
    bool is_writing_ = false;

    void EndRead() {
        std::unique_lock<std::mutex> lock(mutex_);
        --blocked_readers_;
        if (blocked_readers_ == 0) {
            write_cv_.notify_one();
        }
    }

    void EndWrite() {
        std::unique_lock<std::mutex> lock(mutex_);
        is_writing_ = false;
        write_cv_.notify_one();
        read_cv_.notify_all();
    }
};
