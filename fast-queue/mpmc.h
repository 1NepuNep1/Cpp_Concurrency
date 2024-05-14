#pragma once

#include <vector>
#include <atomic>

template <class T>
class MPMCBoundedQueue {
public:
    explicit MPMCBoundedQueue(int size) : max_size_(size), segments_(size) {
        for (auto i = 0; i < size; ++i) {
            segments_[i].generation.store(i);
        }
    }

    bool Enqueue(const T& value) {
        auto const_tail = tail_.load();
        while (true) {
            Segment* segment = &segments_[const_tail & (max_size_ - 1)];
            auto segment_generation = segment->generation.load();
            int dist = segment_generation - const_tail;
            if (dist == 0) {
                if (tail_.compare_exchange_weak(const_tail, const_tail + 1)) {
                    segment->value_ = value;
                    segment->generation.fetch_add(1);
                    return true;
                }
            } else if (dist > 0) {
                const_tail = tail_.load();
            } else {
                return false;
            }
        }
    }

    bool Dequeue(T& data) {
        auto const_head = head_.load();
        while (true) {
            Segment* segment = &segments_[const_head & (max_size_ - 1)];
            auto segment_generation = segment->generation.load();
            int dist = segment_generation - const_head;
            if (dist == 1) {
                if (head_.compare_exchange_weak(const_head, const_head + 1)) {
                    data = segment->value_;
                    segment->generation.fetch_add(max_size_ - 1);
                    return true;
                }
            } else if (dist == 0) {
                return false;
            } else {
                const_head = head_.load();
            }
        }
    }

private:
    struct Segment {
        T value_;
        std::atomic<int> generation;
    };
    std::vector<Segment> segments_;
    int max_size_ = 0;
    std::atomic<int> head_ = {0};
    std::atomic<int> tail_ = {0};
};
