#pragma once

#include <linux/futex.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

#include <atomic>

// Atomically do the following:
//    if (*value == expected_value) {
//        sleep_on_address(value)
//    }
void FutexWait(int *value, int expected_value) {
    syscall(SYS_futex, value, FUTEX_WAIT_PRIVATE, expected_value, nullptr, nullptr, 0);
}

// Wakeup 'count' threads sleeping on address of value(-1 wakes all)
void FutexWake(int *value, int count) {
    syscall(SYS_futex, value, FUTEX_WAKE_PRIVATE, count, nullptr, nullptr, 0);
}

class Mutex {
public:
    void Lock() {
        int x = cmpxchg(&counter_, 0, 1);
        if (x != 0) {
            if (x != 2) {
                x = counter_.exchange(2);
            }
            while (x != 0) {
                FutexWait(reinterpret_cast<int *>(&counter_), 2);
                x = counter_.exchange(2);
            }
        }
    }

    void Unlock() {
        if (counter_.fetch_sub(1) != 1) {
            counter_.store(0);
            FutexWake(reinterpret_cast<int *>(&counter_), 1);
        }
    }

private:
    std::atomic<int> counter_ = {0};

    int cmpxchg(std::atomic<int> *atomic, int a, int b) {  // NOLINT
        int *ep = &a;
        atomic->compare_exchange_strong(a, b);
        return *ep;
    }
};
