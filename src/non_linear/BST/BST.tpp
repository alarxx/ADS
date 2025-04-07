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
#ifndef _BST_TPP_
#define _BST_TPP_

#include <iostream>
#include <stdexcept> // runtime_error, out_of_range
#include <type_traits>
#include <concepts>
#include <functional> // std::hash
#include <cstddef> // std::size_t
#include <algorithm> // swap
#include <new> // bad_alloc

template <typename T>
concept Comparable = requires(T a, T b){
    {a >  b} -> std::convertible_to<bool>;
    {a <  b} -> std::convertible_to<bool>;
    {a <= b} -> std::convertible_to<bool>;
    {a >= b} -> std::convertible_to<bool>;
    {a == b} -> std::convertible_to<bool>;
    {a != b} -> std::convertible_to<bool>;
};

template <Comparable K, typename V>
class BST {
public:
    template <Comparable K_, typename V_>
    class Node {
    public:
        K_ key;
        V_ value;
        Node<K_, V_> *left, *right;
        /*
        What does `Object& &&` mean?
        Object &  &  = Object &
        Object &  && = Object &
        Object && &  = Object &
        Object && && = Object &&
        */
        Node(K_ & key, V_ & value) : key(key), value(value) {} // 1 copy
        Node(K_ && key, V_ && value) = delete;
    };

    Node<K, V> * root;

    BST() : root(nullptr) {}

    Node<K, V>& insert(K && key, V && value){
        return insert(key, value); // передаем как lvalue, потому что key и value - имена переменных
    }
    Node<K, V>& insert(K & key, V & value){
        Node<K, V> * putty = new Node<K, V>(key, value);
        if(root == nullptr){
            root = putty;
            return *putty;
        }
        else {
            // traverse until nullptr
            Node<K, V> * node = root;
            while(true){
                if(putty->key < node->key){
                    if(node->left == nullptr){
                        node->left = putty;
                        return *putty;
                    }
                    node = node->left;
                }
                else {
                    if(node->right == nullptr){
                        node->right = putty;
                        return *putty;
                    }
                    node = node->right;
                }
            }
        }
    }

    Node<K, V>& get(K && key){ return get(key); }
    Node<K, V>& get(K & key){
        // traverse until key == node.key
        Node<K, V> * node = root;
        int count = 0;
        while(node != nullptr && node->key != key){
            if(key < node->key){
                node = node->left;
            }
            else {
                node = node->right;
            }
            count++;
        }
        std::cout << "get() count: " << count << std::endl;
        if(node == nullptr){
            throw std::out_of_range("Error: Key doesn't exist!");
        }
        return *node;
    }
};

#endif
