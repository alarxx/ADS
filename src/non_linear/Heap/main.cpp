/*
    SPDX-License-Identifier: MPL-2.0
    --------------------------------
    This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
    If a copy of the MPL was not distributed with this file,
    You can obtain one at https://mozilla.org/MPL/2.0/.

    Provided “as is”, without warranty of any kind.

    Copyright © 2025 Alar Akilbekov. All rights reserved.
 */

#include <iostream> // cout, endl
#include <iostream> // cout, endl

#include "Heap.tpp"

using heap::Heap;

// int main(){
//     std::cout << "hi" << std::endl;
//
//     Heap<double> heap;
//     for(int i = 3; i >= 0; i--){ // O(N*log(N))
//         heap.insert(42. + i);
//     }
//
//     double item = heap.extractMin();
//     std::cout << "Min: " << item << std::endl;
//
//     // print
//     ArrayList<double>& list = heap.data();
//     for(int i = 0; i < list.size(); i++){
//         std::cout << "print: " << list[i] << std::endl;
//     }
//
//     std::cout << "Min: " << heap.getMin() << std::endl;
// }

int main(){

    std::vector<double> vec;
    for(int i = 0; i < 5; i++){
        vec.emplace_back(42. + (4 - i));
        std::cout << vec[i] << std::endl;
    }

    Heap<double> heap;
    heap.buildHeap(vec);
    // print
    ArrayList<double>& list = heap.data();
    for(int i = 0; i < list.size(); i++){
        std::cout << "print: " << list[i] << std::endl;
    }
}

/*
Build and Run:
g++ main.cpp -Wall -o main.o && ./main.o
*/
