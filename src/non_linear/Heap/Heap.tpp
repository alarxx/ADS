/*
    SPDX-License-Identifier: MPL-2.0
    --------------------------------
    This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
    If a copy of the MPL was not distributed with this file,
    You can obtain one at https://mozilla.org/MPL/2.0/.

    Provided “as is”, without warranty of any kind.

    Copyright © 2025 Alar Akilbekov. All rights reserved.
 */

#pragma once
#ifndef _HEAP_TPP_
#define _HEAP_TPP_

#include <iostream> // cout, endl
#include <algorithm> // swap, min/max
#include <stdexcept> // out_of_range, runtime_error

// src/non_linear/Heap/main.cpp
#include "../../linear/ArrayList/ArrayList.tpp"

using arraylist::ArrayList;

namespace heap {

/*

Demo:
https://www.cs.usfca.edu/~galles/visualization/Heap.html

Heap is a complete binary tree, i.e.:
- Each level of the tree is filled, except the last one
- Each level is filled from left to right

2 types of Heap:
- Min Heap, i.e. list[parent(i)] < list[child(i)]
- Max Heap, i.e. list[parent(i)] > list[child(i)]

Heap-order property:
The data item stored in each node is smaller/greater or equal to any of the data items stored in its children.

- Heap allows us to find Min/Max element in O(1)
- Extracting Min/Max is O(log(N)) because of heapify i.e. traverseDown
    root is replaced with the last element then heapify down from root
- Insert is O(log(N)) becase of traverseUp

ArrayList implementation:

root = i / 2
left child = 2i
right child = 2i + 1

   1
 2   3
4 5 6 7

Number of levels = log2(size) + 1

 */
template <typename T>
class Heap { // Min Heap
protected:
    /*
    // Можно имплементировать используя Node, но sizeof(Node) = 24
    template <typename T>
    class Node {
        T data;
        Node<T> * left;  // data <= left.data
        Ndoe<T> * right; // data <= right.data
    };
     */
    ArrayList<T> list;
public:
    Heap(){
        list.add(T());
    }

    T& getMin(){
        if(list.size() == 0){
            throw std::out_of_range("Heap is empty!");
        }
        return list[1];
    }
    T extractMin(){
        // get 1
        T& ref = getMin();
        T item = std::move(ref);
        // set 1 <- size - 1
        list[1] = list[list.size() - 1];
        list.removeLast();
        heapify();
        return item;
    }
    void insert(T item){
        list.add(item);
        std::cout << " --- Insert " << item << " ---" << std::endl;
        traverseUp(list.size() - 1);
    }

    bool empty(){ return list.size() == 0; }
    int size(){ return list.size(); }

    ArrayList<T>& data(){ return list; }

private:
    // 0 is -inf
    int leftChildOf(int index){ return 2 * index; }
    int rightChildOf(int index){ return 2 * index + 1; }
    int parentOf(int index){ return index / 2; }

    // traverseDown
    void heapify(int index = 1){
        int root = index;
        if(root >= list.size() - 1){
            std::cout << "Last: I have no childs!" << std::endl;
            return;
        }

        int left = leftChildOf(root);
        int right = rightChildOf(root);

        std::cout << "root: " << root << ", value: " << list[root] << std::endl;
        std::cout << "left: " << left << ", value: " << list[left] << std::endl;
        std::cout << "right: " << right << ", value: " << list[right] << (right >= list.size() ? " (X)" : "") << std::endl;

        int minChild = right >= list.size() ? left : list[left] < list[right] ? left : right;

        if(list[minChild] < list[root]){
            swap(minChild, root);
        }

        heapify(minChild);
    }

    void traverseUp(int index){
        int root = parentOf(index);
        if(root <= 0 /*never less actually*/ ){
            std::cout << "Super root: I have no roots!" << std::endl;
            return;
        }

        int left = leftChildOf(root);
        int right = rightChildOf(root);

        std::cout << "root: " << root << ", value: " << list[root] << std::endl;
        std::cout << "left: " << left << ", value: " << list[left] << std::endl;
        std::cout << "right: " << right << ", value: " << list[right] << (right >= list.size() ? " (X)" : "") << std::endl;

        int minChild = right >= list.size() ? left : list[left] < list[right] ? left : right;

        if(list[minChild] < list[root]){
            swap(minChild, root);
        }

        traverseUp(root);
    }

    void swap(int index1, int index2){
        std::swap(list[index1], list[index2]);
    }
};

} // namespace Heap

#endif
