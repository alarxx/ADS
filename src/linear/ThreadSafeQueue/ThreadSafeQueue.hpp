/*
    SPDX-License-Identifier: MPL-2.0
    --------------------------------
    This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
    If a copy of the MPL was not distributed with this file,
    You can obtain one at https://mozilla.org/MPL/2.0/.

    Provided “as is”, without warranty of any kind.

    Copyright © 2026 Alar Akilbekov. All rights reserved.
*/
#pragma once
#ifndef _THREADSAFEQUEUE_
#define _THREADSAFEQUEUE_

#include <mutex>
#include <condition_variable>


template <typename T>
class ThreadSafeQueue {
private:
    // Node
    class Node {
    public:
        T value;
        Node * next; // singly linked list
        // Constructors
        Node() = delete;
        explicit Node(const T & v) : value(v), next(nullptr) {}
        explicit Node(T && v) : value(std::move(v)), next(nullptr) {}
    };

    // Fields
    Node * head;
    Node * tail;
    int size;

    mutable std::mutex m;
    std::condition_variable cv;

public:
    // Default Constructor
    ThreadSafeQueue(){
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    // Destructor
    ~ThreadSafeQueue() {
        std::lock_guard<std::mutex> lk(m);
        Node * current = head;
        while (current != nullptr) {
            Node * next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    // No copy constructor and no assignment operator
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    bool empty() const {
        std::lock_guard<std::mutex> lk(m);
        return size == 0;
    }

    int getSize() const {
        std::lock_guard<std::mutex> lk(m);
        return size;
    }

    void push(const T & v) {
        Node * node = new Node(v); // copy of v
        {
            std::lock_guard<std::mutex> lk(m);
            if (tail == nullptr) {
                head = tail = node;
            } else {
                tail->next = node;
                tail = node;
            }
            ++size;
        }
        cv.notify_one();
    }

    // Non-blocking pop
    bool try_pop(T & out) {
        std::lock_guard<std::mutex> lk(m);
        if (head == nullptr){ // empty
            return false;
        }
        Node * node = head;

        // if this throws, queue remains unchanged
        out = std::move(node->value);

        head = node->next;
        if (head == nullptr){
            tail = nullptr;
        }
        --size;

        delete node;

        return true;
    }

    // Blocking pop
    void wait_and_pop(T& out) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&]()->bool{
            return head != nullptr; // spurious wakeup
        });
        Node * node = head;
        out = std::move(node->value);

        head = node->next;
        if (head == nullptr){
            tail = nullptr;
        }
        --size;
        lk.unlock();
        delete node;
    }
};

#endif
