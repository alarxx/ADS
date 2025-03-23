/*
    SPDX-License-Identifier: MPL-2.0
    --------------------------------
    This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
    If a copy of the MPL was not distributed with this file,
    You can obtain one at https://mozilla.org/MPL/2.0/.

    Provided “as is”, without warranty of any kind.

    Copyright © 2025 Alar Akilbekov. All rights reserved.
 */

#include <iostream>
#include <iterator> // bidirectional_iterator_tag, reverse_iterator
#include <cstddef> // ptrdiff_t
#include <type_traits> // enable_if_t
#include <stdexcept> // runtime_error
#include <algorithm> // swap

/* --- iterator ---
    From https://github.com/alarxx/Tensor-library

    Main article:
        https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp

    iterator_tags:
        https://en.cppreference.com/w/cpp/iterator/iterator_tags
        - Input (read-only)         input_iterator_tag
        - Output (write-only)       output_iterator_tag
        - Forward Iterator          forward_iterator_tag
        - Bidirectional Iterator    bidirectional_iterator_tag
        - Random Access Iterator    random_access_iterator_tag
        - Contiguous Iterator       contiguous_iterator_tag

    The properties of each iterator category are:
        https://cplusplus.com/reference/iterator/

    - iterator
    - constant_iterator - это по сути Input Iterator (read-only)
    - reverse_iterator
    - constant_reverse_iterator

    --- Output Iterator ---
    Мы здесь не реализуем чистый Output Iterator.
    Мы же dereferencing делаем так *_ptr,
    а при Ouput Iterator мы делаем dereferencing так *this,
    то есть это фиктивное разыменование, it returns itself,
    то есть возвращаем сам iterator, чтобы потом воспользовать assign operator =:
    *it = 42;

        struct Output {
            T* _ptr;
            Output& operator * (){ return *this; }
            Output& operator = (const T& data){
                *_ptr = data;
            }
        };
    ------
*/

// Does T has members next and prev
template <typename T, typename U = void>
class iterator {};

template <typename T> // T = LinkedList<U>::Node
class iterator <T, std::void_t<
    decltype(std::declval<T>().next),
    decltype(std::declval<T>().prev)
>> {
public:
    // https://en.cppreference.com/w/cpp/iterator/iterator_traits
    // https://en.cppreference.com/w/cpp/iterator/iterator_tags
    using iterator_category = std::bidirectional_iterator_tag;
    // https://en.cppreference.com/w/cpp/types/ptrdiff_t
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*; // value_type*
    using reference = T&; // value_type&
private:
    pointer _ptr;
public:
    iterator(pointer ptr) : _ptr(ptr) {}

    reference operator * (){ return *_ptr; }
    pointer operator -> (){ return _ptr; }

    // prefix increment
    iterator& operator ++ (){ _ptr = _ptr->next; return *this; }
    // postfix increment
    iterator operator ++ (int){
        iterator tmp = *this; // overhead on creation of temporary object
        ++(*this);
        return tmp;
    }

    // prefix decrement
    iterator& operator -- (){ _ptr = _ptr->prev; return *this; }
    // postfix decrement
    iterator operator -- (int){
        iterator tmp = *this; // overhead on creation of temporary object
        --(*this);
        return tmp;
    }

    friend bool operator == (const iterator& a, const iterator& b){ return a._ptr == b._ptr; }
    friend bool operator != (const iterator& a, const iterator& b){ return a._ptr != b._ptr; }

};

// ------


// --- LinkedList ---

template <typename T>
class LinkedList { // Doubly-Linked List
public:
    template <typename R>
    class Node {
    public:
        R data;
        Node<R> * next;
        Node<R> * prev; // Doubly-Linked List + 8 bytes
        Node(R data) : data(data), next(nullptr), prev(nullptr) {}
    };

private:
    Node<T> * _head;
    Node<T> * _tail;
    int _size;

public:
    explicit LinkedList() : _head(nullptr), _tail(nullptr), _size(0) {}

    ~LinkedList(){
        Node<T> * node = _head;
        Node<T> * next;
        // for(int i = 0; i < _size; i++){
        while(node != nullptr){
            std::cout << "allocate Node{" << node->data << "}" << std::endl;
            next = node->next; // memorize
            delete node; node = nullptr;
            node = next;
        }
    }

    int size(){ return _size; }

