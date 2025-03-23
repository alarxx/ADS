#include "ArrayList.tpp"

using arraylist::ArrayList;

// --- main ---
int main(){
    // ArrayList<double> list; // list(5) by default, 2 times increaseBuffer
    ArrayList<double> list(7);

    // adding
    int a = 42;
    for(int i = 0; i < 10; i++){
        list.add(a);
        std::cout << i << ": " << a << std::endl;
        a++;
    }

    std::cout << "\nfor loop with index operator example:" << std::endl;
    for(int i = 0; i < list.size(); i++){
        // std::cout << list.get(i) << std::endl;
        list[i] *= 2; // purpose is to show that index return non-const variable
        std::cout << list[i] << std::endl;
    }

    std::cout << "\nRange-based for loop with iterator example:" << std::endl;
    for(auto & e: list){
        e *= 2;
        std::cout << e << std::endl;
    }
}

/*
Build and Run:
g++ ArrayList.cpp -Wall -o ArrayList.o && ./ArrayList.o
*/
