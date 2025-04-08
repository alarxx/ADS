#include <iostream>
#include <string>

#include "BST.tpp"

// src/non_linear/BST/main.cpp
#include "../../utils/random/random.hpp"

int main(){
    std::cout << "--- BST/main.cpp execution started! ---" << std::endl;

    // std::string str = "1";
    // std::cout << (str > "2") << std::endl;
    // std::cout << (str == "1") << std::endl;
    BST<std::string, int> bst;

    // /*Get is O(log(N) in average, and O(N) in worst case:)*/ {
    //     Random<int> rand(1000, 9999); // 4 symbols
    //     for(int i = 0; i < 1000; ++i){ // 1000 elements
    //         bst.insert(std::to_string(rand()), (i + 1) * 42);
    //     }
    //
    //     // Insert: O(log(N))
    //     bst.insert("unique", 101); // 1000 + 1 elements
    //
    //     // Search: O(log(N)), in worst case O(N)
    //     // to find key "unique" we will traverse in average log2(1001)=10 elements
    //     // Но может построится несбалансированное дерево, так что может быть больше, чем log2(1001)=10
    //     auto & found = bst.get("unique"); // BST<std::string, int>::Node<std::string, int>
    //     std::cout << "key: " << found.key << ", value: " << found.value << std::endl;
    // }

    // for(int i = 0; i < 10; ++i){
    //     bst.insert(std::to_string(i), i);
    // }

    // [1, 7]
    bst.insert("6", 6);
    bst.insert("4", 4);
    bst.insert("5", 5);
    bst.insert("7", 7);
    bst.insert("2", 2);
    bst.insert("1", 1);
    bst.insert("3", 3);

    std::cout << "\n--- Initial BST: ---" << std::endl;
    bst.vprint(); // sorted by key values
    std::cout << "Height: " << bst.get_height() << std::endl;

    bst.get_min() = 11; // set value
    bst.get_max() = 77;

    std::cout << "\n--- Delete Node example: ---" << std::endl;
    // bst.remove("3"); // you will see 2 destructors of node {"3", 3}
    auto d = bst.remove("2"); // BST<std::string, int>::Node<std::string, int>
    std::cout << "deleted: " << d.value << std::endl;

    std::cout << "\n--- Inorder traversal example: ---" << std::endl;
    bst.inorder_traversal(); // sorted by key values

    std::cout << "\n--- BST after manipulations: ---" << std::endl;
    bst.vprint(); // sorted by key values
    std::cout << "Height: " << bst.get_height() << std::endl;

    std::cout << "\n--- BST/main.cpp execution ended! ---" << std::endl;
}

/*

g++ main.cpp -Wall -std=c++23 -o app.out && ./app.out

*/
