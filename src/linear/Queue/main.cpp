#include <iostream>

#include "Queue.tpp"

int main(){
    Queue<double> queue;
    for(int i = 0; i < 10; i++){
        // queue.push(i + 1.);
        std::cout << "offer: " << i + 1. << std::endl;
        bool ok = queue.offer(i + 1.);
        if(!ok){
            std::cout << "failed to add" << std::endl;
        }
    }

    for(int n = queue.size(), i = 0; i < n + 2; i++){
        std::cout << "--- index " << i << " ---" << std::endl;
        // // std::cout << queue.element() << std::endl; // throws an error when queue is empty
        // auto peek = queue.peek();
        // if(peek.has_value()){
        //     std::cout << peek.value() << std::endl;
        // } else {
        //     std::cout << "peek return value is empty" << std::endl;
        // }

        // std::cout << queue.pop() << std::endl; // throws an error when queue is empty
        auto poll = queue.poll(); // x2 move
        if(poll.has_value()){
            std::cout << poll.value() << std::endl;
        } else {
            std::cout << "poll return value is empty" << std::endl;
        }
        std::cout << "------" << std::endl;
    }

}

/*
Build and Run:
g++ main.cpp -Wall -o main.o && ./main.o
*/
