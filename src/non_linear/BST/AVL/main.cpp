#include <iostream>
#include <string>

#include "AVL.tpp"

int main(){
    std::cout << "--- AVL/main.cpp execution started! ---" << std::endl;

    AVL<std::string, int> avl;

    std::cout << "--- (+1): ---" << std::endl;
    avl.insert("1", 1);
    // avl.insert("1", 11);
    avl.vprint();

    std::cout << "--- (+2): ---" << std::endl;
    avl.insert("2", 2);
    avl.vprint();

    std::cout << "--- (+3): ---" << std::endl;
    avl.insert("3", 3);
    avl.vprint();

    std::cout << "--- (+4): ---" << std::endl;
    avl.insert("4", 4);
    avl.vprint();

    std::cout << "--- (+5): ---" << std::endl;
    avl.insert("5", 5);
    avl.vprint();

    std::cout << "\n--- AVL/main.cpp execution ended! ---" << std::endl;
}

/*

g++ main.cpp -Wall -std=c++23 -o app.out && ./app.out

*/
