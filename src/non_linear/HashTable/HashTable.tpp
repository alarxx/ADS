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
#ifndef _HASH_TABLE_TPP_
#define _HASH_TABLE_TPP_

#include <iostream>
#include <stdexcept> // runtime_error, out_of_range
#include <type_traits>
#include <concepts>
#include <functional> // std::hash
#include <cstddef> // std::size_t
#include <algorithm> // swap
#include <new> // bad_alloc

// src/non_linear/HashTable/HashTable.tpp
// #include "../../linear/LinkedList/LinkedList.tpp"
// using linkedlist::LinkedList;
// Использование стандартной библиотеки сделает этот пример атомарным
#include <list>

/*
    See : `./dev/hash.cpp` for specialization of std::hash<T> for custom T class.

    ------

    https://en.cppreference.com/w/cpp/utility/hash

    std::hash<T>() -> std::size_t

    std::hash<T> используется в:
    - std::unordered_set
    - std::unordered_multiset
    - std::unordered_map
    - std::unordered_multimap

    - If k1 == k2 is true, h(k1) == h(k2) is also true.
    - Unless std::hash<Key> is a program-defined specialization, h(k1) will never throw an exception.

    Custom specialization of std::hash can be injected in namespace std.
    Hash:
        FunctionObject,
        CopyConstructible,
        Destructible
    Default Constructable
    Copy Assignable
    Swappable
*/
template <typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
    { a == a } -> std::convertible_to<bool>;
    std::is_default_constructible_v<std::hash<T>>;
};

template <typename T>
concept DefaultConstructable = /*std::is_default_constructible_v<T>*/ requires {
    T{}; // Проверка, что тип можно создать без параметров. Нужно в operator[], когда ключа нет, мы создаем default-ный объект.
};




/*
    Implemention of Closed Addressing ( Separate Chaining )
    There are also Open Addressing :
        - Linear Probing,
        - Quadratic Probing,
        - Double Hashing

    Value must be Default Constructable
    Because table[key] creates new default Value object if key isn't in the table


    Когда увеличивать hash table?
    if load factor > 0.75, без разницы на кол-во в бакете, we assume our hash function ranges Uniform Distribution

    0: {3}, {6}
    1: {1}, {4}
    2: {2}, {5}, {8}
    3: <- increase next time

 **/

// SFINAE Solution from LinkedList::iterator
// template <typename T, typename U = void>
// class iterator {};
//
// template <typename T> // T = LinkedList<U>::Node
// class iterator <T, std::void_t<
//     decltype(std::declval<T>().next),
//     decltype(std::declval<T>().prev)
// >> {

template <Hashable K, DefaultConstructable V>
class HashTable { // Closed Addressing ( Separate Chaining )
public:

    template <Hashable K_, typename V_>
    struct HashNode {
        K_ key;
        V_ value;
        HashNode(K_ key, V_ value) : key(key), value(value) {
            // std::cout << "HashNode Constructor: " << value << std::endl;
        }
        ~HashNode(){ std::cout << "HashNode Destructor: " << value << std::endl; }
    };

    std::hash<K> hash_fn;

    int capacity; // number of buckets - M
    int N; // общее количество элементов
    int load; // (Load Factor) сколько bucket-ов заполнено в chainArray

    // rehash if load_factor > 0.75
    double load_factor(){ return (double) load / (double) capacity; }

    std::list<HashNode<K, V>> * chainArray; // buckets - array of singly-linked lists of HashNodes<K, V> (chains)

    HashTable(int capacity = 11) : hash_fn(), capacity(capacity), N(0), load(0) {
        std::cout << "HashTable Constructor" << std::endl;
        /*
         * Normally it should not throw an exception, only on Memory Allocation fails
         * OutOfMemoryError in Java
         * in C++ 'new' throws std::bad_alloc if memory allocation fails, use try{}catch(const std::bad_alloc& e){}
         * in C malloc from <stdlib.h> returns NULL, so check if returned value is NULL in C
         */
        chainArray = new std::list<HashNode<K, V>>[capacity];
    }

    ~HashTable(){
        std::cout << "HashTable Destructor" << std::endl;
        if(chainArray != nullptr){
            delete[] chainArray;
        }
    }

    // bucket index
    int hash(const K& key) const {
        // abs(hash) % capacity
        return (hash_fn(key) & 0x7fffffff) % capacity;
    }

    V& put(const K& key, const V& value) {
        if(load_factor() > 0.75 /* || true */){
            rehash();
        }

        int i = hash(key);

        // std::cout << "key: " << key << ", index: "<< i << std::endl;

        std::list<HashNode<K, V>>& bucket = chainArray[i];

        if(bucket.empty()){
            ++load;
        }
        else {
            // if key exists
            // Нужно проверить есть ли HashNode с совпадающим ключом
            // Если есть совпадающий ключ, то нужно не добавлять node, а обновить значение value
            for (auto & node : bucket) {
                // std::cout << key << ": " << node.value << std::endl;
                if(key == node.key){
                    node.value = value;
                    return node.value;
                }
            }
        }

        // else key doesn't exist
        // bucket.push_back( HashNode<K, V>(key, value) );
        auto & node = bucket.emplace_back(key, value);
        ++N;
        return node.value;
    }

    // Get
    // key may not be there, so we can return std::optional<T>, but for ease I would prefer just to use a pointer
    // but ... I will just throw an Error
    V& at(const K& key) const {
        // return table.at(key);

        int i = hash(key);

        std::list<HashNode<K, V>>& bucket = chainArray[i];

        for (auto & node : bucket) {
            // std::cout << key << ": " << node.value << std::endl;
            if(key == node.key){
                return node.value;
            }
        }

        throw std::out_of_range("Error: Key doesn't exist!");
    }

    // creates node{key, value} if doesn't exist
    V& operator [] (const K& key){
        // return table.at(key);
        int i = hash(key);

        std::list<HashNode<K, V>>& bucket = chainArray[i];

        // O(N), but N is so low, that we can say it is O(1)
        for (auto & node : bucket) {
            // std::cout << key << ": " << node.value << std::endl;
            if(key == node.key){
                return node.value;
            }
        }

        return put(key, V());
    }


    bool contains(const K& key){
        int i = hash(key);

        std::list<HashNode<K, V>>& bucket = chainArray[i];

        for (auto & node : bucket) {
            // std::cout << key << ": " << node.value << std::endl;
            if(key == node.key){
                return true;
            }
        }

        return false;
    }

    void rehash(){
        std::cout << "rehash" << std::endl;
        // capacity = (int) (1.5 * capacity); // min
        // why to use prime number as capacity?

        int prev_capacity = capacity;

        load = 0;
        N = 0;
        capacity = (int) (1.5 * capacity);

        // HashTable<K, V> _table((int) (1.5 * capacity));
        std::list<HashNode<K, V>> * _chainArray = new std::list<HashNode<K, V>>[capacity];

        // traverse and c
        for(int i = 0; i < prev_capacity; i++){
            std::list<HashNode<K, V>>& bucket = chainArray[i];
            for (auto & node : bucket) {
                int j = hash(node.key);

                std::list<HashNode<K, V>>& _bucket = _chainArray[j];

                if(_bucket.empty()){ ++load; }
                _bucket.emplace_back(node.key, node.value);
                ++N;
            }
        }

        std::list<HashNode<K, V>> * tmp = chainArray;
        chainArray = _chainArray;
        delete[] tmp;

    }
};

#endif

// почему BST delete -/n'? No it is logN
// sqrt(N):
//    Find number of divisors
//    Find sum of divisors
//    Find Euler's totient

