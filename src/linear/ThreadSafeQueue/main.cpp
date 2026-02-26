#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "ThreadSafeQueue.hpp"


void producer_thread(int id, ThreadSafeQueue<int> & queue){
    for (int i = 0; i < 10; ++i) { // try to tweak number of iterations
        int value = id * 100 + i;
        queue.push(value);
        std::cout << "Producer " << id << " pushed " << value << std::endl;
        // producer1, producer2, producer1, producer2, ...
        std::this_thread::sleep_for(std::chrono::milliseconds(id * 500));
    }
}


void consumer_thread(int id, ThreadSafeQueue<int>& queue){
    for (int i = 0; i < 10; ++i) { // try to tweak number of iterations
        int value;
        // wait_and_pop
        queue.wait_and_pop(value);
        // // try_pop
        // while (!queue.try_pop(value)) {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // }
        std::cout << "Consumer " << id << " popped " << value << std::endl;
    }
}


int main() {
    ThreadSafeQueue<int> queue;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // Producer threads
    for (int p = 0; p < 2; ++p) {
        producers.emplace_back(producer_thread, p+1, std::ref(queue));
    }

    // Consumer threads
    for (int c = 0; c < 2; ++c) {
        consumers.emplace_back(consumer_thread, c+1, std::ref(queue));
    }

    // Join producers and consumers
    for (std::thread & t : producers) t.join();
    for (std::thread & t : consumers) t.join();

    std::cout << "Final queue size: " << queue.getSize() << "\n";
}


/*
Producer 1 pushed 100
Producer 2 pushed 200 <- on first cycle both producers push values
Consumer 2 popped 100
Consumer 2 popped 200 <- starvation
                      <- 500ms
Producer 1 pushed 101
Consumer 2 popped 101 <- consumer awaits and immediately pops value
                      <- 500ms
Producer 2 pushed 201
Consumer 1 popped 201
                      <- 500ms etc.
Producer 1 pushed 102
Consumer 2 popped 102

Producer 1 pushed 103
Consumer 1 popped 103

Producer 2 pushed 202
Consumer 2 popped 202

Producer 1 pushed 104
Consumer 1 popped 104

Producer 1 pushed 105
Consumer 2 popped 105

Producer 2 pushed 203
Consumer 1 popped 203

Producer 1 pushed 106
Consumer 2 popped 106

Producer 1 pushed 107
Consumer 1 popped 107

Producer 2 pushed 204
Consumer 2 popped 204

Producer 1 pushed 108
Consumer 1 popped 108

Producer 1 pushed 109
Consumer 2 popped 109

Producer 2 pushed 205
Consumer 1 popped 205

Producer 2 pushed 206
Consumer 2 popped 206

Producer 2 pushed 207
Consumer 1 popped 207

Producer 2 pushed 208
Consumer 2 popped 208

Producer 2 pushed 209
Consumer 1 popped 209

Final queue size: 0
*/