    // --- Append ---
    // add: O(1)
    void addLast(T item){
        Node<T> * new_node = new Node(item);
        if(_head == nullptr){
            std::cout << "initial" << std::endl;
            _head = new_node;
            _tail = _head;
        }
        else {
            new_node->prev = _tail;
            _tail->next = new_node; // old tail
            _tail = new_node;
            // _tail->next = nullptr
        }
        ++_size;
    }
    // add: O(1)
     void addFirst(T item){
        Node<T> * new_node = new Node(item);
        if(_head == nullptr){
            std::cout << "initial" << std::endl;
            _head = new_node;
            _tail = _head;
        }
        else {
            new_node->next = _head;
            _head->prev = new_node; // old tail
            _head = new_node;
            // _head->prev = nullptr
        }
        ++_size;
    }
    // ------

    // --- Remove ---
    // remove: O(1)
    T removeLast(){
        std::cout << "remove last, _size = (" << _size  << " -> " << _size - 1 << ")" << std::endl;

        Node<T> * tail = _tail;
        T data = tail->data;

        if(_size == 1){
            _head = nullptr;
            _tail = nullptr;
        }
        else {
            _tail = tail->prev;
            _tail->next = nullptr; // до этого tail.prev.next = tail

            delete tail;
            tail = nullptr;
        }

        --_size;

        return data;
    }
    // remove: O(1)
    T removeFirst(){
        std::cout << "remove first, _size = (" << _size  << " -> " << _size - 1 << ")" << std::endl;

        Node<T> * head = _head;
        T data = head->data;

        if(_size == 1){
            _head = nullptr;
            _tail = nullptr;
        }
        else {
            _head = head->next;
            _head->prev = nullptr; // до этого tail.prev.next = tail

            delete head;
            head = nullptr;
        }

        --_size;

        return data;
    }
    // ------

    // get: O(N)
    T get(int index){
        Node<T> * node = _head;
        // for(int i = 0; i < index; ++i, node = node->next){}
        while(index != 0){
            node = node->next;
            --index;
        }
        return node->data;
    }

    int find(const T item){
        Node<T> * node = _head;
        for(int i = 0; /*i < _size*/ node != nullptr; ++i){
            if(node->data == item){
                return i;
            }
            node = node->next;
        }
        return -1;
    }

    void reverse(){
        Node<T> * node = _head;
        _head = _tail;
        _tail = node;
        while(node != nullptr){
            Node<T> * tmp = node->prev;
            node->prev = node->next;
            node->next = tmp;
            node = node->prev; // it is like next
        }
    }

    // --- iterator ---
    using iterator = ::iterator<Node<T>>; // iterator variable shadowing, so we use :: - global namespace.
    // using iterator = ::iterator<T>;
    iterator begin(){ return iterator(_head); }
    iterator end(){ return iterator(_tail->next); }
    // ------
};

// ------


int main(){
    std::cout << "--- LinkedList.cpp execution started! ---" << std::endl;

    LinkedList<double> ll;
    for(int i = 0; i < 5; ++i){
        ll.addLast(42 + i); // 42, 43, 44, 45, 46
        // ll.addFirst(42 + i); // 42, 43, 44, 45, 46
    }

    ll.removeLast();
    // ll.removeFirst();

    // delete all:
    // for(int i = 0; i < ll.size(); i++){
    //     ll.removeLast();
    // }

    // O(N^2) = O(N) * O(N)
    for(int i = 0; i < ll.size(); ++i){ // O(N)
        std::cout << ll.get(i) << std::endl; // O(N)
    }

    // LinkedList<double>::iterator it = ll.begin();
    // std::cout << "(*it).data: " << (*it).data << std::endl;
    // ++it;
    // std::cout << "(*it).data: " << (*it).data << std::endl;

    std::cout << "\nRange-based for loop example:" << std::endl;
    for(auto & e: ll){
        std::cout << e.data << std::endl;
    }

    std::cout << "\nFind example:" << std::endl;
    int find_index = ll.find(43);
    std::cout << "find_index: " << find_index << std::endl;

    std::cout << "\nReverse example:" << std::endl;
    ll.reverse(); // reverse
    ll.reverse(); // normal
    ll.reverse(); // reverse
    for(auto & e: ll){
        std::cout << e.data << std::endl;
    }

    std::cout << "--- LinkedList.cpp execution ended! ---" << std::endl;

}


/*
Build and Run:
g++ LinkedList.cpp -Wall -o LinkedList.o && ./LinkedList.o
*/
