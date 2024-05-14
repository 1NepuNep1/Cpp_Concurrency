#pragma once

#include <atomic>
#include <thread>

struct RWSpinLock {
    void LockRead() {
        int expected = 0;
        while ((count_.load() & 1) != 0 || !count_.compare_exchange_weak(expected, expected + 2)) {
            std::this_thread::yield();
            expected -= expected % 2;
        }
    }

    void UnlockRead() {
        count_.fetch_sub(2);
    }

    void LockWrite() {
        int expected = 0;
        while (!count_.compare_exchange_weak(expected, expected + 1)) {
            std::this_thread::yield();
            expected = 0;
        }

        while (count_.load() > 1) {
            std::this_thread::yield();
        }
    }

    void UnlockWrite() {
        count_.store(0);
    }

private:
    std::atomic<int> count_{0};
};
