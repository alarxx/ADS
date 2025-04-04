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
#ifndef _LINKED_LIST_TPP_
#define _LINKED_LIST_TPP_

#include <iostream>
#include <iterator> // bidirectional_iterator_tag, reverse_iterator
#include <cstddef> // ptrdiff_t
#include <type_traits> // enable_if_t
#include <stdexcept> // runtime_error
#include <algorithm> // swap
#include <new> // bad_alloc

namespace linkedlist {

// Forward Declaration
// Because LinkedList depends on iterator, and iterator on LinkedList::Node
template <typename T>
class iterator;

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
        /*
            LinkedList::Node единственный кто будет хранить lvalue значение data,
            При этом копия будет создаваться только, если передать lvalue data.
            В общем, тут хитрая оптимизация хранение объекта в Node.
            Node должен хранить lvalue значение объекта, не reference объекта, и не pointer на объект.

            Простым решением было бы сделать конструктор с pass-by-value и всегда создавать копию:
            Node(R data) : data(data), next(nullptr), prev(nullptr) {}

            Но, тут, получается, единственный конструктор принимает rvalue и крадет данные, не созадавая лишних копий
            Это нужно учитывать, и это учитывается в `add` функциях.
        */
        Node() = delete; // Когда может понадобиться запоздалый assignment data?
        Node(R & data) = delete; // Передавать reference точно плохая идея (извне могут удалить объект), лучше Node будет полным обладателем data
        Node(R && rvalue) : data(std::move(rvalue)), next(nullptr), prev(nullptr) {}

        // Rule of 5
        ~Node() = default; // можно было бы сделать цепное удаление next, но пусть Node будет максимально простым
        Node(Node & other) = delete; // можно было бы разрешить копирование, но зачем копировать Node и data лишний раз
        Node & operator = (Node & other) = delete;
        Node(Node && other) = delete; // move semantics для Node никогда не понадобится
        Node & operator = (Node && other) = delete;
    };

private:
    Node<T> * _head;
    Node<T> * _tail;
    int _size;

public:
    explicit LinkedList() : _head(nullptr), _tail(nullptr), _size(0) {}

    ~LinkedList(){
        /*
         * Можно было бы реализовать цепочную реакцию удаления:
         * if(_head != nullptr){ delete _head; _head = nullptr; _tail = nullptr; }
         */
        Node<T> * node = _head;
        Node<T> * next;
        // for(int i = 0; i < _size; i++){
        while(node != nullptr){
            std::cout << "deallocate Node{" << typeid(node->data).name() << "}" << std::endl;
            next = node->next; // memorize
            delete node; node = nullptr;
            node = next;
        }
    }

    int size(){
        std::cout << "size()" << std::endl;
        return _size;
    }


// --- Append ---

    // --- addLast ---
private:
    // addLast: O(1)
    T& addLast(Node<T> * new_node){
        /*
         * Normally it should not throw an exception, only on Memory Allocation fails
         * OutOfMemoryError in Java
         * in C++ 'new' throws std::bad_alloc if memory allocation fails, use try{}catch(const std::bad_alloc& e){}
         * in C malloc from <stdlib.h> returns NULL, so check if returned value is NULL in C
         */
        // Node<T> * new_node = new Node(item);

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

        return _tail->data;
    }
public:
    T& addLast(T & lvalue){ // 1 copy
        // std::cout << "addLast(lvalue)" << std::endl;
        T item = lvalue; // copy
        Node<T> * new_node = new Node(std::move(item));
        return addLast(new_node);
    }
    T& addLast(T && rvalue){ // 0 copy, 1 rvalue object
        // std::cout << "addLast(rvalue)" << std::endl;
        // T item = std::move(rvalue); // 1 object which is stealed
        Node<T> * new_node = new Node(std::move(rvalue)); // 1 copy object
        return addLast(new_node);
    }

    // --- addFirst ---
