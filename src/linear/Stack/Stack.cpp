#include <iostream>

#include "Stack.tpp"

class Data {
public:
    double value;
    // ~Data(){ std::cout << "~Data {" << value << "}" << std::endl; }

    Data() = default;
    Data(double value) : value(value) {}
    Data(Data & other){
        std::cout << "Data Copy Constructor" << std::endl;
        value = other.value;
    }
    Data & operator = (Data & other){
        std::cout << "Data Copy Assignment Operator" << std::endl;
        if(this != &other){
            value = other.value;
        }
        return *this;
    }
	Data(Data && other){
        std::cout << "Data Move Constructor" << std::endl;
        value = other.value;
        other.value = 0;
    }
	Data & operator = (Data && other){
        std::cout << "Data Move Assignment Operator" << std::endl;
        if(this != &other){
            value = other.value;
            other.value = 0;
        }
        return *this;
    }
};


int main(){
    Stack<Data> stack;
    for(int i = 0; i < 10; i++){
        stack.push(Data(i+1));
    }

    for(int n = stack.size(), i = 0; i < n + 2; i++){
        std::cout << "--- index " << i << " ---" << std::endl;
        // std::cout << stack.element() << std::endl; // throws an error when stack is empty
        auto peek = stack.peek();
        if(peek.has_value()){
            Data& ref = peek.value();
            std::cout << ref.value << std::endl;
        } else {
            std::cout << "peek return value is empty" << std::endl;
        }

        // std::cout << stack.pop() << std::endl; // throws an error when stack is empty
        auto poll = stack.poll(); // x2 move
        if(poll.has_value()){
            Data& ref = poll.value();
            std::cout << ref.value << std::endl;
        } else {
            std::cout << "poll return value is empty" << std::endl;
        }
    }

}

/*
Build and Run:
g++ Stack.cpp -Wall -o Stack.o && ./Stack.o
*/
