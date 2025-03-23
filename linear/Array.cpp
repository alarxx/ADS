#include <iostream>

int main(){
    int arr[] = {41, 42, 43, 44, 45};

    // arr.length
    if(sizeof(arr) == 0) return 1;
    int n = sizeof(arr) / sizeof(arr[0]);

    // O(N) = O(N) * O(1)
    for(int i = 0; i < n; i++){ // O(N)
        std::cout << arr[i] << std::endl; // O(1)
    }

    // range-based for loop in C++ / for-each loop in Java
    // for(int e: arr){
    //     std::cout << e << std::endl;
    // }

    return 0;
}

/*
Build and Run:
g++ Array.cpp -Wall -o Array.o && ./Array.o
*/