private:
    // addFirst: O(1)
    T& addFirst(Node<T> * new_node){
        // Node<T> * new_node = new Node(item);
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
        return _head->data;
    }

public:
    T& addFirst(T & lvalue){
        T item = lvalue; // copy
        Node<T> * new_node = new Node(std::move(item));
        return addFirst(new_node);
    }
    T& addFirst(T && rvalue){
        Node<T> * new_node = new Node(std::move(rvalue));
        return addFirst(new_node);
    }
    // ------


// --- Remove ---

    // --- removeLast ---
public:
    // remove: O(1)
    /*
        Тут тоже оптимизация.
        Мы должны удалить память выделенную для Node.
        Как тогда вернуть значение, да так, чтобы оно потом само удалилось?
        Мы освобождаем память, но возвращаем stealed объект in Stack Memory
        + тут RVO, из-за чего не должна создаваться копия при возврате.
    */
    T removeLast(){
        std::cout << "remove last, _size = (" << _size  << " -> " << _size - 1 << ")" << std::endl;

        if(_size == 0){
            throw std::runtime_error("Error: cannot remove last, linked list is empty");
        }

        Node<T> * tail = _tail;
        T data = std::move(tail->data); // In Stack Memory, so it will free automatically

        if(_size == 1){
            _head = nullptr;
            _tail = nullptr;
        }
        else {
            _tail = tail->prev;
            _tail->next = nullptr; // до этого tail.prev.next = tail

            delete tail; // Delete Memory Allocated for Node
            tail = nullptr;
        }

        --_size;

        return data; // RVO
    }

    // --- removeFirst ---
public:
    // remove: O(1)
    T removeFirst(){
        std::cout << "remove first, _size = (" << _size  << " -> " << _size - 1 << ")" << std::endl;

        if(_size == 0){
            throw std::runtime_error("Error: cannot remove first, linked list is empty");
        }

        Node<T> * head = _head;
        T data = std::move(head->data);

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


// --- Get ---
public:
    // get: O(N)
    T& get(int index){
        if(index < 0 || index >= _size /*_size == 0)*/){
            // Normal behaviour is to throw an error, not to return NULL
            throw std::out_of_range("Error: index out of range");
        }
        Node<T> * node = _head;
        // for(int i = 0; i < index; ++i, node = node->next){}
        while(index != 0){
            node = node->next;
            --index;
        }
        return node->data;
    }

    T& getFirst(){
        if(_head == nullptr){
            throw std::runtime_error("Error: can't get first, linked list is empty");
        }
        return _head->data;
    }
    T& getLast(){
        if(_tail == nullptr){
            throw std::runtime_error("Error: can't get last, linked list is empty");
        }
        return _tail->data;
    }

public:
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

public:
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

    void clean(){
        for(int n = _size, i = 0; i < n; i++){ // 0 1 2 3 4 5
            removeLast();
        }
    }

// --- iterator ---
public:
    using iterator = linkedlist::iterator<T>;
    // using iterator = ::iterator<T>; // iterator variable shadowing, so we use :: - global namespace.
    iterator begin(){ return iterator(_head); }
    iterator end(){ return iterator(_tail != nullptr ? _tail->next : nullptr); }
    // ------
};

// ------

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

// // Does T has members next and prev
// template <typename T, typename U = void>
// class iterator {};
//
// template <typename T> // T = LinkedList<U>::Node
// class iterator <T, std::void_t<
//     decltype(std::declval<T>().next),
//     decltype(std::declval<T>().prev)
// >> {
template <typename T>
class iterator {
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
    typename linkedlist::LinkedList<T>::Node<T> * _ptr;
public:
    iterator(typename linkedlist::LinkedList<T>::Node<T> * ptr) : _ptr(ptr) {}

    reference operator * (){ return _ptr->data; }
    pointer operator -> (){ return &(_ptr->data); }

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

} // namespace linkedlist

#endif
