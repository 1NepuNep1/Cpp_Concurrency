#pragma once

#include <atomic>
#include <optional>
#include <stdexcept>
#include <utility>

template <class T>
class MPSCStack {
public:
    // Push adds one element to stack top.
    //
    // Safe to call from multiple threads.
    void Push(const T& value) {
        Node* new_node = new Node{value};
        Node* old_head = head_.load();

        do {
            new_node->next = old_head;
        } while (!head_.compare_exchange_weak(old_head, new_node));
    }

    // Pop removes top element from the stack.
    //
    // Not safe to call concurrently.
    std::optional<T> Pop() {
        Node* old_head = head_.load();
        if (old_head == nullptr) {
            return std::nullopt;
        }
        while (!head_.compare_exchange_weak(old_head, old_head->next)) {
        }

        T value = old_head->value;
        delete old_head;
        return value;
    }

    // DequeuedAll Pop's all elements from the stack and calls cb() for each.
    //
    // Not safe to call concurrently with Pop()
    template <class TFn>
    void DequeueAll(const TFn& cb) {
        Node* current = head_.exchange(nullptr);
        while (current != nullptr) {
            cb(current->value);
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    ~MPSCStack() {
        while (Pop()) {
        }
    }

private:
    struct Node {
        T value;
        Node* next = nullptr;
    };

    std::atomic<Node*> head_ = nullptr;
};
